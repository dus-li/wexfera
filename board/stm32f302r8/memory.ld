/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

MEMORY
{
        flash (rx): ORIGIN = 0x08000000, LENGTH = 64K
        sram (rwx): ORIGIN = 0x20000000, LENGTH = 16K
}

LD_STACK_END = 0x20004000;
