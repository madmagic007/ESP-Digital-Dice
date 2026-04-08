#pragma once

#include "Arduino.h"
#include <Wire.h>
#include <randomiser/Randomiser.h>

#define ADDR 0x62
#define REG_PARTID 0x01
#define AXIS_X 0x02
#define AXIS_Y 0x04
#define AXIS_Z 0x06
#define REG_RANGE 0x0F
#define REG_ODR 0x10
#define REG_POWER_MODE 0x11

class Accel {
private:
    static void writeRegister(u8_t reg, u8_t value);
    static u8_t readRegister(u8_t reg);
    static void loopTask(void* pvParameters);
public:
    static void setup();
    static int16_t readAxis(u8_t reg);
};