/* VByte.h
 * Copyright (C) 2012, Francisco Claude & Rodrigo Canovas & Miguel A. Martinez-Prieto
 * all rights reserved.
 *
 * This class implements the Variable Byte (VByte) Code.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 * Contacting the authors:
 *   Francisco Claude:  	fclaude@cs.uwaterloo.ca
 *   Rodrigo Canovas:  rcanovas@student.unimelb.edu.au
 *   Miguel A. Martinez-Prieto:	migumar2@infor.uva.es
 */


#include "VByte.h"

uint32_t VByte::encode(uint64_t c, uint8_t *r) {
	unsigned int i= 0;

	while (c>127) {
		r[i] = (unsigned char)(c&127);
		i++;
		c>>=7;
	}

	r[i] = (unsigned char)(c|0x80);
	i++;

	return i;
}
	
uint32_t VByte::decode(uint64_t *c, uint8_t *r) {
	*c = 0;
	int i = 0;
	int shift = 0;

  uint64_t byte = 0;
	while ( !(r[i] & 0x80) ) {
    byte = r[i] & 0x7F; // cast to 64 bits before shift.
		*c |= byte << shift;
		i++;
		shift+=7;
	}

  byte = r[i] & 0x7F; // cast to 64 bits before shift.
	*c |= byte << shift;
	i++;

	return i;
}

