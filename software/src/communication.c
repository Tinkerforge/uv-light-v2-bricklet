/* uv-light-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/utility/callback_value.h"

#include "veml6075.h"

CallbackValue callback_value_uv_type_a;
CallbackValue callback_value_uv_type_b;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_UV_TYPE_A: return get_callback_value(message, response, &callback_value_uv_type_a);
		case FID_SET_UV_TYPE_A_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration(message, &callback_value_uv_type_a);
		case FID_GET_UV_TYPE_A_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration(message, response, &callback_value_uv_type_a);
		case FID_GET_UV_TYPE_B: return get_callback_value(message, response, &callback_value_uv_type_b);
		case FID_SET_UV_TYPE_B_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration(message, &callback_value_uv_type_b);
		case FID_GET_UV_TYPE_B_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration(message, response, &callback_value_uv_type_b);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

bool handle_uv_type_a_callback(void) {
	return handle_callback_value_callback(&callback_value_uv_type_a, FID_CALLBACK_UV_TYPE_A);
}

bool handle_uv_type_b_callback(void) {
	return handle_callback_value_callback(&callback_value_uv_type_b, FID_CALLBACK_UV_TYPE_B);
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	// TODO: Add proper functions
	callback_value_init(&callback_value_uv_type_a, veml6075_get_uv_type_a);
	callback_value_init(&callback_value_uv_type_b, veml6075_get_uv_type_b);

	communication_callback_init();
}
