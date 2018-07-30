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

VEML6075_t veml6075;

void veml6075_init(void) {
	logd("[+] veml6075_init()\n\r");

	veml6075.uv_comp1_raw = 0;
	veml6075.uv_comp2_raw = 0;
	veml6075.uva_raw = 0;
	veml6075.uvb_raw = 0;
	veml6075.uva_calc = 0;
	veml6075.uvb_calc = 0;
	veml6075.uvi_calc = 0;

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
	veml6075.i2c_fifo_buf[0] = (VEML6075_CONF_MSK_DEFAULT | VEML6075_CONF_MSK_SD_PD);
	veml6075.i2c_fifo_buf[1] = 0;

	i2c_fifo_write_register(&veml6075.i2c_fifo,
	                        (uint8_t)VEML6075_ADDR_UV_CONF,
	                        2,
	                        &veml6075.i2c_fifo_buf[0],
	                        true);

	veml6075.sm = S_SHUTDOWN;
	veml6075.timer_duration_ms = 150;
	veml6075.timer_started_at = system_timer_get_ms();
}

void veml6075_tick(void) {
	uint32_t fifo_value = 0;

	I2CFifoState ifs = i2c_fifo_next_state(&veml6075.i2c_fifo);

	if(ifs & I2C_FIFO_STATE_ERROR) {
		loge("[+] veml6075_tick(): I2C HW Status = %d, I2C FIFO Status = %d\n\r",
		     (uint32_t)veml6075.i2c_fifo.i2c_status,
		     ifs);

		veml6075_init();

		return;
	}

	if((veml6075.sm == S_SHUTDOWN) || (veml6075.sm == S_POWER_UP)) {
		if(veml6075.sm == S_SHUTDOWN) {
			if(system_timer_is_time_elapsed_ms(veml6075.timer_started_at, veml6075.timer_duration_ms)) {
				// Configure the sensor for power up
				veml6075.i2c_fifo_buf[0] = (VEML6075_CONF_MSK_DEFAULT | VEML6075_CONF_MSK_SD_PU);
				veml6075.i2c_fifo_buf[1] = 0;

				// Flush FIFO
				XMC_USIC_CH_RXFIFO_Flush(veml6075.i2c_fifo.i2c);
				XMC_USIC_CH_TXFIFO_Flush(veml6075.i2c_fifo.i2c);

				i2c_fifo_write_register(&veml6075.i2c_fifo,
										(uint8_t)VEML6075_ADDR_UV_CONF,
										2,
										&veml6075.i2c_fifo_buf[0],
										true);

				veml6075.sm = S_POWER_UP;
				veml6075.timer_duration_ms = 150;
				veml6075.timer_started_at = system_timer_get_ms();
			}

			return;
		}
		else if (veml6075.sm == S_POWER_UP) {
			if(system_timer_is_time_elapsed_ms(veml6075.timer_started_at, veml6075.timer_duration_ms)) {
				veml6075.sm = S_GET_UV_TYPE_A;
				veml6075.i2c_fifo.state = I2C_FIFO_STATE_IDLE;
				veml6075.timer_duration_ms = 0;
				veml6075.timer_started_at = 0;
			}
			else {
				return;
			}
		}
	}

	if((veml6075.sm == S_GET_UV_TYPE_A) && (veml6075.i2c_fifo.state == I2C_FIFO_STATE_IDLE)) {
		// Flush FIFO
		XMC_USIC_CH_RXFIFO_Flush(veml6075.i2c_fifo.i2c);
		XMC_USIC_CH_TXFIFO_Flush(veml6075.i2c_fifo.i2c);

		i2c_fifo_read_register(&veml6075.i2c_fifo, (uint8_t)VEML6075_ADDR_UVA_DATA, 2);
	}
	else if(ifs == I2C_FIFO_STATE_READ_REGISTER_READY) {
		// Read data from FIFO
		i2c_fifo_read_fifo(&veml6075.i2c_fifo, veml6075.i2c_fifo_buf, 2);

		fifo_value = (uint32_t)(veml6075.i2c_fifo_buf[1] << 8) | veml6075.i2c_fifo_buf[0];

		if(veml6075.sm == S_GET_UV_TYPE_A) {
			veml6075.uva_raw = fifo_value;
			veml6075.sm = S_GET_UV_TYPE_B;

			// Flush FIFO
			XMC_USIC_CH_RXFIFO_Flush(veml6075.i2c_fifo.i2c);
			XMC_USIC_CH_TXFIFO_Flush(veml6075.i2c_fifo.i2c);

			i2c_fifo_read_register(&veml6075.i2c_fifo, (uint8_t)VEML6075_ADDR_UVB_DATA, 2);
		}
		else if(veml6075.sm == S_GET_UV_TYPE_B) {
			veml6075.uvb_raw = fifo_value;
			veml6075.sm = S_GET_UV_COMP_1;

			// Flush FIFO
			XMC_USIC_CH_RXFIFO_Flush(veml6075.i2c_fifo.i2c);
			XMC_USIC_CH_TXFIFO_Flush(veml6075.i2c_fifo.i2c);

			i2c_fifo_read_register(&veml6075.i2c_fifo, (uint8_t)VEML6075_ADDR_UVCOMP1_DATA, 2);
		}
		else if(veml6075.sm == S_GET_UV_COMP_1) {
			veml6075.uv_comp1_raw = fifo_value;
			veml6075.sm = S_GET_UV_COMP_2;

			// Flush FIFO
			XMC_USIC_CH_RXFIFO_Flush(veml6075.i2c_fifo.i2c);
			XMC_USIC_CH_TXFIFO_Flush(veml6075.i2c_fifo.i2c);

			i2c_fifo_read_register(&veml6075.i2c_fifo, (uint8_t)VEML6075_ADDR_UVCOMP2_DATA, 2);
		}
		else if(veml6075.sm == S_GET_UV_COMP_2) {
			veml6075.uv_comp2_raw = fifo_value;

			//uartbb_printf("uva %u, uvb %u, c1 %u, c2 %u\n\r",
			//              veml6075.uva_raw, veml6075.uvb_raw,
			//              veml6075.uv_comp1_raw, veml6075.uv_comp2_raw);

			// Calculate compensated UVA and UVB values. Check Eq. (1) and Eq. (2)
			// on page 6 of the application note. The gain calibration factors are
			// assumed to be 1. The result is in 1/100 counts
			uint32_t uva_counts = (100 * veml6075.uva_raw - VEML6075_COEF_A * veml6075.uv_comp1_raw - VEML6075_COEF_B * veml6075.uv_comp2_raw);
			uint32_t uvb_counts = (100 * veml6075.uvb_raw - VEML6075_COEF_C * veml6075.uv_comp1_raw - VEML6075_COEF_D * veml6075.uv_comp2_raw);

			// Convert raw values to µW/cm² (for integration time of 50ms).
			// Check the table on page 2.
			// UVA: typical 0.93 counts/µW/cm²
			// UVB: typical 2.10 counts/µW/cm²
			veml6075.uva_calc = (uva_counts * 10) / 93; // 1/10 µW/cm²
			veml6075.uvb_calc = (uvb_counts * 10) / 210; // 1/10 µW/cm²

			veml6075.uvi_calc = ((uva_counts * 10) / VEML6075_UVA_RESP_INV + (uvb_counts * 10) / VEML6075_UVB_RESP_INV) / 2; // 1/10 UVI

			veml6075.sm = S_GET_UV_TYPE_A;
			veml6075.i2c_fifo.state = I2C_FIFO_STATE_IDLE;
		}
	}
}

uint32_t veml6075_get_uva(void) {
	return veml6075.uva_calc;
}

uint32_t veml6075_get_uvb(void) {
	return veml6075.uvb_calc;
}

uint32_t veml6075_get_uvi(void) {
	return veml6075.uvi_calc;
}
