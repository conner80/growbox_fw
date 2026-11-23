/*!
* @file bluetooth.h
* @brief Header for bluetooth transport class for chip HC-05 
* @authors John Doe
* @date 2026-11-16
* @version 1.0.0
*/
#pragma once
#include <SoftwareSerial.h>
#include "hal.h"
#include "relay.h"

/*!
* @brief Bluetooth device status
*/
enum class EnumStatus {Disabled, Config, Transport};

/*!
* @brief Receive packet (answer data) stage
*/
enum class EnumReceiveStage {WAIT_BEGIN, WAIT_COMMAND, WAIT_LENGTH, READ_DATA, ACK_DATA};

/*!
* @brief Receive answer data event
* @param[in] context Reference to the caller class
* @param[in] cmd Command
* @param[in] req_data Request data
* @param[in] req_len Request data length
* @param[inout] asw_data Answer data
* @param[inout] asw_len Answer data length
*/
typedef void (*OnReceiveDataEvent) (void *context, const uint8_t cmd, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len);

/*!
* @brief Header for bluetooth transport class for chip HC-05
* @details For configre using AT-commands (ASCII text), for transport - transport commands (binary data) 
*/
class Bluetooth {
private:
    EnumStatus status = EnumStatus::Disabled;
    EnumReceiveStage stage = EnumReceiveStage::WAIT_BEGIN;
    uint8_t cmd = 0x00;
    uint8_t req_len = 0;
    uint8_t rcv_len = 0;
    bool isInited = false;
    bool isCalibrate = false;
    SoftwareSerial *port = nullptr;
    Relay *pwr = nullptr;
    uint8_t req_buf[255] = {0x00};
    unsigned long last_op;
protected:
    /*!
    * @brief Send AT-command  to Bluetooth
    * @param[in] cmd AT-command
    * @param[inout] asw Answer from Bluetooth
    * @param[in] len Answer string/buffer maximum length
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect Bluetooth mode
    * @retval 2 Invalid AT-command
    */
    uint8_t sendATCommand(const char *cmd, char *asw, size_t len);
    /*!
    * @brief Receive data event callback pre-processor
    * @param[in] cmd Command
    */
    void onReceive(const uint8_t cmd);
    /*!
    * @brief Send positive acknowlage packet 
    */
    void sendACK();
    /*!
    * @brief Send negative acknowlage packet 
    */
    void sendNACK();
    /*!
    * @brief Request common Bluetooth parameters via AT-commands
    */
    void reqBTParams();
public:
    /*!
    * @brief The time without connection after which Bluetooth will turn off automatically in seconds
    */
    uint16_t IdleOfflineDelay = 0;
    /*!
    * @brief Bluetooth slave device name
    */
    char Name[33] = {"\0"};
    /*!
    * @brief Bluetooth slave device password
    */
    uint16_t Password = 0;
    /*!
    * @brief Bluetooth device address
    */
    char Address[15] = {"\0"};
    /*!
    * @brief Bluetooth firmware version
    */
    char Version[20] = {"\0"};
    /*!
    * @brief Reference to the caller class
    */
    void *Context = nullptr;
    /*!
    * @brief Receive answer data event (callback) function
    */
    OnReceiveDataEvent OnReceiveData = nullptr;

    /*!
    * @brief Constructor
    */
    Bluetooth();
    /*!
    * @brief Destructor
    */
    virtual ~Bluetooth();
    /*!
    * @brief Bluetooth module power active status
    * @return <b>TRUE</b> - Power is ON and Bluetooth module is active otherwise <b>FALSE</b>
    */
    bool IsActivated();
    /*!
    * @brief Bluetooth successfully initalized or nor
    * @return Bluetooth initialization status
    */
    bool IsInited();
    /*!
    * @brief Bluetooth device connection status
    * @return <b>TRUE</b> - Master device already connected otherwise <b>FALSE</b>
    */
    bool IsConnected();
    /*!
    * @brief Returns Bluetooth device current status
    * @return Bluetooth device current status (see #EnumStatus)
    */
    EnumStatus Status();
    /*!
    * @brief Activate Bluetooth device (POWER ON)
    * @param[in] isTransport Set device transport mode or config mode (AT-commands)
    * @return Opration result code
    * @retval 0 Success
    * @retval 1 Relay rate limit error
    * @retval 2 Relay not initialized
    */
    uint8_t Activate(bool isTransport = true);
    /*!
    * @brief Deactivate Bluetooth device (POWER OFF) 
    * @return Opration result code
    * @retval 0 Success
    * @retval 1 Relay rate limit error
    * @retval 2 Relay not initialized
    */
    uint8_t Deactivate();
    /*!
    * @brief Change Bluetooth device mode
    * @param[in] isConfig Перевести устройство в режим AT-команд (конфигурационный) или работать в транспортном режиме
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Device not initialized
    */
    uint8_t ChangeMode(bool isConfig = false);
    /*!
    * @brief Send AT-command for checking Bluetooth device available
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect Bluetooth mode
    * @retval 2 Device is not available or error
    */
    uint8_t SendATPing();
    /*!
    * @brief Get Bluetooth address
    * @param[inout] addr Address of the device (NAP:UAP:LAP)
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect Bluetooth mode
    * @retval 2 Device is not available or error
    */
    uint8_t GetAddr(char *addr);
    /*!
    * @brief Get Bluetooth device firmware version
    * @param[inout] ver Version of the device firmware
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect Bluetooth mode
    * @retval 2 Device is not available or error
    */
    uint8_t GetVersion(char *ver);
    /*!
    * @brief Reset Bluetooth device settings to default
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect Bluetooth mode
    * @retval 2 Device is not available or error
    */
    uint8_t ResetToDefault();
    /*!
    * @brief Get current Bluetooth device name
    * @param[inout] name Bluetooth device name (max 32 characters)
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect Bluetooth mode
    * @retval 2 Device is not available or error
    */
    uint8_t GetName(char *name);
    /*!
    * @brief Set new Bluetooth device name for current module
    * @param[in] name Bluetooth device name (max 32 characters)
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect Bluetooth mode
    * @retval 2 Device is not available or error
    */
    uint8_t SetName(const char *name);
    /*!
    * @brief Get current Bluetooth device password
    * @param[inout] pass Bluetooth device password (max 16 characters)
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect Bluetooth mode
    * @retval 2 Device is not available or error
    */
    uint8_t GetPassword(char *pass);
    /*!
    * @brief Set new password for current Bluetooth device
    * @param[in] pass New Bluetooth device password (max 16 characters)
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect Bluetooth mode
    * @retval 2 Device is not available or error
    */
    uint8_t SetPassword(const char *pass);
    /*!
    * @brief Send answer response to the Master Bluetooth device
    * @param[in] cmd Command 
    * @param[in] asw Answer data
    * @param[in] asw_len Answer data length
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Incorrect command
    * @retval 2 Incorrect answer data
    * @retval 3 Incorrect Bluetooth mode
    * @retval 4 Device is not available or error
    */
    uint8_t SendAnswer(const uint8_t cmd, const uint8_t *asw, const size_t asw_len);
    /*!
    * @brief Processing Bluetooth transport commands
    */
    void ProcessCommands();  
};