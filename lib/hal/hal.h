/*!
* @file hal.h
* @brief Header for HAL configuraion and constants
* @authors John Doe
* @date 2026-11-09
* @version 1.0.0
*/
#pragma once
#include <Arduino.h>

//-------------------- System devices
/*!
* @brief RTC device type
* @details The following values are supported: <b>RTC_DS1302</b>, <b>RTC_DS1307</b>, <b>RTC_DS3231</b>, <b>RTC_RX8025</b>.
*/
#define RTC_DEVICE RTC_RX8025

/*!
* @brief Storage write operation delay in milliseconds
*/
#define STORAGE_OPERATION_DELAY 5

/*!
* @brief EEPROM I2C device address
*/
#define STORAGE_ADDR 0x50

//----------------------- Sensors
/*!
* @brief Minimum humidity value of the soil moisture sensor
*/
#define SOIL_MOISTURE_MIN 630

/*!
* @brief Maximum humidity value of the soil moisture sensor
*/
#define SOIL_MOISTURE_MAX 330

/*!
* @brief Soil moisture sensor 1 data PIN
* @details Only analog MCU pin
* @details For disable this sensor remove this define
*/
#define SOIL_MOISTURE_PIN1 A0

/*!
* @brief Soil moisture sensor 2 data PIN
* @details Only analog MCU pin
* @details For disable this sensor remove this define
*/
#define SOIL_MOISTURE_PIN2 A1

/*!
* @brief Air humidity and temperature sensor startup delay in milliseconds
*/
#define AIR_SENSOR_START_DELAY 1000

/*!
* @brief Air humidity and temeprature sensor operation delay in milliseconds
*/
#define AIR_SENSOR_OPERATION_DELAY 2000

/*!
* @brief Air humidity and temperature sensor MCU PIN number
* @details Only digital MCU pin
* @details For disable this sensor remove this define
*/
#define AIR_SENSOR_PIN 11

//--------------------------- The executive mechanisms (devices)
/*!
* @brief Operation delay for electromagnetic relay in milliseconds
*/
#define RELAY_DELAY 50

/*!
* @brief Operation delay for transistor key in milliseconds
*/
#define KEY_DELAY 0

/*!
* @brief MCU PIN number for Bluetooth power control relay
* @details Only digital MCU pin
*/
#define BT_RELAY_PIN 6

/*!
* @brief Bluetooth power control relay type
* @details Possible relay types: 1 - relay, 2 - n-channel key, 3 - p-channel key
*/
#define BT_RELAY_TYPE 2

/*!
* @brief MCU PIN number for lamp relay
* @details Only digital MCU pin
* @details For disable this relay remove this define
*/
#define LAMP_RELAY_PIN 5

/*!
* @brief Lamp relay type
* @details Possible relay types: 1 - relay, 2 - n-channel key, 3 - p-channel key
*/
#define LAMP_RELAY_TYPE 1

/*!
* @brief MCU PIN number for pump relay
* @details Only digital MCU pin
* @details For disable this relay remove this define
*/
#define PUMP_RELAY_PIN 4

/*!
* @brief Pump relay type
* @details Possible relay types: 1 - relay, 2 - n-channel key, 3 - p-channel key
*/
#define PUMP_RELAY_TYPE 2

/*!
* @brief MCU PIN number for fan relay
* @details Only digital MCU pin
* @details For disable this relay remove this define
*/
#define FAN_RELAY_PIN 3

/*!
* @brief Fan relay type
* @details Possible relay types: 1 - relay, 2 - n-channel key, 3 - p-channel key
*/
#define FAN_RELAY_TYPE 1

/*!
* @brief MCU PIN number for air humidifier relay
* @details Only digital MCU pin
* @details For disable this relay remove this define
*/
#define HUMIDIFIER_RELAY_PIN 2

/*!
* @brief Air humidifier relay type
* @details Possible relay types: 1 - relay, 2 - n-channel key, 3 - p-channel key
*/
#define HUMIDIFIER_RELAY_TYPE 1

//--------------------------- Communication
/*!
* @brief Bluetooth RX line
* @details Only digital MCU pin
* @details This pin is RX for module and TX for Arduino board
* @attention The voltage on this pin should be no more than 3.3 V, otherwise the Bluetooth module will fail.
*/
#define BT_RX_PIN 10
/*!
* @brief Bluetooth TX line
* @details Only digital MCU pin
* @details This pin is TX for module and RX for Arduino board 
*/
#define BT_TX_PIN 9
/*!
* @brief Bluetooth MODE line (AT-commands or transport)
* @details Only digital MCU pin
* @attention The voltage on this pin should be no more than 3.3 V, otherwise the Bluetooth module will fail.
*/
#define BT_MODE_PIN 13
/*!
* @brief Bluetooth connection state line
* @details Only digital MCU pin
*/
#define BT_STATE_PIN 12

//--------------------------- Buttons
/*!
* @brief Bluetooth activation button
* @details Only digital MCU pin
* @details For disable this button remove this define
*/
#define BT_BUTTON_PIN 7
