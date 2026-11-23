/*!
* @file rtc.h
* @brief Header for RTC class
* @authors John Doe
* @date 2026-11-09
* @version 1.0.0
*/
#pragma once
#include <Arduino.h>
#include "iarduino_RTC.h"

/*!
* @brief Class describes the main methods of Real Time Clock
* @details Supported RTC: DS1302, DS1307, DS3231, RX8025
*/
class RTC {
private:
    bool isInited = false;
    iarduino_RTC *rtc = nullptr;
public:
    /*!
    * @brief The main constructor
    */
    RTC();
    /*!
    * @brief Destructor
    */
    virtual ~RTC();
    /*!
    * @brief Is RTC successfully initialized
    * @return Initialization status (<b>TRUE</b> - RTC was successfully initialized, otherwise <b>FALSE</b>)
    */
    bool IsInited();
    /*!
    * @brief Set new time to RTC (Unix format)
    * @param[in] unixtime New datetime in Unix-format
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Set datetime error due to device error or uninitialized
    */
    uint8_t SetTime(uint32_t unixtime);
    /*!
    * @brief Get current time from RTC (Unix format)
    * @param[inout] unixtime Current datetime in Unix-format
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Get datetime error due to device error or uninitialized
    * @retval 2 Parameter is nil
    */
    uint8_t GetTime(uint32_t *unixtime);
};