#pragma once

#include <Arduino.h>
#include <lights/Lights2.h>

class Randomiser {
private:
    static void loopTask1(void* pvParameters);
    static void loopTask2(void* pvParameters);
    static void loopTask3(void* pvParameters);

    static volatile bool stopRequested;
    static volatile u8_t stoppedCount;
public:
    static void start();
    static void stop();
};