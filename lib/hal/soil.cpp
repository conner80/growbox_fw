/*!
* @file soil.cpp
* @brief Implementation for soil moisture sensor class
* @authors John Doe
* @date 2026-11-09
* @version 1.0.0
*/
#include "soil.h"
#include "hal.h"

Soil::Soil(uint8_t pin) {
    int v = analogRead(pin);
    int m = min(SOIL_MOISTURE_MIN, SOIL_MOISTURE_MAX);
    this->pin = pin;
    this->isInited = (v >= m);
}

bool Soil::IsInited() {
    return this->isInited;
}

uint8_t Soil::GetHumidity() {
    if (!this->isInited) {
        return 255;
    }
    int val_ = analogRead(this->pin);
    int min_ = min(SOIL_MOISTURE_MIN, SOIL_MOISTURE_MAX);
    int max_ = max(SOIL_MOISTURE_MIN, SOIL_MOISTURE_MAX);
    if (val_ < min_ || val_ > max_) {
        return 254;
    }
    uint8_t res = map(val_, SOIL_MOISTURE_MIN, SOIL_MOISTURE_MAX, 0, 100);
    return res;
}
