#! /bin/bash
# Copyright (C) 2024 CIP United Inc.
# This file is part of elfutils.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# elfutils is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

. $srcdir/test-subr.sh

tempfiles test-readelf-h.txt test-readelf-reloc.txt
testrun ${abs_top_builddir}/src/readelf -h ${abs_top_builddir}/src/strip.o > test-readelf-h.txt
machine=`cat test-readelf-h.txt | grep Machine`
class=`cat test-readelf-h.txt | grep Class`
endian=`cat test-readelf-h.txt | grep Data`
if [[ "$machine" == *MIPS* && "$class" == *ELF64 && "$endian" == *little* ]]; then
testrun ${abs_top_builddir}/src/readelf -r ${abs_top_builddir}/src/strip.o | head -n 12 | tail -n 10 > test-readelf-reloc.txt

testrun_compare cat test-readelf-reloc.txt  << EOF
  Offset              Type            Value               Addend Name
  0x0000000000000008  MIPS_GPREL16    000000000000000000      +0 .text
		      Type2: MIPS_SUB
		      Type3: MIPS_HI16
  0x0000000000000010  MIPS_GPREL16    000000000000000000      +0 .text
		      Type2: MIPS_SUB
		      Type3: MIPS_LO16
  0x0000000000000014  MIPS_CALL16     000000000000000000      +0 gelf_getehdr
		      Type2: MIPS_NONE
		      Type3: MIPS_NONE
EOF
fi

exit 0
