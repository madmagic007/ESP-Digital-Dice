#include <Arduino.h>
#include <lights/Lights2.h>
#include <accel/Accel.h>
#include <WiFi.h>


void setup() {
  WiFi.mode(WIFI_OFF);
  Lights::setup();
  Lights::setBrightness(10);

  Lights::displayBatteryValue();
  delay(5000);
  Lights::setValues(0, 0, 0);

  Accel::setup();
}

void loop() {}