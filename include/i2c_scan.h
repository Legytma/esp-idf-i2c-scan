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
 * i2c_scan.h
 *
 *  Created on: 17 de fev de 2023
 *      Author: Alex Manoel Ferreira Silva
 */

#pragma once

#include "hal/i2c_types.h"

#include "esp_err.h"

esp_err_t i2c_scan(i2c_port_t i2c_port);
