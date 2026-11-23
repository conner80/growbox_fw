/*!
* @file relay.cpp
* @brief Implementation for relay control class
* @authors John Doe
* @date 2026-11-09
* @version 1.0.0
*/
#include "relay.h"
#include "hal.h"

Relay::Relay(uint8_t pin, uint8_t relay_type) {
    pinMode(pin, OUTPUT);
    switch (relay_type) {
    case 3:
        this->delay_ms = KEY_DELAY;
        this->is_normal = false;
        break;
    case 2:
        this->delay_ms = KEY_DELAY;
        this->is_normal = true;
        break;
    default:
        this->delay_ms = RELAY_DELAY;
        this->is_normal = true;
        break;
    }
    digitalWrite(pin, (this->is_normal ? LOW : HIGH));
    this->pin = pin;
    this->last_op = millis();
}

bool Relay::IsActivated()
{
    if (digitalRead(this->pin) == HIGH) {
        if (this->is_normal) {
            return true;
        } else {
            return false;
        }
    } 

    if (this->is_normal) {
        return false;
    }

    return true;
}

uint8_t Relay::Activate() {
    unsigned long cur = millis();
    if ((cur - this->last_op) < this->delay_ms) {
        return 1;
    }
    digitalWrite(this->pin, (this->is_normal ? HIGH : LOW));
    this->last_op = cur;
    return 0;
}

uint8_t Relay::Deactivate() {
    unsigned long cur = millis();
    if ((cur - this->last_op) < this->delay_ms) {
        return 1;
    }
    digitalWrite(this->pin, (this->is_normal ? LOW : HIGH));
    this->last_op = cur;
    return 0;
}
