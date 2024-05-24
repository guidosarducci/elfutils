/* MIPS specific core note handling.
   Copyright (C) 2024 CIP United Inc.
   This file is part of elfutils.

   This file is free software; you can redistribute it and/or modify
   it under the terms of either

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at
       your option) any later version

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at
       your option) any later version

   or both in parallel, as here.

   elfutils is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see <http://www.gnu.org/licenses/>.  */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <elf.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/time.h>
#include "libebl_CPU.h"

#ifndef BITS
# define BITS 32
#define BACKEND	mips_
#else
# define BITS 64
# define BACKEND mips64_
#endif

#define PRSTATUS_REGS_SIZE	(45 * (BITS / 8))
static const Ebl_Register_Location prstatus_regs[] =
  {
    { .offset = 0, .regno = 0, .count = (BITS == 32 ? 40 : 34), .bits = BITS },
    { .offset = BITS/8 * (BITS == 32 ? 41 : 35), .regno = (BITS == 32 ? 41 : 35), .count = (BITS == 32 ? 4 : 10), .bits = BITS },
  };

#define PRSTATUS_REGSET_ITEMS						\
  {									\
    .name = "pc", .type = ELF_T_ADDR, .format = 'x',			\
    .offset = offsetof (struct EBLHOOK(prstatus), pr_reg) + ((BITS/8) * (BITS == 32 ? 40 : 34)),	\
    .group = "register",						\
    .pc_register = true							\
  }

static const Ebl_Register_Location mips_fpregset_regs[] =
  {
    { .offset = 0, .regno = 38, .count = 32, .bits = 64 }, /* fp0-fp31 */
  };

static const Ebl_Core_Item mips_fpregset_items[] =
  {
    {
      .name = "fcs", .type = ELF_T_WORD, .format = 'x',
      .offset = 32 * 8, .group = "register"
    },
    {
      .name = "fir", .type = ELF_T_WORD, .format = 'x',
      .offset = 32 * 8 + 4, .group = "register"
    }
  };

#if BITS == 32
# define ULONG			uint32_t
# define ALIGN_ULONG		4
# define TYPE_ULONG		ELF_T_WORD
#define TYPE_LONG		ELF_T_SWORD
#else
#define ULONG			uint64_t
#define ALIGN_ULONG		8
#define TYPE_ULONG		ELF_T_XWORD
#define TYPE_LONG		ELF_T_SXWORD
#endif
#define PID_T			int32_t
#define	UID_T			uint32_t
#define	GID_T			uint32_t
#define ALIGN_PID_T		4
#define ALIGN_UID_T		4
#define ALIGN_GID_T		4
#define TYPE_PID_T		ELF_T_SWORD
#define TYPE_UID_T		ELF_T_WORD
#define TYPE_GID_T		ELF_T_WORD

#define	EXTRA_NOTES \
  EXTRA_REGSET_ITEMS (NT_FPREGSET, 32 * 8 + 4 * 2, mips_fpregset_regs, mips_fpregset_items)

#include "linux-core-note.c"
