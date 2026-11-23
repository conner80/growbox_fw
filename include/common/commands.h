/*!
* @file commands.h
* @brief Header for network commands
* @authors John Doe
* @date 2026-11-18
* @version 1.0.0
*/
#pragma once

#include <Arduino.h>

/*!
* @brief Signature for request packets
*/
#define REQUEST_PACKET 0xFE
/*!
* @brief Signature for response packets
*/
#define RESPONSE_PACKET 0xEF
/*!
* @brief Positive acknowlage packet
*/
#define ACK_PACKET 0xAA
/*!
* @brief Negative acknowlage packet
*/
#define NACK_PACKET 0xBB

/*!
* @brief Request common device information
*/
#define CMD_INFO 0x01
/*!
* @brief Get common bluetooth device information
*/
#define CMD_GET_BT_INFO 0x02
/*!
* @brief Set bluetooth device name
*/
#define CMD_SET_BT_NAME 0x03
/*!
* @brief Set new bluetooth device password
*/
#define CMD_SET_BT_PASS 0x04
/*!
* @brief Get device current date and time (Unix-time format)
*/
#define CMD_GET_TIME 0x05
/*!
* @brief Set device new date and time (Unix-time format)
*/
#define CMD_SET_TIME 0x06
/*!
* @brief Get device configuration and status for all sensors and relays
*/
#define CMD_GET_STATUS 0x07
/*!
* @brief Set device configuration and status for all sensors and relays
*/
#define CMD_SET_STATUS 0x08
/*!
* @brief Get current mode of the device
*/
#define CMD_GET_MODE 0x09
/*!
* @brief Set mode for the device
*/
#define CMD_SET_MODE 0x0A

/*!
* @brief Network command's header
*/
struct __attribute__((packed)) PacketHeader {
    /*!
    * @brief Packet's signature
    */
    uint8_t Sign;
    /*!
    * @brief Command type
    */
    uint8_t Cmd;            
    /*!
    * @brief Command's data length
    */
    uint8_t Len = 0;        
};

/*!
* @brief Get common info command's response (see #CMD_INFO)
*/
struct __attribute__((packed)) CmdInfoReponse {
    /*!
    * @brief Device and firmware name
    */
    char Name[21] = {0x00};
    /*!
    * @brief Device firmware version
    */
    char FWVer[11] = {0x00};
    /*!
    * @brief Bluetooth device firmware version
    */
    char BTVer[21] = {0x00};
    /*!
    * @brief Enabled devices mask
    */
    uint16_t Devices = 0x0000;
    /*!
    * @brief Current device mode
    */
    uint8_t Mode = 0x00;
};

/*!
* @brief Get Bluetooth device common information command's response (see #CMD_GET_BT_INFO)
*/
struct __attribute__((packed)) CmdBTInfoResponse {
    /*!
    * @brief Bluetooth device current name
    */
    char Name[33] = {0x00};
    /*!
    * @brief Bluetooth device address
    */
    char Addr[15] = {0x00};
};

/*!
* @brief Set Bluetooth device name command's request (see #CMD_SET_BT_NAME)
*/
struct __attribute__((packed)) CmdBTSetNameRequest {
    /*!
    * @brief Bluetooth device name
    */
    char Name[33] = {0x00};
};

/*!
* @brief Common command's response
*/
struct __attribute__((packed)) CmdResponse {
    /*!
    * @brief Response code
    * @details Success is 0x00
    */
    uint8_t Code;
};

/*!
* @brief Set Bluetooth device password command's request (see #CMD_SET_BT_PASS)
*/
struct __attribute__((packed)) CmdBTSetPassRequest {
    /*!
    * @brief Bluetooth device password
    */
    char Pass[5] = {0x00};
};

/*!
* @brief Get device current date and time command's response (see #CMD_GET_TIME)
*/
struct __attribute__((packed)) CmdGetTimeResponse {
    /*!
    * @brief Response code
    * @details Success is 0x00
    */
    uint8_t Code;
    /*!
    * @brief Current device date and time (Unix-time format)
    */
    uint32_t Time;
};

/*!
* @brief Set device date and time command's request (see #CMD_SET_TIME)
*/
struct __attribute__((packed)) CmdSetTimeRequest {
    /*!
    * @brief Device date and time (Unix-time format)
    */
    uint32_t Time;
};

/*!
* @brief Get device current status command's request (see #CMD_GET_STATUS)
*/
struct __attribute__((packed)) CmdGetStatusRequest {
    /*!
    * @brief Device modules mask
    */
    uint16_t Mask;
};

/*!
* @brief Set device status command's request (see #CMD_SET_STATUS)
*/
struct __attribute__((packed)) CmdSetStatusRequest {
    /*!
    * @brief Save changes to EEPROM or nor
    */
    uint8_t SaveChanges;
};

/*!
* @brief Set device status command's response (see #CMD_SET_STATUS)
*/
struct __attribute__((packed)) CmdSetStatusResponse {
    /*!
    * @brief Response code
    * @details Success is 0x00
    */
    uint8_t Code;
    /*!
    * @brief Fault modules/devices mask
    */
    uint16_t FaultMask;
};

/*!
* @brief Get current or set new device mode command's response/request (see #CMD_GET_MODE and #CMD_SET_MODE)
*/
struct __attribute__((packed)) CmdMode {
    /*!
    * @brief Current device mode
    * @details <b>0x00</b> - Callibrate mode<br><b>0x01</b> - Standart mode
    */
    uint8_t Mode;
};

/*!
* @brief Air humidity and temperature sensor information and configuration
*/
struct __attribute__((packed)) AirSensorInfo {
    /*!
    * @brief Device identificator value
    */
    uint16_t DeviceID = 0x0004;
    /*!
    * @brief The temperature of the beginning of ventilation (in Celsius)
    */
    float TempStartVent;
    /*!
    * @brief The temperature of the finishing of ventilation (in Celsius)
    */
    float TempEndVent;
    /*!
    * @brief The air humidity of the beginning of ventilation (in percent)
    */
    uint8_t HumStartVent;
    /*!
    * @brief The air humidity of the finishing of ventilation (in percent)
    */
    uint8_t HumEndVent;
    /*!
    * @brief The air humidity of the beginning of humidification (in percent)
    */
    uint8_t HumStartHum;
    /*!
    * @brief The air humidity of the finishing of humidification (in percent)
    */
    uint8_t HumEndHum;
    /*!
    * @brief Current air temperature (in Celsius)
    */
    float Temp;
    /*!
    * @brief Current air humidity (in percent)
    */
    uint8_t Hum;
};

/*!
* @brief Soil moisture sensor information and configuration
*/
struct __attribute__((packed)) SoilSensorInfo {
    /*!
    * @brief Device identificator value
    */
    uint16_t DeviceID;
    /*!
    * @brief Minimal soil moisture (0%)
    * @details Calibrate or manual input value
    */
    uint16_t MinValue;
    /*!
    * @brief Maximal soil moisture (100%)
    * @details Calibrate or manual input value
    */
    uint16_t MaxValue;
    /*!
    * @brief The percentage of soil moisture at which watering begins 
    */
    uint8_t StartWatering;
    /*!
    * @brief The percentage of soil moisture at which watering ends
    */
    uint8_t EndWatering;
    /*!
    * @brief Current soil moisture
    */
    uint8_t Hum;
};

/*!
* @brief Activity period configuration 
* @details If begin part of the prtiod equal end of the period or period has incorrect values - ignore period and always device active
*/
struct __attribute__((packed)) PeriodInfo {
    /*!
    * @brief Hour of the beginning activity
    */
    uint8_t StartH;
    /*!
    * @brief Minute of the beginning activity
    */
    uint8_t StartM;
    /*!
    * @brief Hour of the ending activity
    */
    uint8_t EndH;
    /*!
    * @brief Minute of the ending activity
    */
    uint8_t EndM;
};

/*!
* @brief Common relay information and configuration
*/
struct __attribute__((packed)) RelayInfo {
    /*!
    * @brief Device identificator value
    */
    uint16_t DeviceID;
    /*!
    * @brief Lamp activity period
    */
    PeriodInfo Activity;
    /*!
    * @brief Activity relay status
    * @details <b>0x00</b> - Inactive<br><b>0x01</b> - Active
    */
    uint8_t IsActive;
    /*!
    * @brief Working mode
    * @details <b>0x00</b> - Manual mode<br><b>0x01</b> - Automatic mode
    */
    uint8_t IsAuto;
};

/*!
* @brief Pump relay information and configuration
*/
struct __attribute__((packed)) PumpRelayInfo {
    /*!
    * @brief Common relay information and configuration
    */
    RelayInfo Common;
    /*!
    * @brief Maximum pump working time in seconds
    * @details <b>0</b> - Infinity time (stops by command only)
    */
    uint16_t MaxWorkTime;
    /*!
    * @brief Pause between pump runs in seconds
    * @details <b>0</b> - No pause between pump starts
    */
    uint16_t PauseBetweenStart;
    /*!
    * @brief Watering frequency in hours
    * @details <b>0</b> - Ignore this parameter otherwise maximum watering frequency in hours
    */
    uint8_t FreqWatering;
};