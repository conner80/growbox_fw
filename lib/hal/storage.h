/*!
* @file storage.h
* @brief Header for data storage class
* @authors John Doe
* @date 2026-11-09
* @version 1.0.0
*/
#pragma once
#include <Arduino.h>
#include <config/config.h>

/*!
* @brief Data storage class
* @details Supported chips: AT24C256
*/
class Storage {
private:
    bool isInited = false;
    uint16_t slotSize;
    uint16_t totalSlots;
    int curSlot;
    uint32_t lastWriteCount;
protected:
    /*!
    * @brief Leveling the slot size up
    * @param[in] sz Slot size with user's data block
    * @return Aligned slot
    */
    static uint16_t alignSize(uint16_t sz);
public:
    /*!
    * @brief Constructor
    */
    Storage();
    /*!
    * @brief Is storage successfully initialized
    * @return <b>TRUE</b> Storage successfully initialized otherwise <b>FALSE</b>
    */
    bool IsInited();
    /*!
    * @brief Read configuration data from EEPROM
    * @param[inout] dest Buffer for read configuration
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect data block
    * @retval 2 Incorrect data block length
    * @retval 3 Incorrect EEPROM memory address
    * @retval 4 Device wasn't initialized (no data found)
    */
   uint8_t Read(GrowConfig &dest);
    /*!
    * @brief Write configuration data to EEPROM
    * @param[in] addr The address of the memory location from which the data recording will start
    * @param[in] src Specified configuration for write
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect data block
    * @retval 2 Incorrect data block length
    * @retval 3 Incorrect EEPROM memory address
    * @retval 4 Data write error
    */
   uint8_t Write(const GrowConfig &src);
};