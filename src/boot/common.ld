/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

ENTRY(boot0);

INCLUDE memory.ld

SECTIONS
{
	.corevtab :
	{
		. = ALIGN(4);
		LD_COREVTAB_BEG = .;
		KEEP(*(.vtab))
		. = ALIGN(4);
		LD_COREVTAB_END = .;
	} >flash

	.text :
	{
		. = ALIGN(4);
		*(.text)
		*(.text*)
		. = ALIGN(4);
	} >flash

	.rodata :
	{
		. = ALIGN(4);
		*(.rodata)
		*(.rodata*)
		. = ALIGN(4);
	} >flash

	LD_DATA_LMA_BEG = .;
	.data : AT(LD_DATA_LMA_BEG)
	{
		. = ALIGN(4);
		LD_DATA_BEG = .;
		*(.data)
		*(.data*)
		. = ALIGN(4);
		LD_DATA_END = .;
	} >sram

	.bss :
	{
		. = ALIGN(4);
		LD_BSS_BEG = .;
		*(.bss)
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		LD_BSS_END = .;
	} >sram
}
