/*!
* @file soil.h
* @brief Header for soil moisture sensor class
* @authors John Doe
* @date 2026-11-09
* @version 1.0.0
*/
#pragma once
#include <Arduino.h>

/*!
* @brief Soil moisture sensor class
*/
class Soil {
private:
    uint8_t pin;
    bool isInited = false;
public:
    /*!
    * @brief Constructor
    * @param[in] pin MCU analog PIN number for reading data from soil moisture sensor class
    */
    Soil(uint8_t pin);
    /*!
    * @brief Is soil moisture sensor successfully initialized
    * @return <b>TRUE</b> Soil moisture sensor was successfully initialized, otherwise <b>FALSE</b>
    */
    bool IsInited();
    /*!
    * @brief Get soil moisture sensor value in percent
    * @return Soil moisture sensor current value in percent
    * @retval 0-100 Soil moisture in percent
    * @retval 254 Incorrect sensor value
    * @retval 255 Sensor wasn't successfully inititialized
    */
    uint8_t GetHumidity();
};