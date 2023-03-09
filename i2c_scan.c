/**
 * Copyright 2023 Legytma Soluções Inteligentes LTDA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * i2c_scan.c
 *
 *  Created on: 17 de fev de 2023
 *      Author: Alex Manoel Ferreira Silva
 */

#include <stddef.h>
#include <stdint.h>
#include <strings.h>

#include "hal/i2c_types.h"

#include "driver/i2c.h"

#include "esp_err.h"

#include "log_utils.h"

#include "i2c_scan.h"

#define WRITE_BIT    I2C_MASTER_WRITE /*!< I2C master write */
#define ACK_CHECK_EN 0x1 /*!< I2C master will check ack from slave*/

#define CHECK_RET(x)           \
	{                          \
		esp_err_t _err_ = x;   \
                               \
		if (_err_ != ESP_OK) { \
			return _err_;      \
		}                      \
	}

LOG_TAG("i2c_scan");

esp_err_t i2c_scan(i2c_port_t i2c_port) {
	return i2c_scan_to_array(i2c_port, NULL, 0, NULL);
}

esp_err_t i2c_scan_to_array(i2c_port_t i2c_port, uint8_t address_buffer[],
							size_t buffer_size, size_t* address_count) {
	uint8_t address;
	size_t  buffer_size = 52 + 1;
	char    buffer[buffer_size];
	size_t  count = 0;

	LOGI("");
	LOGI("I2C Port: %d", i2c_port);
	LOGI("    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");

	for (int i = 0; i < 128; i += 16) {
		bzero(buffer, buffer_size);

		sprintf(buffer, "%02X: ", i);

		for (int j = 0; j < 16; j++) {
			address = i + j;

			i2c_cmd_handle_t cmd = i2c_cmd_link_create();

			CHECK_RET(i2c_master_start(cmd));
			CHECK_RET(i2c_master_write_byte(cmd, (address << 1) | WRITE_BIT,
											ACK_CHECK_EN));
			CHECK_RET(i2c_master_stop(cmd));

			esp_err_t ret =
				i2c_master_cmd_begin(i2c_port, cmd, pdMS_TO_TICKS(50));

			i2c_cmd_link_delete(cmd);

			if (ret == ESP_OK) {
				if (address_buffer != NULL) {
					address_buffer[count] = address;
				}

				count++;

				if (address_count != NULL) {
					*address_count = count;
				}

				sprintf(buffer + strlen(buffer), "%02X ", address);
			} else if (ret == ESP_ERR_TIMEOUT) {
				sprintf(buffer + strlen(buffer), "UU ");
			} else {
				sprintf(buffer + strlen(buffer), "-- ");
			}
		}

		LOGI("%s", buffer);
	}

	LOGI("");

	return ESP_OK;
}
