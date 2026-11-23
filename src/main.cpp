/*!
* @file main.cpp
* @brief The main entry point of the GROWBOX firmware
* @authors John Doe
* @date 2026-11-08
* @version 1.0.0
*/
#include <Arduino.h>
#include <controller.h>

Controller *ctrl;
//RTC *rtc;
//Air *air;

/*!
* @brief Device initialization after startup
*/
void setup() {
  //Serial.begin(9600);
  //rtc = new RTC();
  //air = new Air(11);
  //uint8_t reset_flags = MCUSR;
  //MCUSR = 0; 
  ctrl = new Controller();
  //char buf[10];
  //sprintf(buf, "MCUSR: %02X", reset_flags);
  //ctrl->LogDbg(buf);
//  Serial.println("SETUP");
//  Serial.flush();
}

/*!
* @brief The main firmware working loop
*/
void loop() {
/*  uint32_t unixtime;
  uint8_t rc = rtc->GetTime(&unixtime);
  if (!rc) {
    Serial.print("Time: ");
    Serial.println(unixtime, DEC);
  } else {
    Serial.print("RTC error: ");
    Serial.println(rc, HEX);
  }
  Serial.flush();
  AirData dat;
  rc = air->GetData(&dat);
  if (!rc) {
    Serial.print("Air temp: ");
    Serial.println((int)dat.Temperature, DEC);
    Serial.print("Air humidity: ");
    Serial.println(dat.Humidity, DEC);
  } else {
    Serial.print("Air error: ");
    Serial.println(rc, HEX);
  }
  Serial.flush();
  delay(2000);*/
  ctrl->Process();
}
