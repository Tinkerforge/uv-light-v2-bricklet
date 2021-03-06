/* uv-light-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 * Copyright (C) 2018 Matthias Bolte <matthias@tinkerforge.com>
 *
 * veml6075.c: VEML6075 driver
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

#include <stdint.h>

#include "veml6075.h"

#include "bricklib2/logging/logging.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include "configs/config_veml6075.h"

// http://www.soda-pro.com/web-services/typical-years/typical-uv

VEML6075_t veml6075;

void veml6075_init_i2c(void) {
	veml6075.it_msk_pending = VEML6075_CONF_MSK_IT_400MS;
	veml6075.it_factor_pending = 8;
	veml6075.it_msk = VEML6075_CONF_MSK_IT_400MS;
	veml6075.it_factor = 8;

	veml6075.i2c_fifo.baudrate = VEML6075_I2C_BAUDRATE;
	veml6075.i2c_fifo.i2c = VEML6075_I2C;
	veml6075.i2c_fifo.scl_port = VEML6075_SCL_PORT;
	veml6075.i2c_fifo.scl_pin = VEML6075_SCL_PIN;
	veml6075.i2c_fifo.scl_mode = VEML6075_SCL_PIN_MODE;
	veml6075.i2c_fifo.scl_input = VEML6075_SCL_INPUT;
	veml6075.i2c_fifo.scl_source = VEML6075_SCL_SOURCE;
	veml6075.i2c_fifo.scl_fifo_size = VEML6075_SCL_FIFO_SIZE;
	veml6075.i2c_fifo.scl_fifo_pointer = VEML6075_SCL_FIFO_POINTER;

	veml6075.i2c_fifo.sda_port = VEML6075_SDA_PORT;
	veml6075.i2c_fifo.sda_pin = VEML6075_SDA_PIN;
	veml6075.i2c_fifo.sda_mode = VEML6075_SDA_PIN_MODE;
	veml6075.i2c_fifo.sda_input = VEML6075_SDA_INPUT;
	veml6075.i2c_fifo.sda_source = VEML6075_SDA_SOURCE;
	veml6075.i2c_fifo.sda_fifo_size = VEML6075_SDA_FIFO_SIZE;
	veml6075.i2c_fifo.sda_fifo_pointer = VEML6075_SDA_FIFO_POINTER;

	i2c_fifo_init(&veml6075.i2c_fifo);

	// Set slave address
	veml6075.i2c_fifo.address = VEML6075_I2C_ADDRESS;

	// Shutdown the sensor for init
	uint8_t data[2] = { VEML6075_CONF_MSK_DEFAULT | VEML6075_CONF_MSK_SD_PD | veml6075.it_msk, 0 };

	i2c_fifo_write_register(&veml6075.i2c_fifo, VEML6075_ADDR_UV_CONF, 2, data, true);

	veml6075.sm = S_CONFIGURE;
	veml6075.timer_duration_ms = 10;
	veml6075.timer_started_at = system_timer_get_ms();
}

void veml6075_init(void) {
	logd("[+] veml6075_init()\n\r");

	memset(&veml6075, 0, sizeof(VEML6075_t));
	veml6075_init_i2c();
}

void veml6075_tick(void) {
	I2CFifoState ifs = i2c_fifo_next_state(&veml6075.i2c_fifo);

	if(ifs & I2C_FIFO_STATE_ERROR) {
		loge("[+] veml6075_tick(): I2C HW Status = %d, I2C FIFO Status = %d\n\r",
		     (uint32_t)veml6075.i2c_fifo.i2c_status,
		     ifs);

		veml6075_init_i2c();

		return;
	}

	if(ifs == I2C_FIFO_STATE_WRITE_REGISTER_READY) {
		switch(veml6075.sm) {
			case S_CONFIGURE: {
				if(system_timer_is_time_elapsed_ms(veml6075.timer_started_at, veml6075.timer_duration_ms)) {
					// Configure the sensor for power up
					uint8_t data[2] = { VEML6075_CONF_MSK_DEFAULT | VEML6075_CONF_MSK_SD_PU | veml6075.it_msk_pending, 0 };

					i2c_fifo_write_register(&veml6075.i2c_fifo, VEML6075_ADDR_UV_CONF, 2, data, true);

					veml6075.sm = S_GET_UVA_WAIT;

					// wait for 2.5x old integration time plus 2.5x new integration times
					// to ensure that the next reading was taken with the new integration
					// time. it's unclear why it has to be 2.5x of both integration times.
					// actually 1x integration time of both should be enough, but tests
					// show that it is not enough
					veml6075.timer_duration_ms = 50 * (((veml6075.it_factor + veml6075.it_factor_pending) * 10) / 4) + 10;
					veml6075.timer_started_at = system_timer_get_ms();

					veml6075.it_msk = veml6075.it_msk_pending;
					veml6075.it_factor = veml6075.it_factor_pending;
				}
				break;
			}

			case S_GET_UVA_WAIT: {
				if (system_timer_is_time_elapsed_ms(veml6075.timer_started_at, veml6075.timer_duration_ms)) {

					veml6075.sm = S_GET_UVA;
					i2c_fifo_read_register(&veml6075.i2c_fifo, VEML6075_ADDR_UVA_DATA, 2);
				}
				break;
			}

			default: {
				logw("Unexpected write state: %d\n\r", veml6075.sm);
				veml6075_init_i2c();
				break;
			}
		}
	} else if(ifs == I2C_FIFO_STATE_READ_REGISTER_READY) {
		// Read data from FIFO
		uint8_t data[2];

		i2c_fifo_read_fifo(&veml6075.i2c_fifo, data, 2);

		uint16_t value = (uint16_t)(data[1] << 8) | data[0];

		switch(veml6075.sm) {
			case S_GET_UVA: {
				veml6075.uva_raw = value;
				veml6075.sm = S_GET_UVB;

				i2c_fifo_read_register(&veml6075.i2c_fifo, VEML6075_ADDR_UVB_DATA, 2);
				break;
			}

			case S_GET_UVB: {
				veml6075.uvb_raw = value;
				veml6075.sm = S_GET_UV_COMP_1;

				i2c_fifo_read_register(&veml6075.i2c_fifo, VEML6075_ADDR_UVCOMP1_DATA, 2);
				break;
			}

			case S_GET_UV_COMP_1: {
				veml6075.uv_comp1_raw = value;
				veml6075.sm = S_GET_UV_COMP_2;

				i2c_fifo_read_register(&veml6075.i2c_fifo, VEML6075_ADDR_UVCOMP2_DATA, 2);
				break;
			}

			case S_GET_UV_COMP_2: {
				veml6075.uv_comp2_raw = value;

				if (veml6075.uva_raw == UINT16_MAX || veml6075.uvb_raw == UINT16_MAX ||
					veml6075.uv_comp1_raw == UINT16_MAX || veml6075.uv_comp2_raw == UINT16_MAX) {
					// sensor is saturated
					veml6075.uva_calc = -1;
					veml6075.uvb_calc = -1;
					veml6075.uvi_calc = -1;
				}
				else {
					// Calculate compensated UVA and UVB values. Check Eq. (1) and Eq. (2)
					// on page 6 of the application note. The gain calibration factors are
					// assumed to be 1. The result is in 1/100 counts
					int32_t uva_counts = 100 * (int32_t)veml6075.uva_raw - VEML6075_COEF_A * (int32_t)veml6075.uv_comp1_raw - VEML6075_COEF_B * (int32_t)veml6075.uv_comp2_raw;
					int32_t uvb_counts = 100 * (int32_t)veml6075.uvb_raw - VEML6075_COEF_C * (int32_t)veml6075.uv_comp1_raw - VEML6075_COEF_D * (int32_t)veml6075.uv_comp2_raw;

					// check for underflow. counts can get negative in low UV situations
					if (uva_counts < 0) {
						uva_counts = 0;
					}

					if (uvb_counts < 0) {
						uvb_counts = 0;
					}

					//uartbb_printf("uva %u, uvb %u, c1 %u, c2 %u, Ca %d, Cb %d\n\r", veml6075.uva_raw, veml6075.uvb_raw, veml6075.uv_comp1_raw, veml6075.uv_comp2_raw, uva_counts, uvb_counts);

					// Convert raw values to µW/cm² (for integration time of 50ms).
					// Check the table on page 2.
					// UVA: typical 0.93 counts/µW/cm²
					// UVB: typical 2.10 counts/µW/cm²
					veml6075.uva_calc = (uva_counts * 100) / (93 * veml6075.it_factor); // 1/100 µW/cm² = 1/10 mW/m²
					veml6075.uvb_calc = (uvb_counts * 100) / (210 * veml6075.it_factor); // 1/100 µW/cm² = 1/10 mW/m²

					// Calculate UVI value. Check Eq. (3, 4) and Eq. (5)
					// on page 6 of the application note.
					veml6075.uvi_calc = ((uva_counts * 10) / VEML6075_UVA_RESP_INV + (uvb_counts * 10) / VEML6075_UVB_RESP_INV) / (2 * veml6075.it_factor); // 1/10 UVI
				}

				veml6075.i2c_fifo.state = I2C_FIFO_STATE_WRITE_REGISTER_READY;

				if (veml6075.it_msk_pending != veml6075.it_msk) {
					veml6075.sm = S_CONFIGURE;
					veml6075.timer_duration_ms = 0;
					veml6075.timer_started_at = system_timer_get_ms();
				}
				else {
					veml6075.sm = S_GET_UVA_WAIT;
					veml6075.timer_duration_ms = 10;
					veml6075.timer_started_at = system_timer_get_ms();
				}
				break;
			}

			default: {
				logw("Unexpected read state: %d\n\r", veml6075.sm);
				veml6075_init_i2c();
				break;
			}
		}
	}
}

void veml6075_set_integration_time(uint8_t it_msk) {
	veml6075.it_msk_pending = it_msk;

	switch (it_msk) {
	case VEML6075_CONF_MSK_IT_50MS:  veml6075.it_factor_pending = 1;  break;
	case VEML6075_CONF_MSK_IT_100MS: veml6075.it_factor_pending = 2;  break;
	case VEML6075_CONF_MSK_IT_200MS: veml6075.it_factor_pending = 4;  break;
	case VEML6075_CONF_MSK_IT_400MS: veml6075.it_factor_pending = 8;  break;
	default:
	case VEML6075_CONF_MSK_IT_800MS: veml6075.it_factor_pending = 16; break;
	}
}

uint8_t veml6075_get_integration_time(void) {
	return veml6075.it_msk_pending;
}

int32_t veml6075_get_uva(void) {
	return veml6075.uva_calc;
}

int32_t veml6075_get_uvb(void) {
	return veml6075.uvb_calc;
}

int32_t veml6075_get_uvi(void) {
	return veml6075.uvi_calc;
}
