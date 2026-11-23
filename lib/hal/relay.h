/*!
* @file relay.h
* @brief Header for relay control class
* @authors John Doe
* @date 2026-11-09
* @version 1.0.0
*/
#pragma once
#include <Arduino.h>

class Relay {
private:
    bool is_normal;
    uint8_t delay_ms;
    uint8_t pin;
    unsigned long last_op;
public:
    /*!
    * @brief Constructor
    * @param[in] pin MCU analog PIN number for relay signal
    * @param[in] relay_type Relay type: 1 - relay, 2 - n-channel key, 3 - p-channel key
    */
    Relay(uint8_t pin, uint8_t relay_type);
    /*!
    * @brief Check relay activation status
    * @return <b>TRUE</b> Relay was activate by high level signal, otherwise <b>FALSE</b>
    */
    bool IsActivated();
    /*!
    * @brief Activate relay by high level signal
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Relay rate limit error
    */
    uint8_t Activate();
    /*!
    * @brief Deactivate relay by low level signal
    * @return Operation result code
    * @retval 0 Success
    * @retval 1 Relay rate limit error
    */
    uint8_t Deactivate();
};