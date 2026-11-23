/*!
* @file air.cpp
* @brief Implementation for air humidity and temperature sensor class
* @authors John Doe
* @date 2026-11-09
* @version 1.0.0
*/
#include "air.h"
#include "hal.h"

Air::Air(uint8_t pin) {
    this->isInited = false;
    this->air = new iarduino_DHT(pin);
    this->pin = pin;
    this->last_op = millis();
    delay(AIR_SENSOR_START_DELAY);
    this->isInited = true;
}

Air::~Air() {
    if (this->air != nullptr) {
        delete this->air;
        this->air = nullptr;
        this->isInited = false;
    }
}

bool Air::IsInited() {
    return this->isInited;
}

uint8_t Air::GetData(AirData *data)
{
    if (!this->isInited || this->air == nullptr) {
        return 1;
    }
    if (data == nullptr) {
        return 2;
    }
    unsigned long now = millis();
    if ((now - this->last_op) < AIR_SENSOR_OPERATION_DELAY) {
        return 3;
    }
    int8_t rc = this->air->read();
    this->last_op = now;

    float hum = this->air->hum;
    if (hum < 0 || hum > 100) {
        return 5;
    }

    switch (rc) {
    case DHT_OK:
        data->Temperature = this->air->tem;
        data->Humidity = round(hum);
        return 0;
    case DHT_ERROR_CHECKSUM:
        return 4;
    case DHT_ERROR_DATA:
        return 5;
    case DHT_ERROR_NO_REPLY:
        return 6;
    };

    return 7;
}
