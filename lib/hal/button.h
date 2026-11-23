/*!
* @file button.h
* @brief Header for user's button class
* @authors John Doe
* @date 2026-11-20
* @version 1.0.0
*/
#pragma once
#include <Arduino.h>

/*!
* @brief User's button pre-processor class
*/
class Button {
private:
    uint8_t pin;
    int state = HIGH;
    int last_st = HIGH;
    unsigned long last_op = 0;
public:
    /*!
    * @brief Constructor
    * @param[in] pin MCU digital pin for reading button state
    * @param[in] need_pullup Need pinup MCU pin by internal resistor or not
    */
    Button(uint8_t pin, bool need_pullup = true);
    /*!
    * @brief Check button state
    * @return Button state
    */
    bool IsPressed();
};