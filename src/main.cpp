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

/*!
* @brief Device initialization after startup
*/
void setup() {
  Serial.begin(9600);
  ctrl = new Controller();
}

/*!
* @brief The main firmware working loop
*/
void loop() {
  ctrl->Process();
}
