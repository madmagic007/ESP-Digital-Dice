#pragma once

#include <Arduino.h>
#include <math.h>

#define BAT_ADC_PIN 2
#define VREF 3.3f
#define ADC_MAX 4095.0f
#define BAT_DIV_RATIO ((10.0f + 33.0f) / 10.0f)

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
                sum += analogRead(BAT_ADC_PIN);
                delay(1);
            }

            float avg = sum / (float)samples;
            float v_adc = (avg / ADC_MAX) * VREF;
            float v_batt = v_adc * BAT_DIV_RATIO;

            return round(v_batt * 100.0) / 100.0;
        }
};