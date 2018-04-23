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

CallbackValue callback_value_uvb_light;
CallbackValue callback_value_uva_light;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_UVA_LIGHT: return get_callback_value(message, response, &callback_value_uva_light);
		case FID_SET_UVA_LIGHT_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration(message, &callback_value_uva_light);
		case FID_GET_UVA_LIGHT_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration(message, response, &callback_value_uva_light);
		case FID_GET_UVB_LIGHT: return get_callback_value(message, response, &callback_value_uvb_light);
		case FID_SET_UVB_LIGHT_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration(message, &callback_value_uvb_light);
		case FID_GET_UVB_LIGHT_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration(message, response, &callback_value_uvb_light);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

bool handle_uva_light_callback(void) {
	return handle_callback_value_callback(&callback_value_uva_light, FID_CALLBACK_UVA_LIGHT);
}

bool handle_uvb_light_callback(void) {
	return handle_callback_value_callback(&callback_value_uvb_light, FID_CALLBACK_UVB_LIGHT);
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	callback_value_init(&callback_value_uvb_light, veml6075_get_uvb_light);
	callback_value_init(&callback_value_uva_light, veml6075_get_uva_light);

	communication_callback_init();
}
