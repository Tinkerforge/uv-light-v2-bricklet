/* uv-light-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 * Copyright (C) 2018 Matthias Bolte <matthias@tinkerforge.com>
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

#define VEML6075_ADDR_UV_CONF 0x00
#define VEML6075_ADDR_UVA_DATA 0x07
#define VEML6075_ADDR_UVB_DATA 0x09
#define VEML6075_ADDR_UVCOMP1_DATA 0x0A
#define VEML6075_ADDR_UVCOMP2_DATA 0x0B

#define VEML6075_CONF_MSK_SD_PU 0x00
#define VEML6075_CONF_MSK_SD_PD 0x01
#define VEML6075_CONF_MSK_AF_AUTO 0x00
#define VEML6075_CONF_MSK_AF_FORCE 0x01
#define VEML6075_CONF_MSK_TRIG_NO 0x00
#define VEML6075_CONF_MSK_TRIG_ONCE 0x04
#define VEML6075_CONF_MSK_HD_NORMAL 0x00
#define VEML6075_CONF_MSK_HD_HIGH 0x08
#define VEML6075_CONF_MSK_IT_50MS 0x00
#define VEML6075_CONF_MSK_IT_100MS 0x10
#define VEML6075_CONF_MSK_IT_200MS 0x20
#define VEML6075_CONF_MSK_IT_400MS 0x30
#define VEML6075_CONF_MSK_IT_800MS 0x40

#define VEML6075_CONF_MSK_DEFAULT \
	(VEML6075_CONF_MSK_AF_AUTO | VEML6075_CONF_MSK_TRIG_NO | VEML6075_CONF_MSK_HD_NORMAL)

// coefficients and responsivity values taken from the table on application
// note page 10, first row for open-air. the responsivity values are given for
// 100ms integration time, multiply by 2 to get values for 50ms integration time
#define VEML6075_COEF_A 222 // 2.22 * 100
#define VEML6075_COEF_B 133 // 1.33 * 100
#define VEML6075_COEF_C 295 // 2.95 * 100
#define VEML6075_COEF_D 174 // 1.74 * 100
#define VEML6075_UVA_RESP_INV 34223 // 1 / (0.001461 * 2) * 100
#define VEML6075_UVB_RESP_INV 19298 // 1 / (0.002591 * 2) * 100

typedef enum {
	S_SHUTDOWN = 0,
	S_CONFIGURE,
	S_GET_UVA_WAIT,
	S_GET_UVA,
	S_GET_UVB,
	S_GET_UV_COMP_1,
	S_GET_UV_COMP_2
} SMVEML6075_t;

typedef struct {
	uint8_t it_msk_pending;
	uint32_t it_factor_pending;
	uint8_t it_msk;
	uint32_t it_factor;

	SMVEML6075_t sm;
	I2CFifo i2c_fifo;

	uint16_t uv_comp1_raw;
	uint16_t uv_comp2_raw;
	uint16_t uva_raw;
	uint16_t uvb_raw;
	int32_t uva_calc;
	int32_t uvb_calc;
	int32_t uvi_calc;

	uint32_t timer_started_at;
	uint32_t timer_duration_ms;
} VEML6075_t;

extern VEML6075_t veml6075;

void veml6075_init(void);
void veml6075_tick(void);

void veml6075_set_integration_time(uint8_t it_msk);
uint8_t veml6075_get_integration_time(void);

int32_t veml6075_get_uva(void);
int32_t veml6075_get_uvb(void);
int32_t veml6075_get_uvi(void);

#endif
