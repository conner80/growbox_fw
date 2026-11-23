/*!
* @file at24c256.h
* @brief Implementation for EEPROM AT24C256 (32KB)
* @authors John Doe
* @date 2026-11-10
* @version 1.0.0
*/
#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "../lib/hal/hal.h"

/*!
* @brief EEPROM memory page size in bytes
*/
#define PAGE_SIZE 64

/*!
* @brief EEPROM start address
*/
#define START_ADDRESS 0x0000

/*!
* @brief EEPROM end address (size)
*/
#define END_ADDRESS 0x8000

/*!
* @brief Memory initialization flag
*/
#define SLOT_ACTIVE 0xA9

/*!
* @brief Memory uninitialization 
* @details EEPROM default value 0xFF (empty byte) 
*/
#define SLOT_EMPTY 0xFF

/*!
* @brief Read data block from EEPROM
* @param[in] addr The address of the memory location from which the data reading will start
* @param[inout] dest Memory buffer for data read
* @param[in] len Reading block size in bytes
* @return Operation result code
* @retval 0 Success
* @retval 1 Incorrect data block
* @retval 2 Incorrect data block length
* @retval 3 Incorrect EEPROM memory address
*/
uint8_t EepromRead(uint16_t addr, uint8_t *dest, uint16_t len) {
    if (dest == nullptr) {
        return 1;
    }
    uint16_t max_len = END_ADDRESS - addr;
    if (len < 1 || len > max_len) {
        return 2;
    }
    if ((addr + len) > END_ADDRESS) {
        return 3;
    }
    while (len > 0) {
        uint16_t chunk = (len > 32) ? 32 : len;
        Wire.beginTransmission(STORAGE_ADDR);
        Wire.write((uint8_t)(addr >> 8));
        Wire.write((uint8_t)(addr & 0xFF));
        Wire.endTransmission();

        Wire.requestFrom((uint8_t)STORAGE_ADDR, (uint8_t)chunk);
        for (uint16_t i = 0; i < chunk && Wire.available(); i++) {
            dest[i] = Wire.read();
        }

        addr += chunk;
        dest += chunk;
        len -= chunk;
    }

    return 0;
}

/*!
* @brief Write specified data block to EEPROM
* @param[in] addr The address of the memory location from which the data recording will start
* @param[in] src Data block for write
* @param[in] len Data block length in bytes
* @return Operation result code
* @retval 0 Success
* @retval 1 Incorrect data block
* @retval 2 Incorrect data block length
* @retval 3 Incorrect EEPROM memory address
*/
uint8_t EepromWrite(uint16_t addr, const uint8_t *src, uint16_t len) {
    if (src == nullptr) {
        return 1;
    }
    uint16_t max_len = END_ADDRESS - addr;
    if (len < 1 || len > max_len) {
        return 2;
    }
    if ((addr + len) > END_ADDRESS) {
        return 3;
    }
    while (len > 0) {
        uint16_t availBytesInPage = PAGE_SIZE - (addr % PAGE_SIZE);
        uint16_t maxWrite = 30;
        uint16_t chunk = len;
        if (chunk > availBytesInPage) {
            chunk = availBytesInPage;
        }
        if (chunk > maxWrite) {
            chunk = maxWrite;
        }

        Wire.beginTransmission(STORAGE_ADDR);
        Wire.write((uint8_t)(addr >> 8));
        Wire.write((uint8_t)(addr & 0xFF));
        for (uint16_t i = 0; i < chunk; i++) {
            Wire.write(src[i]);
        }
        Wire.endTransmission();
        delay(STORAGE_OPERATION_DELAY);

        addr += chunk;
        src += chunk;
        len -= chunk;
    }

    return 0;
} 

/*!
* @brief Read specified data from EEPROM
* @param[in] addr The address of the memory location from which the data reading will start
* @param[inout] data Buffer for read
* @return Operation result code
* @retval 0 Success
* @retval 1 Incorrect data block
* @retval 2 Incorrect data block length
* @retval 3 Incorrect EEPROM memory address
*/
template <typename T>
uint8_t EepromGet(uint16_t addr, T &data) {
    return EepromRead(addr, (uint8_t*)&data, sizeof(T));
}

/*!
* @brief ReadWrite specified data to EEPROM
* @param[in] addr The address of the memory location from which the data writting will start
* @param[in] data Buffer for write
* @return Operation result code
* @retval 0 Success
* @retval 1 Incorrect data block
* @retval 2 Incorrect data block length
* @retval 3 Incorrect EEPROM memory address
*/
template <typename T>
uint8_t EepromPut(uint16_t addr, const T &data) {
    return EepromWrite(addr, (const uint8_t*)&data, sizeof(T));
}