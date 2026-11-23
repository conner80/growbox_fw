/*!
* @file rtc.cpp
* @brief Implementation for RTC class
* @authors John Doe
* @date 2026-11-09
* @version 1.0.0
*/
#include "rtc.h"
#include "hal.h"

RTC::RTC() {
    this->rtc = new iarduino_RTC(RTC_DEVICE); 
    this->isInited = this->rtc->begin();     
}

RTC::~RTC() {
    if (this->rtc != nullptr) { 
        delete this->rtc;
        this->rtc = nullptr;
    } 
    this->isInited = false;
}

bool RTC::IsInited() {
    return this->isInited;
}

uint8_t RTC::SetTime(uint32_t unixtime) {
    if (!this->isInited || this->rtc == nullptr) {
        return 1;
    }
    this->rtc->settimeUnix(unixtime);
    uint32_t now = this->rtc->gettimeUnix();
    if (now != unixtime) {
        return 1;
    }
    return 0;
}

uint8_t RTC::GetTime(uint32_t *unixtime) {
    if (!this->isInited || this->rtc == nullptr) {
        return 1;
    }
    if (unixtime == nullptr) {
        return 2;
    }
    *unixtime = this->rtc->gettimeUnix();
    return 0;
}
