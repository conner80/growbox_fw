/*!
* @file parameters.h
* @brief Header for the current device parameters structure
* @authors John Doe
* @date 2026-11-10
* @version 1.0.0
*/
#pragma once
#include "air.h"

/*!
* @brief Current device parameters structure
*/
struct Params {
    /*!
    * @brief Current date ant time from RTC
    */
    uint32_t Now = 0;
    /*!
    * @brief Air humidity and temperature data from sensor
    */
    AirData AirSensor;
    /*!
    * @brief Soil moisture sensor 1 data
    */
    uint8_t SoilSensor1 = 0;
    /*!
    * @brief Soil moisture sensor 2 data
    */
    uint8_t SoilSensor2 = 0;
    /*!
    * @brief The lamp device is active
    */
    bool LampActive = false;
    /*!
    * @brief The fun is active
    */
    bool FanActive = false;
    /*!
    * @brief Water pump is active
    */
    bool PumpActive = false;
    /*!
    * @brief Air humidifier is active
    */
    bool HumidifierActive = false;
};