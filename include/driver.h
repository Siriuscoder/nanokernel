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

#ifndef DRIVER_H_
#define DRIVER_H_

#include <types.h>
#include <memory.h>

#define DECLARE_DRIVER(name, dr_name, dr_vendor) \
		const struct driverInfo_t name = { \
			dr_name, \
			dr_vendor, \
			driver_init_entry, \
			driver_shutdown_entry, \
			driver_start_entry, \
			driver_stop_entry, \
			{ \
					k_malloc, \
					k_free, \
					k_realloc \
			} \
		};

#define DECLARE_DRIVER_WITH_MEM_MANAGER(name, dr_name, dr_vendor, dr_mem) \
		const struct driverInfo_t name = { \
			dr_name, \
			dr_vendor, \
			init_driver_entry, \
			shutdown_driver_entry, \
			start_driver_entry, \
			dr_mem \
		};

typedef bool (*init_driver)(size_t, char **);
typedef bool (*shutdown_driver)(void);

typedef bool (*start_driver)(void);
typedef bool (*stop_driver)(void);

typedef struct driverInfo
{
	char driverName[50];
	char driverVendor[50];
	init_driver initDriver;
	shutdown_driver shutdownDriver;
	start_driver start;
	stop_driver stop;
	mallocInfo_t mallocInfo;
} driverInfo_t;

void drivers_start(size_t argc, char **args);

void drivers_stop();

#endif /* DRIVER_H_ */
