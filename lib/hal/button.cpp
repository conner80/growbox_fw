/*!
* @file button.cpp
* @brief Implementation for user's button class
* @authors John Doe
* @date 2026-11-20
* @version 1.0.0
*/
#include "button.h"

/*!
* @brief Button debounce delay in milliseconds
*/
#define DEBOUNCE_DELAY 50

Button::Button(uint8_t pin, bool need_pullup) {
    this->pin = pin;
    if (need_pullup) {
        pinMode(pin, INPUT_PULLUP);
    } else {
        pinMode(pin, INPUT);
    }
}

bool Button::IsPressed() {
    int st = digitalRead(this->pin);
    bool res = false;
    if (st != this->last_st) {
        this->last_op = millis();
    }

    if ((millis() - this->last_op) > DEBOUNCE_DELAY) {
        if (st != this->state) {
            this->state = st;
            if (st == LOW) {
                res = true;
            }
        }
    }
    this->last_st = st;
    
    return res;
}
