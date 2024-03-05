/* MIPS specific symbolic name handling.
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

#include <system.h>

#include <elf.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#define BACKEND		mips_
#include "libebl_CPU.h"
#include "libelfP.h"

/* Check for the simple reloc types.  */
Elf_Type
mips_reloc_simple_type (Ebl *ebl, int type,
			   int *addsub __attribute__ ((unused)))
{
  int typeNew = type;
  if(ebl->elf->class == ELFCLASS64)
    typeNew = ELF64_MIPS_R_TYPE1(type);
  switch (typeNew)
    {
    case R_MIPS_64:
      return ELF_T_XWORD;
    case R_MIPS_32:
      return ELF_T_WORD;
    case R_MIPS_16:
      return ELF_T_HALF;

    default:
      return ELF_T_NUM;
    }
}

/* copy binutils-2.34/binutils/readelf.c get_mips_section_type_name  */
const char *
mips_section_type_name (int type,
		       char *buf __attribute__ ((unused)),
		       size_t len __attribute__ ((unused)))
{
  switch (type)
    {
    case SHT_MIPS_LIBLIST:
      return "MIPS_LIBLIST";
    case SHT_MIPS_MSYM:
      return "MIPS_MSYM";
    case SHT_MIPS_CONFLICT:
      return "MIPS_CONFLICT";
    case SHT_MIPS_GPTAB:
      return "MIPS_GPTAB";
    case SHT_MIPS_UCODE:
      return "MIPS_UCODE";
    case SHT_MIPS_DEBUG:
      return "MIPS_DEBUG";
    case SHT_MIPS_REGINFO:
      return "MIPS_REGINFO";
    case SHT_MIPS_PACKAGE:
      return "MIPS_PACKAGE";
    case SHT_MIPS_PACKSYM:
      return "MIPS_PACKSYM";
    case SHT_MIPS_RELD:
      return "MIPS_RELD";
    case SHT_MIPS_IFACE:
      return "MIPS_IFACE";
    case SHT_MIPS_CONTENT:
      return "MIPS_CONTENT";
    case SHT_MIPS_OPTIONS:
      return "MIPS_OPTIONS";
    case SHT_MIPS_SHDR:
      return "MIPS_SHDR";
    case SHT_MIPS_FDESC:
      return "MIPS_FDESC";
    case SHT_MIPS_EXTSYM:
      return "MIPS_EXTSYM";
    case SHT_MIPS_DENSE:
      return "MIPS_DENSE";
    case SHT_MIPS_PDESC:
      return "MIPS_PDESC";
    case SHT_MIPS_LOCSYM:
      return "MIPS_LOCSYM";
    case SHT_MIPS_AUXSYM:
      return "MIPS_AUXSYM";
    case SHT_MIPS_OPTSYM:
      return "MIPS_OPTSYM";
    case SHT_MIPS_LOCSTR:
      return "MIPS_LOCSTR";
    case SHT_MIPS_LINE:
      return "MIPS_LINE";
    case SHT_MIPS_RFDESC:
      return "MIPS_RFDESC";
    case SHT_MIPS_DELTASYM:
      return "MIPS_DELTASYM";
    case SHT_MIPS_DELTAINST:
      return "MIPS_DELTAINST";
    case SHT_MIPS_DELTACLASS:
      return "MIPS_DELTACLASS";
    case SHT_MIPS_DWARF:
      return "MIPS_DWARF";
    case SHT_MIPS_DELTADECL:
      return "MIPS_DELTADECL";
    case SHT_MIPS_SYMBOL_LIB:
      return "MIPS_SYMBOL_LIB";
    case SHT_MIPS_EVENTS:
      return "MIPS_EVENTS";
    case SHT_MIPS_TRANSLATE:
      return "MIPS_TRANSLATE";
    case SHT_MIPS_PIXIE:
      return "MIPS_PIXIE";
    case SHT_MIPS_XLATE:
      return "MIPS_XLATE";
    case SHT_MIPS_XLATE_DEBUG:
      return "MIPS_XLATE_DEBUG";
    case SHT_MIPS_WHIRL:
      return "MIPS_WHIRL";
    case SHT_MIPS_EH_REGION:
      return "MIPS_EH_REGION";
    case SHT_MIPS_XLATE_OLD:
      return "MIPS_XLATE_OLD";
    case SHT_MIPS_PDR_EXCEPTION:
      return "MIPS_PDR_EXCEPTION";
    case SHT_MIPS_ABIFLAGS:
      return "MIPS_ABIFLAGS";
    case SHT_MIPS_XHASH:
      return "MIPS_XHASH";
    default:
      break;
    }
  return NULL;
}

bool
mips_check_reloc_target_type (Ebl *ebl __attribute__ ((unused)), Elf64_Word sh_type)
{
  return (sh_type == SHT_MIPS_DWARF);
}

/* Check whether given symbol's st_value and st_size are OK despite failing
   normal checks.  */
bool
mips_check_special_symbol (Elf *elf,
			    const GElf_Sym *sym __attribute__ ((unused)),
			    const char *name __attribute__ ((unused)),
			    const GElf_Shdr *destshdr)
{
  size_t shstrndx;
  if (elf_getshdrstrndx (elf, &shstrndx) != 0)
    return false;
  const char *sname = elf_strptr (elf, shstrndx, destshdr->sh_name);
  if (sname == NULL)
    return false;
  return (strcmp (sname, ".got") == 0 || strcmp (sname, ".bss") == 0);
}

/* Check whether SHF_MASKPROC flags are valid.  */
bool
mips_machine_section_flag_check (GElf_Xword sh_flags)
{
  return ((sh_flags &~ (SHF_MIPS_GPREL |
			SHF_MIPS_MERGE |
			SHF_MIPS_ADDR |
			SHF_MIPS_STRINGS |
			SHF_MIPS_NOSTRIP |
			SHF_MIPS_LOCAL |
			SHF_MIPS_NAMES |
			SHF_MIPS_NODUPE)) == 0);
}

/* Check whether machine flags are valid.  */
bool
mips_machine_flag_check (GElf_Word flags)
{
  if ((flags &~ (EF_MIPS_NOREORDER |
		 EF_MIPS_PIC |
		 EF_MIPS_CPIC |
		 EF_MIPS_UCODE |
		 EF_MIPS_ABI2 |
		 EF_MIPS_OPTIONS_FIRST |
		 EF_MIPS_32BITMODE |
		 EF_MIPS_NAN2008 |
		 EF_MIPS_FP64 |
		 EF_MIPS_ARCH_ASE_MDMX |
		 EF_MIPS_ARCH_ASE_M16 |
		 EF_MIPS_ARCH_ASE_MICROMIPS)) == 0)
    return false;

  switch(flags & EF_MIPS_MACH)
    {
    case EF_MIPS_MACH_3900:
    case EF_MIPS_MACH_4010:
    case EF_MIPS_MACH_4100:
    case EF_MIPS_MACH_4111:
    case EF_MIPS_MACH_4120:
    case EF_MIPS_MACH_4650:
    case EF_MIPS_MACH_5400:
    case EF_MIPS_MACH_5500:
    case EF_MIPS_MACH_5900:
    case EF_MIPS_MACH_SB1:
    case EF_MIPS_MACH_9000:
    case EF_MIPS_MACH_LS2E:
    case EF_MIPS_MACH_LS2F:
    case EF_MIPS_MACH_GS464:
    case EF_MIPS_MACH_GS464E:
    case EF_MIPS_MACH_GS264E:
    case EF_MIPS_MACH_OCTEON:
    case EF_MIPS_MACH_OCTEON2:
    case EF_MIPS_MACH_OCTEON3:
    case EF_MIPS_MACH_XLR:
    case EF_MIPS_MACH_IAMR2:
    case 0:
      break;
    default:
      return false;
    }

  switch ((flags & EF_MIPS_ABI))
    {
    case EF_MIPS_ABI_O32:
    case EF_MIPS_ABI_O64:
    case EF_MIPS_ABI_EABI32:
    case EF_MIPS_ABI_EABI64:
    case 0:
      break;
    default:
      return false;
    }

  switch ((flags & EF_MIPS_ARCH))
    {
    case EF_MIPS_ARCH_1:
    case EF_MIPS_ARCH_2:
    case EF_MIPS_ARCH_3:
    case EF_MIPS_ARCH_4:
    case EF_MIPS_ARCH_5:
    case EF_MIPS_ARCH_32:
    case EF_MIPS_ARCH_32R2:
    case EF_MIPS_ARCH_32R6:
    case EF_MIPS_ARCH_64:
    case EF_MIPS_ARCH_64R2:
    case EF_MIPS_ARCH_64R6:
      return true;
    default:
      return false;
    }
  return false;
}

/* copy binutils-2.34/binutils/readelf.c get_machine_flags */
const char *
mips_machine_flag_name (Elf64_Word orig __attribute__ ((unused)), Elf64_Word *flagref)
{
  if (*flagref & EF_MIPS_NOREORDER)
    {
      *flagref &= ~((Elf64_Word) EF_MIPS_NOREORDER);
      return "noreorder";
    }

  if (*flagref & EF_MIPS_PIC)
    {
      *flagref &= ~((Elf64_Word) EF_MIPS_PIC);
      return "pic";
    }

  if (*flagref & EF_MIPS_CPIC)
    {
      *flagref &= ~((Elf64_Word) EF_MIPS_CPIC);
      return "cpic";
    }

  if (*flagref & EF_MIPS_UCODE)
    {
      *flagref &= ~((Elf64_Word) EF_MIPS_UCODE);
      return "ugen_reserved";
    }

  if (*flagref & EF_MIPS_ABI2)
    {
      *flagref &= ~((Elf64_Word) EF_MIPS_ABI2);
      return "abi2";
    }

  if (*flagref & EF_MIPS_OPTIONS_FIRST)
    {
      *flagref &= ~((Elf64_Word) EF_MIPS_OPTIONS_FIRST);
      return "odk first";
    }

  if (*flagref & EF_MIPS_32BITMODE)
    {
      *flagref &= ~((Elf64_Word) EF_MIPS_32BITMODE);
      return "32bitmode";
    }

  if (*flagref & EF_MIPS_NAN2008)
    {
      *flagref &= ~((Elf64_Word) EF_MIPS_NAN2008);
      return "nan2008";
    }

  if (*flagref & EF_MIPS_FP64)
    {
      *flagref &= ~((Elf64_Word) EF_MIPS_FP64);
      return "fp64";
    }

  switch (*flagref & EF_MIPS_MACH)
    {
    case EF_MIPS_MACH_3900:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_3900);
      return "3900";
    case EF_MIPS_MACH_4010:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_4010);
      return "4010";
    case EF_MIPS_MACH_4100:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_4100);
      return "4100";
    case EF_MIPS_MACH_4111:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_4111);
      return "4111";
    case EF_MIPS_MACH_4120:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_4120);
      return "4120";
    case EF_MIPS_MACH_4650:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_4650);
      return "4650";
    case EF_MIPS_MACH_5400:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_5400);
      return "5400";
    case EF_MIPS_MACH_5500:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_5500);
      return "5500";
    case EF_MIPS_MACH_5900:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_5900);
      return "5900";
    case EF_MIPS_MACH_SB1:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_SB1);
      return "sb1";
    case EF_MIPS_MACH_9000:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_9000);
      return "9000";
    case EF_MIPS_MACH_LS2E:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_LS2E);
      return "loongson-2e";
    case EF_MIPS_MACH_LS2F:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_LS2F);
      return "loongson-2f";
    case EF_MIPS_MACH_GS464:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_GS464);
      return "gs464";
    case EF_MIPS_MACH_GS464E:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_GS464E);
      return "gs464e";
    case EF_MIPS_MACH_GS264E:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_GS264E);
      return "gs264e";
    case EF_MIPS_MACH_OCTEON:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_OCTEON);
      return "octeon";
    case EF_MIPS_MACH_OCTEON2:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_OCTEON2);
      return "octeon2";
    case EF_MIPS_MACH_OCTEON3:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_OCTEON3);
      return "octeon3";
    case EF_MIPS_MACH_XLR:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_XLR);
      return "xlr";
    case EF_MIPS_MACH_IAMR2:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH_IAMR2);
      return "interaptiv-mr2";
    case 0:
      /* We simply ignore the field in this case to avoid confusion:
	 MIPS ELF does not specify EF_MIPS_MACH, it is a GNU
	 extension.  */
      break;
    default:
      *flagref &= ~((Elf64_Word) EF_MIPS_MACH);
      return "unknown CPU";
    }
  switch (*flagref & EF_MIPS_ABI)
    {
    case EF_MIPS_ABI_O32:
      *flagref &= ~((Elf64_Word) EF_MIPS_ABI_O32);
      return "o32";
    case EF_MIPS_ABI_O64:
      *flagref &= ~((Elf64_Word) EF_MIPS_ABI_O64);
      return "o64";
    case EF_MIPS_ABI_EABI32:
      *flagref &= ~((Elf64_Word) EF_MIPS_ABI_EABI32);
      return "eabi32";
    case EF_MIPS_ABI_EABI64:
      *flagref &= ~((Elf64_Word) EF_MIPS_ABI_EABI64);
      return "eabi64";
    case 0:
      /* We simply ignore the field in this case to avoid confusion:
	 MIPS ELF does not specify EF_MIPS_ABI, it is a GNU extension.
	 This means it is likely to be an o32 file, but not for
	 sure.  */
      break;
    default:
      *flagref &= ~((Elf64_Word) EF_MIPS_ABI);
      return "unknown ABI";
    }

  if (*flagref & EF_MIPS_ARCH_ASE_MDMX)
    {
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_ASE_MDMX);
      return "mdmx";
    }

  if (*flagref & EF_MIPS_ARCH_ASE_M16)
    {
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_ASE_M16);
      return "mips16";
    }

  if (*flagref & EF_MIPS_ARCH_ASE_MICROMIPS)
    {
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_ASE_MICROMIPS);
      return "micromips";
    }

  switch (*flagref & EF_MIPS_ARCH)
    {
    case EF_MIPS_ARCH_1:
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_1);
      return "mips1";
    case EF_MIPS_ARCH_2:
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_2);
      return "mips2";
    case EF_MIPS_ARCH_3:
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_3);
      return "mips3";
    case EF_MIPS_ARCH_4:
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_4);
      return "mips4";
    case EF_MIPS_ARCH_5:
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_5);
      return "mips5";
    case EF_MIPS_ARCH_32:
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_32);
      return "mips32";
    case EF_MIPS_ARCH_32R2:
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_32R2);
      return "mips32r2";
    case EF_MIPS_ARCH_32R6:
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_32R6);
      return "mips32r6";
    case EF_MIPS_ARCH_64:
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_64);
      return "mips64";
    case EF_MIPS_ARCH_64R2:
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_64R2);
      return "mips64r2";
    case EF_MIPS_ARCH_64R6:
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH_64R6);
      return "mips64r6";
    default:
      *flagref &= ~((Elf64_Word) EF_MIPS_ARCH);
      return "unknown ISA";
    }
  return NULL;
}

/* copy binutils-2.34/binutils/readelf.c get_mips_segment_type  */
const char *
mips_segment_type_name (int segment, char *buf __attribute__ ((unused)),
			size_t len __attribute__ ((unused)))
{
  switch (segment)
    {
    case PT_MIPS_REGINFO:
      return "REGINFO";
    case PT_MIPS_RTPROC:
      return "RTPROC";
    case PT_MIPS_OPTIONS:
      return "OPTIONS";
    case PT_MIPS_ABIFLAGS:
      return "ABIFLAGS";
    default:
      return NULL;
    }
}

bool
mips_dynamic_tag_check (int64_t tag)
{
  return ((tag &~ (DT_MIPS_RLD_VERSION
	  | DT_MIPS_TIME_STAMP
	  | DT_MIPS_ICHECKSUM
	  | DT_MIPS_IVERSION
	  | DT_MIPS_FLAGS
	  | DT_MIPS_BASE_ADDRESS
	  | DT_MIPS_MSYM
	  | DT_MIPS_CONFLICT
	  | DT_MIPS_LIBLIST
	  | DT_MIPS_LOCAL_GOTNO
	  | DT_MIPS_CONFLICTNO
	  | DT_MIPS_LIBLISTNO
	  | DT_MIPS_SYMTABNO
	  | DT_MIPS_UNREFEXTNO
	  | DT_MIPS_GOTSYM
	  | DT_MIPS_HIPAGENO
	  | DT_MIPS_RLD_MAP
	  | DT_MIPS_DELTA_CLASS
	  | DT_MIPS_DELTA_CLASS_NO
	  | DT_MIPS_DELTA_INSTANCE
	  | DT_MIPS_DELTA_INSTANCE_NO
	  | DT_MIPS_DELTA_RELOC
	  | DT_MIPS_DELTA_RELOC_NO
	  | DT_MIPS_DELTA_SYM
	  | DT_MIPS_DELTA_SYM_NO
	  | DT_MIPS_DELTA_CLASSSYM
	  | DT_MIPS_DELTA_CLASSSYM_NO
	  | DT_MIPS_CXX_FLAGS
	  | DT_MIPS_PIXIE_INIT
	  | DT_MIPS_SYMBOL_LIB
	  | DT_MIPS_LOCALPAGE_GOTIDX
	  | DT_MIPS_LOCAL_GOTIDX
	  | DT_MIPS_HIDDEN_GOTIDX
	  | DT_MIPS_PROTECTED_GOTIDX
	  | DT_MIPS_OPTIONS
	  | DT_MIPS_INTERFACE
	  | DT_MIPS_DYNSTR_ALIGN
	  | DT_MIPS_INTERFACE_SIZE
	  | DT_MIPS_RLD_TEXT_RESOLVE_ADDR
	  | DT_MIPS_PERF_SUFFIX
	  | DT_MIPS_COMPACT_SIZE
	  | DT_MIPS_GP_VALUE
	  | DT_MIPS_AUX_DYNAMIC
	  | DT_MIPS_PLTGOT
	  | DT_MIPS_RWPLT
	  | DT_MIPS_RLD_MAP_REL
	  | DT_MIPS_XHASH)) == 0);
}

/* copy binutils-2.34/binutils/readelf.c  get_mips_dynamic_type*/
const char *
mips_dynamic_tag_name (int64_t tag, char *buf __attribute__ ((unused)),
			  size_t len __attribute__ ((unused)))
{
  switch (tag)
    {
    case DT_MIPS_RLD_VERSION:
      return "MIPS_RLD_VERSION";
    case DT_MIPS_TIME_STAMP:
      return "MIPS_TIME_STAMP";
    case DT_MIPS_ICHECKSUM:
      return "MIPS_ICHECKSUM";
    case DT_MIPS_IVERSION:
      return "MIPS_IVERSION";
    case DT_MIPS_FLAGS:
      return "MIPS_FLAGS";
    case DT_MIPS_BASE_ADDRESS:
      return "MIPS_BASE_ADDRESS";
    case DT_MIPS_MSYM:
      return "MIPS_MSYM";
    case DT_MIPS_CONFLICT:
      return "MIPS_CONFLICT";
    case DT_MIPS_LIBLIST:
      return "MIPS_LIBLIST";
    case DT_MIPS_LOCAL_GOTNO:
      return "MIPS_LOCAL_GOTNO";
    case DT_MIPS_CONFLICTNO:
      return "MIPS_CONFLICTNO";
    case DT_MIPS_LIBLISTNO:
      return "MIPS_LIBLISTNO";
    case DT_MIPS_SYMTABNO:
      return "MIPS_SYMTABNO";
    case DT_MIPS_UNREFEXTNO:
      return "MIPS_UNREFEXTNO";
    case DT_MIPS_GOTSYM:
      return "MIPS_GOTSYM";
    case DT_MIPS_HIPAGENO:
      return "MIPS_HIPAGENO";
    case DT_MIPS_RLD_MAP:
      return "MIPS_RLD_MAP";
    case DT_MIPS_RLD_MAP_REL:
      return "MIPS_RLD_MAP_REL";
    case DT_MIPS_DELTA_CLASS:
      return "MIPS_DELTA_CLASS";
    case DT_MIPS_DELTA_CLASS_NO:
      return "MIPS_DELTA_CLASS_NO";
    case DT_MIPS_DELTA_INSTANCE:
      return "MIPS_DELTA_INSTANCE";
    case DT_MIPS_DELTA_INSTANCE_NO:
      return "MIPS_DELTA_INSTANCE_NO";
    case DT_MIPS_DELTA_RELOC:
      return "MIPS_DELTA_RELOC";
    case DT_MIPS_DELTA_RELOC_NO:
      return "MIPS_DELTA_RELOC_NO";
    case DT_MIPS_DELTA_SYM:
      return "MIPS_DELTA_SYM";
    case DT_MIPS_DELTA_SYM_NO:
      return "MIPS_DELTA_SYM_NO";
    case DT_MIPS_DELTA_CLASSSYM:
      return "MIPS_DELTA_CLASSSYM";
    case DT_MIPS_DELTA_CLASSSYM_NO:
      return "MIPS_DELTA_CLASSSYM_NO";
    case DT_MIPS_CXX_FLAGS:
      return "MIPS_CXX_FLAGS";
    case DT_MIPS_PIXIE_INIT:
      return "MIPS_PIXIE_INIT";
    case DT_MIPS_SYMBOL_LIB:
      return "MIPS_SYMBOL_LIB";
    case DT_MIPS_LOCALPAGE_GOTIDX:
      return "MIPS_LOCALPAGE_GOTIDX";
    case DT_MIPS_LOCAL_GOTIDX:
      return "MIPS_LOCAL_GOTIDX";
    case DT_MIPS_HIDDEN_GOTIDX:
      return "MIPS_HIDDEN_GOTIDX";
    case DT_MIPS_PROTECTED_GOTIDX:
      return "MIPS_PROTECTED_GOTIDX";
    case DT_MIPS_OPTIONS:
      return "MIPS_OPTIONS";
    case DT_MIPS_INTERFACE:
      return "MIPS_INTERFACE";
    case DT_MIPS_DYNSTR_ALIGN:
      return "MIPS_DYNSTR_ALIGN";
    case DT_MIPS_INTERFACE_SIZE:
      return "MIPS_INTERFACE_SIZE";
    case DT_MIPS_RLD_TEXT_RESOLVE_ADDR:
      return "MIPS_RLD_TEXT_RESOLVE_ADDR";
    case DT_MIPS_PERF_SUFFIX:
      return "MIPS_PERF_SUFFIX";
    case DT_MIPS_COMPACT_SIZE:
      return "MIPS_COMPACT_SIZE";
    case DT_MIPS_GP_VALUE:
      return "MIPS_GP_VALUE";
    case DT_MIPS_AUX_DYNAMIC:
      return "MIPS_AUX_DYNAMIC";
    case DT_MIPS_PLTGOT:
      return "MIPS_PLTGOT";
    case DT_MIPS_RWPLT:
      return "MIPS_RWPLT";
    case DT_MIPS_XHASH:
      return "MIPS_XHASH";
    default:
      return NULL;
    }
  return NULL;
}
