/* Object attribute tags for MIPS.
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

#include <string.h>
#include <dwarf.h>

#define BACKEND mips_
#include "libebl_CPU.h"

#define KNOWN_VALUES(...) do				\
  {							\
    static const char *table[] = { __VA_ARGS__ };	\
    if (value < sizeof table / sizeof table[0])		\
      *value_name = table[value];			\
  } while (0)

//copy gnu attr tags from binutils-2.34/elfcpp/mips.h
/* Object attribute tags.  */
enum
{
  /* 0-3 are generic.  */

  /* Floating-point ABI used by this object file.  */
  Tag_GNU_MIPS_ABI_FP = 4,

  /* MSA ABI used by this object file.  */
  Tag_GNU_MIPS_ABI_MSA = 8,
};

/* Object attribute values.  */
enum
{
  /* Values defined for Tag_GNU_MIPS_ABI_MSA.  */

  /* Not tagged or not using any ABIs affected by the differences.  */
  Val_GNU_MIPS_ABI_MSA_ANY = 0,

  /* Using 128-bit MSA.  */
  Val_GNU_MIPS_ABI_MSA_128 = 1,
};

/* Object attribute values.  */
enum
{
  /* This is reserved for backward-compatibility with an earlier
     implementation of the MIPS NaN2008 functionality.  */
  Val_GNU_MIPS_ABI_FP_NAN2008 = 8,
};

/* copy binutils-2.34/binutils/readelf.c display_mips_gnu_attribute */
bool
mips_check_object_attribute (Ebl *ebl __attribute__ ((unused)),
			    const char *vendor, int tag, uint64_t value,
			    const char **tag_name, const char **value_name)
{
  if (!strcmp (vendor, "gnu"))
    switch (tag)
      {
      case Tag_GNU_MIPS_ABI_FP:
	*tag_name = "Tag_GNU_MIPS_ABI_FP";
	switch (value)
	{
	  case Val_GNU_MIPS_ABI_FP_ANY:
	    *value_name = "Hard or soft float";
	    return true;
	  case Val_GNU_MIPS_ABI_FP_DOUBLE:
	    *value_name = "Hard float (double precision)";
	    return true;
	  case Val_GNU_MIPS_ABI_FP_SINGLE:
	    *value_name = "Hard float (single precision)";
	    return true;
	  case Val_GNU_MIPS_ABI_FP_SOFT:
	    *value_name = "Soft float";
	    return true;
	  case Val_GNU_MIPS_ABI_FP_OLD_64:
	    *value_name = "Hard float (MIPS32r2 64-bit FPU 12 callee-saved)";
	    return true;
	  case Val_GNU_MIPS_ABI_FP_XX:
	    *value_name = "Hard float (32-bit CPU, Any FPU)";
	    return true;
	  case Val_GNU_MIPS_ABI_FP_64:
	    *value_name = "Hard float (32-bit CPU, 64-bit FPU)";
	    return true;
	  case Val_GNU_MIPS_ABI_FP_64A:
	    *value_name = "Hard float compat (32-bit CPU, 64-bit FPU)";
	    return true;
	  case Val_GNU_MIPS_ABI_FP_NAN2008:
	    *value_name = "NaN 2008 compatibility";
	    return true;
	  default:
	    return true;
	}
	return true;
      case Tag_GNU_MIPS_ABI_MSA:
	*tag_name = "Tag_GNU_MIPS_ABI_MSA";
	switch (value)
	{
	  case Val_GNU_MIPS_ABI_MSA_ANY:
	    *value_name = "Any MSA or not";
	    return true;
	  case Val_GNU_MIPS_ABI_MSA_128:
	    *value_name = "128-bit MSA";
	    return true;
	  default:
	    return true;
	}
	return true;
      }

  return false;
}
