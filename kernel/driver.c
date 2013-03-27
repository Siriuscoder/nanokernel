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

extern const driverInfo_t drivers_definition[];

void drivers_start(size_t argc, char **args)
{
	int i;

	for(i = 0; ; i++)
	{
		if(drivers_definition[i].driverName[0] == 0)
		{
			break;
		}

		if(drivers_definition[i].initDriver)
			drivers_definition[i].initDriver(argc, args);
		if(drivers_definition[i].start)
			drivers_definition[i].start();
	}
}

void drivers_stop()
{
	int i;

	for(i = 0; ; i++)
	{
		if(drivers_definition[i].driverName[0] == 0)
		{
			break;
		}

		if(drivers_definition[i].stop)
			drivers_definition[i].stop();
		if(drivers_definition[i].shutdownDriver)
			drivers_definition[i].shutdownDriver();
	}
}


