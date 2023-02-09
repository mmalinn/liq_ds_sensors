/*
 * ds.h
 *
 *  Created on: Jan 11, 2023
 *      Author: mmali
 */

#pragma once
#include "stm32f3xx.h"

#define DS_ROM_CODE_SIZE		8

HAL_StatusTypeDef ds_init(void);
HAL_StatusTypeDef ds_read_address(uint8_t* rom_code);
HAL_StatusTypeDef ds_start_measure(const uint8_t* rom_code);
float ds_get_temp(const uint8_t* rom_code);
