/*!
* @file air.h
* @brief Header for air humidity and temperature sensor class
* @authors John Doe
* @date 2026-11-09
* @version 1.0.0
*/
#pragma once
#include <Arduino.h>
#include "iarduino_DHT.h"

/*!
* @brief Air humidity and temperature values structure
* @details Supported sensors: DHT-11, DHT-22
*/
struct AirData {
    /*!
    * @brief Air humidity
    */
    uint8_t Humidity = 0;
    /*!
    * @brief Air temperature
    */
    float Temperature = 0.0;
};

/*!
* @brief Air humidity and temperature sensor class
*/
class Air {
private:
    bool isInited = false;
    uint8_t pin;
    iarduino_DHT *air = nullptr;
    unsigned long last_op;
public:
    /*!
    * @brief Constructor
    * @param[in] pin MCU PIN number for air humidity and temperature sensor
    */
    Air(uint8_t pin);
    /*!
    * @brief Destructor
    */
    virtual ~Air();
    /*!
    * @brief Is air humidity and temperature sensor successfully initialized
    * @return <b>TRUE</b> Air humidity and temeprature sensor successfully initialized, otherwise <b>FALSE</b>
    */
    bool IsInited();
    /*!
    * @brief Get air humidity and temperature data from sensor
    * @param[inout] data Variable for save values (see #AirData)
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Sensor wasn't successfully initialized
    * @retval 2 Data variable is not initialized
    * @retval 3 Sensor rate limit error
    * @retval 4 Invalid data checksum
    * @retval 5 Invalid or corrupted data
    * @retval 6 No answer
    * @retval 7 Undefined sensor error
    */
    uint8_t GetData(AirData *data);
};