/*
 * ds.c
 *
 *  Created on: Jan 11, 2023
 *      Author: mmali
 */
#include <string.h>
#include "ds.h"
#include "wire.h"

#define DS_SCRATCHPAD_SIZE    9

#define DS_READ_ROM           0x33
#define DS_MATCH_ROM          0x55
#define DS_SKIP_ROM           0xCC

#define DS_CONVERT_T          0x44
#define DS_READ_SCRATCHPAD    0xBE

HAL_StatusTypeDef ds_init(void)
{
  return wire_init();
}

HAL_StatusTypeDef ds_read_address(uint8_t* rom_code)
{
  int i;
  uint8_t crc;

  if (wire_reset() != HAL_OK)
    return HAL_ERROR;

  wire_write(DS_READ_ROM);

  for (i = 0; i < DS_ROM_CODE_SIZE; i++)
    rom_code[i] = wire_read();

  crc = wire_crc(rom_code, DS_ROM_CODE_SIZE - 1);
  if (rom_code[DS_ROM_CODE_SIZE - 1] == crc)
    return HAL_OK;
  else
    return HAL_ERROR;
}

static HAL_StatusTypeDef send_cmd(const uint8_t* rom_code, uint8_t cmd)
{
  int i;

  if (wire_reset() != HAL_OK)
    return HAL_ERROR;

  if (!rom_code) {
    wire_write(DS_SKIP_ROM);
  } else {
    wire_write(DS_MATCH_ROM);
    for (i = 0; i < DS_ROM_CODE_SIZE; i++)
      wire_write(rom_code[i]);
  }
  wire_write(cmd);
  return HAL_OK;
}

HAL_StatusTypeDef ds_start_measure(const uint8_t* rom_code)
{
  return send_cmd(rom_code, DS_CONVERT_T);
}

static HAL_StatusTypeDef ds_read_scratchpad(const uint8_t* rom_code, uint8_t* scratchpad)
{
  int i;
  uint8_t crc;

  if (send_cmd(rom_code, DS_READ_SCRATCHPAD) != HAL_OK)
    return HAL_ERROR;

  for (i = 0; i < DS_SCRATCHPAD_SIZE; i++)
    scratchpad[i] = wire_read();

  crc = wire_crc(scratchpad, DS_SCRATCHPAD_SIZE - 1);
  if (scratchpad[DS_SCRATCHPAD_SIZE - 1] == crc)
    return HAL_OK;
  else
    return HAL_ERROR;
}

float ds_get_temp(const uint8_t* rom_code)
{
  uint8_t scratchpad[DS_SCRATCHPAD_SIZE];
  int16_t temp;

  if (ds_read_scratchpad(rom_code, scratchpad) != HAL_OK)
    return 85.0f;

  memcpy(&temp, &scratchpad[0], sizeof(temp));

  return temp / 16.0f;
}



