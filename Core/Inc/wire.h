/*
 * wire.h
 *
 *  Created on: Jan 11, 2023
 *      Author: mmali
 */

#pragma once

#include "stm32f3xx.h"

HAL_StatusTypeDef wire_init(void);
HAL_StatusTypeDef wire_reset(void);
uint8_t wire_read(void);
void wire_write(uint8_t byte);
uint8_t wire_crc(const uint8_t* data, int len);
