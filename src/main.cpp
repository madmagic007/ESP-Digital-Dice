#include <Arduino.h>
#include <lights/Lights.h>
#include <accel/Accel.h>
#include <WiFi.h>
#include <ble/ble.h>


void setup() {
  WiFi.mode(WIFI_OFF);

  Lights::setup();
  Lights::setBrightness(10);
  Lights::displayBatteryValue();
  
  ble::init();

  delay(2000);
  Lights::setValues(0, 0, 0);

  Accel::setup();
}

void loop() {}