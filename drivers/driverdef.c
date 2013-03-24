/*  This file is part of simple kernel.
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

#include <driver.h>

const driverInfo_t drivers_definition[] = {
		{
				"Keyboard generic",
				"SiriusDrivers",
				NULL,
				NULL,
				NULL,
				NULL,
				{
						k_malloc,
						k_free,
						k_realloc
				}
		},
		{
				"\0",
				"\0",
				NULL,
				NULL,
				NULL,
				NULL,
				{
						NULL,
						NULL,
						NULL
				}
		}
};

