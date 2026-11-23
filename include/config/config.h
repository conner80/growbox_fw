/*!
* @file config.h
* @brief Firmware user configuration
* @authors John Doe
* @date 2026-11-10
* @version 1.0.0
*/
#pragma once

#include <Arduino.h>
#include "hal.h"
#include "version.h"

/*!
* @brief Bluetooth configuration
*/
struct BluetoothConfig {
    /*!
    * @brief Bluetooth slave device name
    */
    char Name[21] = {"Growbox\0"};
    /*!
    * @brief Bluetooth slave device password
    */
    uint16_t Password = 1234;
    /*!
    * @brief Online/Active mode delay in seconds in idle mode (no connection)
    * @details <b>0</b> - Bluetooth device always active
    */
    uint16_t OnlineDelay = 300;
};

/*!
* @brief Activity period configuration 
* @details If begin part of the prtiod equal end of the period or period has incorrect values - ignore period and always device active
*/
struct PeriodConfig {
    /*!
    * @brief Hour of the beginning activity
    */
    int StartH = 6;
    /*!
    * @brief Minute of the beginning activity
    */
    int StartM = 0;
    /*!
    * @brief Hour of the ending activity
    */
    int EndH = 0;
    /*!
    * @brief Minute of the ending activity
    */
    int EndM = 0;
};

/*!
* @brief Default relay configuration
*/
struct RelayConfig {
    /*!
    * @brief Automatic control or manual
    */
    bool IsAuto = true;
    /*!
    * @brief Relay activity period
    */
    PeriodConfig ActivityPeriod;
};

/*!
* @brief Pump configuration
*/
struct PumpConfig {
    /*!
    * @brief Automatic control or manual
    */
    bool IsAuto = true;
    /*!
    * @brief Maximum pump working time in seconds
    * @details <b>0</b> - Infinity time (stops by command only)
    */
    uint16_t MaxWorkTime = 300;
    /*!
    * @brief Pause between pump runs in seconds
    * @details <b>0</b> - No pause between pump starts
    */
    uint16_t PauseBetweenStart = 600;
    /*!
    * @brief Watering frequency in hours
    * @details <b>0</b> - Ignore this parameter otherwise maximum watering frequency in hours
    */
    uint8_t FreqWatering = 72;
    /*!
    * @brief Pump activity period
    */
    PeriodConfig ActivityPeriod;
};

/*!
* @brief Soil moisture sensor configuration
*/
struct SoilMoistureConfig {
    /*!
    * @brief The percentage of soil moisture at which watering begins 
    */
    uint8_t StartWatering = 30;
    /*!
    * @brief The percentage of soil moisture at which watering ends
    */
    uint8_t EndWatering = 60;
    /*!
    * @brief Minimal soil moisture (0%)
    * @details Calibrate or manual input value
    */
    uint16_t MinValue = SOIL_MOISTURE_MIN;
    /*!
    * @brief Maximal soil moisture (100%)
    * @details Calibrate or manual input value
    */
    uint16_t MaxValue = SOIL_MOISTURE_MAX;
};

/*!
* @brief Air parameters configuration for fan activate
*/
struct AirConfig {
    /*!
    * @brief The temperature of the beginning of ventilation (in Celsius)
    */
    float TempStartVent = 24.0;
    /*!
    * @brief The temperature of the finishing of ventilation (in Celsius)
    */
    float TempEndVent = 20.0;
    /*!
    * @brief The air humidity of the beginning of ventilation (in percent)
    */
    uint8_t HumStartVent = 70;
    /*!
    * @brief The air humidity of the finishing of ventilation (in percent)
    */
    uint8_t HumEndVent = 50;
    /*!
    * @brief The air humidity of the beginning of humidification (in percent)
    */
    uint8_t HumStartHum = 30;
    /*!
    * @brief The air humidity of the finishing of humidification (in percent)
    */
    uint8_t HumEndHum = 40;
};

/*!
* @brief User configuration structure
*/
struct GrowConfig {
    /*!
    * @brief User's configuration structure's version
    */
    uint16_t Version = CONFIG_VERSION;
    /*!
    * @brief Bluetooth configuration
    */
    BluetoothConfig Bluetooth;
    /*!
    * @brief Lamp activity period configuration
    */
    RelayConfig Lamp;
    /*!
    * @brief Pump configuration
    */
    PumpConfig Pump;
    /*!
    * @brief Fan activity period configuration
    */
    RelayConfig Fan;
    /*!
    * @brief Air humidifier activity period configuration
    */
    RelayConfig Humidifier;
    /*!
    * @brief Soil moisture sensor 1 configuration
    */
    SoilMoistureConfig SoilSensor1;
    /*!
    * @brief Soil moisture sensor 2 configuration
    */
    SoilMoistureConfig SoilSensor2;
    /*!
    * @brief Air sensor(s) configuration
    */
    AirConfig AirSensor;
};

/*!
* @brief The default device configuration
*/
constexpr GrowConfig DEFAULT_CONFIGURATION;

/*!
* @brief Check the specified period for full activity
* @param[in] period Specified period (time interval)
* @return <b>TRUE</b> Ignore current time and use other sensors for device activation, otherwise <b>FALSE</b>
*/
inline bool IsAlwaysActivity(PeriodConfig &period) {
    if (period.StartH < 0 || period.StartH > 23 || period.StartM < 0 || period.StartM > 59 || period.EndH < 0 || period.EndH > 23 || period.EndM < 0 || period.EndM > 59) {
        return true;
    }
    if (period.StartH == period.EndH && period.StartM == period.EndM) {
        return true;
    }
    return false;
}