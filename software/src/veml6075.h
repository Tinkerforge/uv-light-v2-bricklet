/* uv-light-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * veml6075.h: VEML6075 driver
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef VEML6075_H
#define VEML6075_H

#include <stdint.h>

#include "bricklib2/hal/i2c_fifo/i2c_fifo.h"

typedef struct {
	I2CFifo i2c_fifo;
} VEML6075_t;

extern VEML6075_t veml6075;

void veml6075_init(void);
void veml6075_tick(void);
uint32_t veml6075_get_uv_light(void);

#endif
