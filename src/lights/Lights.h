#pragma once

#include <soc/gpio_struct.h>
#include <Arduino.h>
#include <PinMapping.h>

class Lights {
private:
    static u8_t diceValues[3];
    static u8_t dicePins[3];
    static u8_t allPipPins[4];
    static u8_t diceValueMasks[6];

    static inline hw_timer_t* timer = nullptr;
    static void IRAM_ATTR timerISR();

    static inline volatile u8_t stage = 0;
    static inline volatile u8_t curDice = 0;
    static inline volatile u8_t timeOn = 2;
    static inline volatile u8_t timeOff = 18;
public:
    static void setup();
    static void setValue(u8_t val1, u8_t dice);
    static void setValues(u8_t val1, u8_t val2, u8_t val3);
    static void displayBatteryValue();
    static void setBrightness(u8_t val);
};