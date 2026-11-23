/*!
* @file controller.h
* @brief Header for the main Growbox controller class
* @authors John Doe
* @date 2026-11-10
* @version 1.0.0
*/
#pragma once
#include <Arduino.h>
#include "hal.h"
#include "rtc.h"
#include "soil.h"
#include "air.h"
#include "storage.h"
#include "relay.h"
#include "parameters.h"
#include "bluetooth.h"
#include "button.h"

/*!
* @brief RTC 
*/
#define SENS_RTC 0x0001
/*!
* @brief EEPROM storage
*/
#define SENS_STORAGE 0x0002
/*!
* @brief Air humidity and temperature sensor (DHT-11, DHT-22)
*/
#define SENS_AIR 0x0004
/*!
* @brief Soil moisture sensor 1
*/
#define SENS_SOIL1 0x0008
/*!
* @brief Soil moisture sensor 2
*/
#define SENS_SOIL2 0x0010
/*!
* @brief Reserved for sensor
*/
#define SENS_RES1 0x0020
/*!
* @brief Reserved for sensor
*/
#define SENS_RES2 0x0040
/*!
* @brief Reserved for sensor
*/
#define SENS_RES3 0x0080
/*!
* @brief Lamp relay
*/
#define DEV_LAMP 0x0100
/*!
* @brief Fan relay
*/
#define DEV_FAN 0x0200
/*!
* @brief Pump relay
*/
#define DEV_PUMP 0x0400
/*!
* @brief Air humidifier relay
*/
#define DEV_HUMIDIFIER 0x0800
/*!
* @brief Bluetooth activation button
*/
#define DEV_BT_BUTTON 0x1000

/*!
* @brief The main Growbox controller class 
*/
class Controller {
private:
    bool isStdMode = true;
    uint16_t device_mask; // initialized devices
    RTC *rtc;
    Air *air;
    Storage *storage;
    Soil *sensor1;
    Soil *sensor2;
    Relay *lamp;
    Relay *fan;
    Relay *pump;
    Relay *humidifier;
    Bluetooth *bt;
    Button *btn;
    GrowConfig cfg;
    Params pars;
    unsigned long pump_act;
    unsigned long pump_sleep;
    unsigned long last_op; 

    /*!
    * @brief Receive answer data event
    * @details Answer buffer must return from callback because of is NIL by default
    * @details Answer length parameter has initialized (NOT NIL) before enter callback
    * @param[in] context Reference to the caller class
    * @param[in] cmd Command
    * @param[in] req_data Request data
    * @param[in] req_len Request data length
    * @param[inout] asw_data Answer data
    * @param[inout] asw_len Answer data length
    */
    static void onReceive(void *context, const uint8_t cmd, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len);
    /*!
    * @brief Execute #CMD_INFO command 
    * @param[in] instance Reference to the Controller class
    * @param[inout] asw_data Answer data
    * @param[inout] asw_len Answer data length
    */
    static void getInfo(Controller *instance, uint8_t *asw_data, size_t *asw_len);
    /*!
    * @brief Execute #CMD_GET_BT_INFO command 
    * @param[in] instance Reference to the Controller class
    * @param[inout] asw_data Answer data
    * @param[inout] asw_len Answer data length
    */
    static void getBTInfo(Controller *instance, uint8_t *asw_data, size_t *asw_len);
    /*!
    * @brief Execute #CMD_SET_BT_NAME command
    * @param[in] instance Reference to the Controller class
    * @param[in] req_data Request data
    * @param[in] req_len Request data length
    * @param[out] asw_data Answer data
    * @param[out] asw_len Answer data length
    */
    static void setBTName(Controller *instance, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len);
    /*!
    * @brief Execute #CMD_SET_BT_PASS command
    * @param[in] instance Reference to the Controller class
    * @param[in] req_data Request data
    * @param[in] req_len Request data length
    * @param[out] asw_data Answer data
    * @param[out] asw_len Answer data length
    */
    static void setBTPass(Controller *instance, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len);
    /*!
    * @brief Execute #CMD_GET_TIME command 
    * @param[in] instance Reference to the Controller class
    * @param[inout] asw_data Answer data
    * @param[inout] asw_len Answer data length
    */
    static void getTime(Controller *instance, uint8_t *asw_data, size_t *asw_len);
    /*!
    * @brief Execute #CMD_SET_TIME command
    * @param[in] instance Reference to the Controller class
    * @param[in] req_data Request data
    * @param[in] req_len Request data length
    * @param[out] asw_data Answer data
    * @param[out] asw_len Answer data length
    */
    static void setTime(Controller *instance, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len);
    /*!
    * @brief Execute #CMD_GET_STATUS command
    * @param[in] instance Reference to the Controller class
    * @param[in] req_data Request data
    * @param[in] req_len Request data length
    * @param[out] asw_data Answer data
    * @param[out] asw_len Answer data length
    */
    static void getStatus(Controller *instance, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len);
    /*!
    * @brief Execute #CMD_SET_STATUS command
    * @param[in] instance Reference to the Controller class
    * @param[in] req_data Request data
    * @param[in] req_len Request data length
    * @param[out] asw_data Answer data
    * @param[out] asw_len Answer data length
    */
    static void setStatus(Controller *instance, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len);
    /*!
    * @brief Execute #CMD_GET_MODE command
    * @param[in] instance Reference to the Controller class
    * @param[out] asw_data Answer data
    * @param[out] asw_len Answer data length
    */
    static void getMode(Controller *instance, uint8_t *asw_data, size_t *asw_len);
    /*!
    * @brief Execute #CMD_SET_MODE command
    * @param[in] instance Reference to the Controller class
    * @param[in] req_data Request data
    * @param[in] req_len Request data length
    * @param[out] asw_data Answer data
    * @param[out] asw_len Answer data length
    */
    static void setMode(Controller *instance, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len);
protected:
    /*!
    * @brief Read all initialized sensors values
    * @return Bit mask of sensors which successfully processed
    */
    uint8_t readSensors();
    /*!
    * @brief Check the pump running time and deactivate it after timeout exceed
    */
    inline void checkPumpRunning();
    /*!
    * @brief Process lamp relay
    * @param[in] is_active_rtc RTC device is active
    */
    void processLamp(bool is_active_rtc);
    /*!
    * @brief Process fan relay
    * @param[in] is_active_rtc RTC device is active
    * @param[in] is_active_air Air humidity and temperature sensor os active
    */
    void processFan(bool is_active_rtc, bool is_active_air);
    /*!
    * @brief Process pump relay
    * @param[in] is_active_rtc RTC device is active
    * @param[in] is_active_soil1 Soil moisture sensor 1 is active
    * @param[in] is_active_soil2 Soil moisture sensor 2 is active
    */
    void processPump(bool is_active_rtc, bool is_active_soil1, bool is_active_soil2);
    /*!
    * @brief Process humidifier relay
    * @param[in] is_active_rtc RTC device is active
    * @param[in] is_active_air Air humidity and temperature sensor os active
    */
    void processHumidifier(bool is_active_rtc, bool is_active_air);
    /*!
    * @brief Check Bluetooth activate button state and activate device if needed
    */
    void checkBTButton();
public:
    /*!
    * @brief Constructor
    */
    Controller();
    /*!
    * @brief Destructor
    */
    virtual ~Controller();
    /*!
    * @brief Get device mode
    * @return <b>TRUE</b> - Standart mode otherwise Callibrate mode
    */
    virtual bool IsStdMode();
    /*!
    * @brief Initialized modules/devices mask
    */
    virtual uint16_t Devices();
    /*!
    * @brief Reset user configuration to default
    */
    void ResetConfig();
    /*!
    * @brief Read user configuration from EEPROM
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect data block
    * @retval 2 Incorrect data block length
    * @retval 3 Incorrect EEPROM memory address
    * @retval 4 Device wasn't initialized (no data found)
    * @retval 5 Device isn't active
    */
    uint8_t ReadConfig();
    /*!
    * @brief Write current user configuration to EEPROM
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect data block
    * @retval 2 Incorrect data block length
    * @retval 3 Incorrect EEPROM memory address
    * @retval 4 Data write error
    * @retval 5 Device isn't active
    */
    uint8_t WriteConfig();
    /*!
    * @brief Set new date and time to RTC (Unix-time format)
    * @param[in] datetime New date and time in Unix-time format
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Set datetime error due to device error or uninitialized
    */
    uint8_t SetTime(uint32_t datetime);
    /*!
    * @brief Get current date and time from RTC (Unix-time format)
    * @param[inout] datetime Buffer for saving current date and time
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Get datetime error due to device error or uninitialized
    * @retval 2 Parameter is nil
    */
    uint8_t GetTime(uint32_t *datetime);
    /*!
    * @brief Devices activation management
    * @param[in] dev_mask Bit mask for device activation
    */
    void ActivateDevices(uint16_t dev_mask);
    /*!
    * @brief Get active devices at the moment bit mask
    * @return Bit mask of active relay devices
    */
    uint16_t GetActiveDevices();
    /*!
    * @brief Set device mode: callibrate or standart opearion mode
    * @param[in] isStdandart Is standart operation mode or callibrate mode
    */
    void SetMode(bool isStdandart = true);
    /*!
    * @brief Process loop iteration and manage sensors and devices
    */
    void Process();
};