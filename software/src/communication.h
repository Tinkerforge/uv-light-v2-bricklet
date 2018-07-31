/* uv-light-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 * Copyright (C) 2018 Matthias Bolte <matthias@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define UV_LIGHT_V2_THRESHOLD_OPTION_OFF 'x'
#define UV_LIGHT_V2_THRESHOLD_OPTION_OUTSIDE 'o'
#define UV_LIGHT_V2_THRESHOLD_OPTION_INSIDE 'i'
#define UV_LIGHT_V2_THRESHOLD_OPTION_SMALLER '<'
#define UV_LIGHT_V2_THRESHOLD_OPTION_GREATER '>'

#define UV_LIGHT_V2_INTEGRATION_TIME_50MS 0
#define UV_LIGHT_V2_INTEGRATION_TIME_100MS 1
#define UV_LIGHT_V2_INTEGRATION_TIME_200MS 2
#define UV_LIGHT_V2_INTEGRATION_TIME_400MS 3
#define UV_LIGHT_V2_INTEGRATION_TIME_800MS 4

#define UV_LIGHT_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define UV_LIGHT_V2_BOOTLOADER_MODE_FIRMWARE 1
#define UV_LIGHT_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define UV_LIGHT_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define UV_LIGHT_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define UV_LIGHT_V2_BOOTLOADER_STATUS_OK 0
#define UV_LIGHT_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define UV_LIGHT_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define UV_LIGHT_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define UV_LIGHT_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define UV_LIGHT_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define UV_LIGHT_V2_STATUS_LED_CONFIG_OFF 0
#define UV_LIGHT_V2_STATUS_LED_CONFIG_ON 1
#define UV_LIGHT_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define UV_LIGHT_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_GET_UVA 1
#define FID_SET_UVA_CALLBACK_CONFIGURATION 2
#define FID_GET_UVA_CALLBACK_CONFIGURATION 3
#define FID_GET_UVB 5
#define FID_SET_UVB_CALLBACK_CONFIGURATION 6
#define FID_GET_UVB_CALLBACK_CONFIGURATION 7
#define FID_GET_UVI 9
#define FID_SET_UVI_CALLBACK_CONFIGURATION 10
#define FID_GET_UVI_CALLBACK_CONFIGURATION 11
#define FID_SET_CONFIGURATION 13
#define FID_GET_CONFIGURATION 14

#define FID_CALLBACK_UVA 4
#define FID_CALLBACK_UVB 8
#define FID_CALLBACK_UVI 12

typedef struct {
	TFPMessageHeader header;
	uint8_t integration_time;
} __attribute__((__packed__)) SetConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t integration_time;
} __attribute__((__packed__)) GetConfiguration_Response;


// Function prototypes
BootloaderHandleMessageResponse set_configuration(const SetConfiguration *data);
BootloaderHandleMessageResponse get_configuration(const GetConfiguration *data, GetConfiguration_Response *response);

// Callbacks
bool handle_uva_callback(void);
bool handle_uvb_callback(void);
bool handle_uvi_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 3
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_uva_callback, \
	handle_uvb_callback, \
	handle_uvi_callback, \

#endif
