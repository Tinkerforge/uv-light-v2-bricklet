/* uv-light-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 * Copyright (C) 2018 Matthias Bolte <matthias@tinkerforge.com>
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

CallbackValue_uint32_t callback_value_uva;
CallbackValue_uint32_t callback_value_uvb;
CallbackValue_uint32_t callback_value_uvi;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_UVA: return get_callback_value_uint32_t(message, response, &callback_value_uva);
		case FID_SET_UVA_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_uint32_t(message, &callback_value_uva);
		case FID_GET_UVA_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_uint32_t(message, response, &callback_value_uva);
		case FID_GET_UVB: return get_callback_value_uint32_t(message, response, &callback_value_uvb);
		case FID_SET_UVB_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_uint32_t(message, &callback_value_uvb);
		case FID_GET_UVB_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_uint32_t(message, response, &callback_value_uvb);
		case FID_GET_UVI: return get_callback_value_uint32_t(message, response, &callback_value_uvi);
		case FID_SET_UVI_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_uint32_t(message, &callback_value_uvi);
		case FID_GET_UVI_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_uint32_t(message, response, &callback_value_uvi);
		case FID_SET_CONFIGURATION: return set_configuration(message);
		case FID_GET_CONFIGURATION: return get_configuration(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse set_configuration(const SetConfiguration *data) {
	if (data->integration_time > UV_LIGHT_V2_INTEGRATION_TIME_800MS) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	veml6075_set_integration_time(data->integration_time << 4);

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_configuration(const GetConfiguration *data, GetConfiguration_Response *response) {
	response->header.length = sizeof(GetConfiguration_Response);
	response->integration_time = veml6075_get_integration_time() >> 4;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_uva_callback(void) {
	return handle_callback_value_callback_uint32_t(&callback_value_uva, FID_CALLBACK_UVA);
}

bool handle_uvb_callback(void) {
	return handle_callback_value_callback_uint32_t(&callback_value_uvb, FID_CALLBACK_UVB);
}

bool handle_uvi_callback(void) {
	return handle_callback_value_callback_uint32_t(&callback_value_uvi, FID_CALLBACK_UVI);
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	callback_value_init_uint32_t(&callback_value_uva, veml6075_get_uva);
	callback_value_init_uint32_t(&callback_value_uvb, veml6075_get_uvb);
	callback_value_init_uint32_t(&callback_value_uvi, veml6075_get_uvi);

	communication_callback_init();
}
