#include <Arduino.h>
#include <lights/Lights.h>
#include <accel/Accel.h>
#include <WiFi.h>
#include <ble/ble.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);

  Lights::setup();
  Lights::setBrightness(10);
  Lights::displayBatteryValue();
  
  ble::init();

  delay(2000);
  Lights::setValues(0, 0, 0);

  Accel::setup();
}


void loop() {
  delay(10000);

  float battVoltage = Util::readBatteryVoltage();
  float max = 4.2;
  float depleted = 3.0;

  if (battVoltage <= depleted) {
    ESP.deepSleep(0);
    return;
  }

  u8_t perct = Util::getBatteryPercentage();
  ble::notifyParam("battery", perct);
  ble::notifyParam("brightness", Lights::brightness);
}