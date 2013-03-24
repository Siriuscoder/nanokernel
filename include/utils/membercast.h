/*
 Project NanoKernel (for study purposes only)
 Copyright (C) 2013  Sirius (Vdov Nikita)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MEMBERCAST_H_
#define MEMBERCAST_H_

#define OFFSETOF(tstruct, field)	( (size_t) &((tstruct*)0x0)->field )
#define MEMBERCAST(tstruct, pfield, field) ((tstruct*)((size_t)pfield - OFFSETOF(tstruct, field)))


#endif /* MEMBERCAST_H_ */
