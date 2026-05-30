#pragma once

#include <Arduino.h>
#include <math.h>

#define BAT_ADC_PIN 2
#define BAT_DIV_RATIO ((10.0f + 33.0f) / 33.0f)

class Util {
    public:
        static void pinOutput(u8_t pin, bool defaultVal) {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, defaultVal);
        }

        static float readBatteryVoltage() {
            const int samples = 32;
            long sum = 0;

            for (int i = 0; i < samples; i++) {
                sum += analogReadMilliVolts(BAT_ADC_PIN);
                delay(1);
            }

            float avg = sum / (float)samples;
            float v_batt = avg * BAT_DIV_RATIO / 1000.0;

            return round(v_batt * 100.0) / 100.0;
        }

        static u8_t getBatteryPercentage() {
            float voltage = readBatteryVoltage();
            float pct = (voltage - 3.0) / (4.2 - 3.0) * 100.0;
            return round(pct);
        }
};