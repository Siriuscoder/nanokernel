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

/* Programing interrupt controler interface */

#ifndef PIC_HEADER
#define PIC_HEADER	1

#include "kernel.h"

int k_pic_init();

uint16_t k_pic_get_irq_mask();
void k_pic_set_irq_mask(uint16_t mask);

/* hardware disable PIC */
void k_pic_disable();

/* mask IRQ vectors */
void k_pic_disable_irq_line(byte irq);

void k_pic_enable_irq_line(byte irq);

/* Returns the combined value of the cascaded PICs irq request register */
uint16_t k_pic_get_irr(void);
 
/* Returns the combined value of the cascaded PICs in-service register */
uint16_t k_pic_get_isr(void);

/* send EOI (end of interrupt command) */
void k_pic_eoi(byte irq);

#endif
