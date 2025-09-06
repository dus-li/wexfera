/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <arch/fault.h>
#include <arch/irq.h>
#include <arch/scs.h>

#include <kernel/panic.h>

#include <lib/array_size.h>
#include <lib/bits.h>
#include <lib/compiler.h>
#include <lib/log.h>
#include <lib/reg.h>

#define FAULT_HARDFAULT  (3)
#define FAULT_MEMMANAGE  (4)
#define FAULT_BUSFAULT   (5)
#define FAULT_USAGEFAULT (6)

#define FAULT_ENABLE_BITS                                        \
	(SCS_SCB_SHCSR_USGFAULTENA | SCS_SCB_SHCSR_BUSFAULTENA | \
	    SCS_SCB_SHCSR_MEMFAULTENA)

extern void *const LD_DATA_FAULT_HOOKS_BEG;
extern void *const LD_DATA_FAULT_HOOKS_END;

static void __used __fault_hook_dummy(const struct ctx_full *_)
{
	// Intentionally empty
}
FAULT_HOOK(__fault_hook_dummy);

void fault_enable(void)
{
	union32(FAULT_ENABLE_BITS, SCS_SCB_SHCSR);
}

static const char *__fault_names[] = {
	[FAULT_HARDFAULT]  = "HardFault",
	[FAULT_MEMMANAGE]  = "MemManage",
	[FAULT_BUSFAULT]   = "BusFault",
	[FAULT_USAGEFAULT] = "UsageFault",
};

static void __fault_describe_memmanage(u32 cfsr, const char *pfx)
{
	const u32 MMARVALID = 1U << 7;

	u32 addr  = read32(SCS_SCB_MMFAR);
	u32 nofar = ~MMARVALID & BITS_EXTRACT(cfsr, SCS_SCB_CFSR_MEMMANAGE);

	if (nofar & (1U << 0)) // IACCVIOL
		log_always("%s: Instruction access violation\n", pfx);

	if (nofar & (1U << 1)) // DACCVIOL
		log_always("%s: Data access violation\n", pfx);

	if (nofar & (1U << 3)) // MUNSTKERR
		log_always("%s: Stack pop error\n", pfx);

	if (nofar & (1U << 4)) // MSTKERR
		log_always("%s: Stack push error\n", pfx);

	if (nofar & (1U << 5)) // MLSPERR
		log_always("%s: FP lazy state preservation error\n", pfx);

	if (cfsr & MMARVALID)
		log_always("%s: Caused by access to %#x\n", pfx, addr);
}

static void __fault_describe_bus(u32 cfsr, const char *pfx)
{
	const u32 BFARVALID = 1U << 7;

	u32 addr  = read32(SCS_SCB_BFAR);
	u32 nofar = ~BFARVALID & BITS_EXTRACT(cfsr, SCS_SCB_CFSR_BUSFAULT);

	if (nofar & (1U << 0)) // IBUSERR
		log_always("%s: Instruction prefetch bus fault\n", pfx);

	if (nofar & (1U << 1)) // PRECISBUS
		log_always("%s: Precise data bus error\n", pfx);

	if (nofar & (1U << 2)) // IMPRECISBUS
		log_always("%s: Imprecise data bus error\n", pfx);

	if (nofar & (1U << 3)) // UNSTKERR
		log_always("%s: Stack pop error\n", pfx);

	if (nofar & (1U << 4)) // STKERR
		log_always("%s: Stack push error\n", pfx);

	if (nofar & (1U << 5)) // LSPERR
		log_always("%s: FP lazy state preservation error\n", pfx);

	if (cfsr & BFARVALID)
		log_always("%s: Caused by access to %#x\n", pfx, addr);
}

static void __fault_describe_usage(u32 cfsr, const char *pfx)
{
	cfsr = BITS_EXTRACT(cfsr, SCS_SCB_CFSR_USAGEFAULT);

	if (cfsr & (1U << 0)) // UNDEFINSTR
		log_always("%s: Undefined instruction error\n", pfx);

	if (cfsr & (1U << 1)) // INVSTATE
		log_always("%s: Invalid execution program state\n", pfx);

	if (cfsr & (1U << 2)) // INVPC
		log_always("%s: Invalid PC value on exception return\n", pfx);

	if (cfsr & (1U << 3)) // NOCP
		log_always("%s: Coprocessor not available\n", pfx);

	if (cfsr & (1U << 8)) // UNALIGNED
		log_always("%s: Unaligned memory access\n", pfx);

	if (cfsr & (1U << 9)) // DIVBYZERO
		log_always("%s: Divide by zero\n", pfx);
}

static void __fault_describe_hard(u32 cfsr, const char *pfx)
{
	u32 hfsr = read32(SCS_SCB_HFSR);

	if (hfsr & (1U << 1)) // VECTTBL
		log_always("%s: Vector table read fault\n", pfx);

	// Escalated faults
	if (hfsr & (1U << 30)) { // FORCED
		log_always("%s: Escalated fault\n", pfx);
		__fault_describe_memmanage(cfsr, pfx);
		__fault_describe_bus(cfsr, pfx);
		__fault_describe_usage(cfsr, pfx);
	}
}

static void (*const __fault_describe_map[])(u32, const char *) = {
	[FAULT_HARDFAULT]  = __fault_describe_hard,
	[FAULT_MEMMANAGE]  = __fault_describe_memmanage,
	[FAULT_BUSFAULT]   = __fault_describe_bus,
	[FAULT_USAGEFAULT] = __fault_describe_usage,
};

static void _fault_log(struct ctx_full *full, unsigned type)
{
	u32 cfsr = read32(SCS_SCB_CFSR);

	__fault_describe_map[type](cfsr, __fault_names[type]);
	_panic(full, __fault_names[type]);
}

void _fault_run_hooks(const struct ctx_full *regs)
{
	const fault_hook_t *beg = (fault_hook_t *)&LD_DATA_FAULT_HOOKS_BEG;
	const fault_hook_t *end = (fault_hook_t *)&LD_DATA_FAULT_HOOKS_END;

	u32 count = ((u32)end - (u32)beg) / sizeof(fault_hook_t);

	for (u32 i = 0; i < count; ++i)
		beg[i](regs);
}

u32 __fault_get_isr(void)
{
	u32 ipsr;

	asm volatile("mrs %0, ipsr" : "=r"(ipsr));

	return ipsr & 0x1FF;
}

/**
 * Handle a fault.
 * @param regs Register values of the offending process.
 */
void fault_dispatcher(struct irq_stacked *regs)
{
	struct ctx_full full;

	irq_disable();
	ctx_collect(&full, regs);
	_fault_log(&full, __fault_get_isr());
	_fault_run_hooks(&full);

	for (;;)
		;

	__builtin_unreachable();
}
