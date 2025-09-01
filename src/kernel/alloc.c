/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <arch/spinlock.h>

#include <kernel/alloc.h>

#include <lib/container.h>
#include <lib/error.h>
#include <lib/list.h>
#include <lib/log.h>
#include <lib/round.h>
#include <lib/string.h>

extern void *const LD_DYNALLOCS_BEG;
extern void *const LD_DYNALLOCS_END;

#define ALLOC_HEAP_SIZE (((u32)(&LD_DYNALLOCS_END) - (u32)(&LD_DYNALLOCS_BEG)))

/** Block metadata. */
struct alloc_meta {
	struct list node;
	size_t      size; ///< Size of the block, including metadata.
	bool        is_free;
};

static LIST_HEAD(_alloc_blocks);    ///< Head of the list of blocks.
static spinlock_t _alloc_heap_lock; ///< Spinlock for heap operations.

err_t alloc_init(void)
{
	struct alloc_meta *heap = (typeof(heap))&LD_DYNALLOCS_BEG;

	spinlock_init(&_alloc_heap_lock);
	list_init(&_alloc_blocks);

	if (ALLOC_HEAP_SIZE < sizeof(*heap))
		return -ERR_MEM;

	heap->size    = ALLOC_HEAP_SIZE - sizeof(*heap);
	heap->is_free = true;
	list_init(&heap->node);

	err_t tmp = list_append(&_alloc_blocks, &heap->node);
	if (tmp != ERR_NONE)
		return tmp;

	log_info("Initialized heap of size %u\n", ALLOC_HEAP_SIZE);
	return ERR_NONE;
}

/**
 * Locate first free block of at least given size.
 * @param total Size that needs to be accomodated by the block.
 *
 * @return Address of metadata of a block that can fit allocation of given size.
 * @return @a NULL if no block was found.
 */
static struct alloc_meta *_alloc_find_fit(size_t total)
{
	struct alloc_meta *ret;
	struct list       *node;

	list_foreach (node, &_alloc_blocks) {
		ret = containerof(node, struct alloc_meta, node);
		if (ret->is_free && ret->size >= total)
			return ret;
	}

	return NULL;
}

/**
 * Split a block into two smaller blocks.
 * @param block Block that is to be split.
 * @param size  Size of the first block.
 *
 * If user requests an allocation that is larger than first available block,
 * then that block is split into two:
 *
 *   1. A block with size equal to @a size.
 *   2. A block with size equal to the size of @a block minus @a size.
 *
 * This function performs that split.
 *
 * @return @ref err_t
 */
static err_t _alloc_split_block(struct alloc_meta *block, size_t size)
{
	struct alloc_meta *new = (void *)block + size;
	err_t ret;

	list_init(&new->node);
	new->size    = block->size - size;
	new->is_free = true;

	ret = list_append(&block->node, &new->node);
	if (ret != ERR_NONE) {
		bzero(new, sizeof(*new));
		return ret;
	}

	block->size = size;

	return ERR_NONE;
}

void *alloc_get(size_t size)
{
	size_t total = ALIGN(size + sizeof(struct alloc_meta), 8);

	if (size == 0)
		return ERR_PENC(ERR_ARG);

	LOCKED_UNTIL_END(spinlock, &_alloc_heap_lock);

	// Attempt to find first block that fits the requested allocation.
	struct alloc_meta *claim = _alloc_find_fit(total);
	if (claim == NULL)
		return ERR_PENC(ERR_MEM);

	// Split the block if size allows. Otherwise remove from the list.
	if (claim->size > total + sizeof(struct alloc_meta)) {
		err_t ret = _alloc_split_block(claim, total);
		if (ret != ERR_NONE)
			return ERR_PENC(ret);
	}

	claim->is_free = false;

	return (void *)((uintptr_t)claim + sizeof(*claim));
}

static inline struct alloc_meta *_alloc_meta_suc(struct alloc_meta *block)
{
	if (block->node.next == &_alloc_blocks)
		return NULL;

	return containerof(block->node.next, struct alloc_meta, node);
}

static inline struct alloc_meta *_alloc_meta_pre(struct alloc_meta *block)
{
	if (block->node.prev == &_alloc_blocks)
		return NULL;

	return containerof(block->node.prev, struct alloc_meta, node);
}

static err_t __alloc_merge(struct alloc_meta *fst, struct alloc_meta *snd)
{
	if (fst == NULL || snd == NULL || fst == snd)
		return -ERR_ARG;

	if (!fst->is_free || !snd->is_free)
		return -ERR_ILLEGAL;

	if (fst != _alloc_meta_pre(snd) || snd != _alloc_meta_suc(fst))
		return -ERR_CORRUPT;

	err_t ret = list_remove(&snd->node);
	if (ret != ERR_NONE)
		return ret;

	fst->size += snd->size;
	bzero(snd, sizeof(*snd));

	return ERR_NONE;
}

static err_t _alloc_merge(struct alloc_meta *fst, struct alloc_meta *snd)
{
	// Can't merge 'em, but its no biggie - maybe some other time.
	if (!fst || !snd || fst == snd || !fst->is_free || !snd->is_free)
		return ERR_NONE;

	return __alloc_merge(fst, snd);
}

err_t alloc_free(void *addr)
{
	struct alloc_meta *block = (typeof(block))(addr - sizeof(*block));

	if (addr == NULL)
		return -ERR_ARG;

	LOCKED_UNTIL_END(spinlock, &_alloc_heap_lock);

	// Detect double free attempts
	if (block->is_free)
		return -ERR_DONE;

	block->is_free = true;

	struct alloc_meta *pre = _alloc_meta_pre(block);
	struct alloc_meta *suc = _alloc_meta_suc(block);

	// Try to defrag neighbourhood of the freed block.
	err_t tmp = _alloc_merge(block, suc) ?: _alloc_merge(pre, block);
	if (tmp != ERR_NONE) {
		log_error("Block list corruption detected (code:%u)!\n", -tmp);
		return -ERR_INTERNAL;
	}

	return ERR_NONE;
}

#if (BOARD_DEBUG_FEATURES == 1)
void alloc_describe_heap(void)
{
	struct alloc_meta *tmp;
	struct list       *node;

	log_debug("Current heap structure:\n");

	list_foreach (node, &_alloc_blocks) {
		tmp = containerof(node, struct alloc_meta, node);

		size_t    size  = tmp->size - sizeof(*tmp);
		uintptr_t start = (uintptr_t)tmp + sizeof(*tmp);

		log_debug("start:%#x,size:%u,free:%u\n",
		    start,
		    size,
		    tmp->is_free ? 1 : 0);
	}
}
#endif
