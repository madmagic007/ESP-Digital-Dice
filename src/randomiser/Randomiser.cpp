#include "Randomiser.h"

volatile bool Randomiser::stopRequested = false;
volatile u8_t Randomiser::stoppedCount = 3;

void Randomiser::start() {
    if (stoppedCount < 3) return;

    stopRequested = false;
    stoppedCount = 0;

    xTaskCreate(loopTask1, "RandomTask1", 2048, NULL, 1, NULL);
    xTaskCreate(loopTask2, "RandomTask2", 2048, NULL, 1, NULL);
    xTaskCreate(loopTask3, "RandomTask3", 2048, NULL, 1, NULL);
    
}

unsigned long slowdownStart = 0;

void Randomiser::stop() {
    stopRequested = true;
    slowdownStart = millis();
}

const int baseDelayMs = 50;
const int maxRandomDelay = 200;
const int slowdownDurationMs = 1500;

void Randomiser::loopTask1(void* pvParameters) {
    while (true) {
        Lights::setValue(random(1, 7), 0);
        
        int delayTime = random(baseDelayMs, maxRandomDelay);
        
        if (stopRequested) {
            unsigned long elapsed = millis() - slowdownStart;
            if (elapsed >= slowdownDurationMs) {
                break;
            }
            
            delayTime += ((slowdownDurationMs - baseDelayMs) * elapsed) / slowdownDurationMs;
        }
        
        delay(delayTime);
    }

    stoppedCount = stoppedCount + 1;
    
    vTaskDelete(NULL);
}

void Randomiser::loopTask2(void* pvParameters) {
    while (true) {
        Lights::setValue(random(1, 7), 1);
        
        int delayTime = random(baseDelayMs, maxRandomDelay);
        
        if (stopRequested) {
            unsigned long elapsed = millis() - slowdownStart;
            if (elapsed >= slowdownDurationMs) {
                break;
            }
            
            delayTime += ((slowdownDurationMs - baseDelayMs) * elapsed) / slowdownDurationMs;
        }
        
        delay(delayTime);
    }

    stoppedCount = stoppedCount + 1;
    
    vTaskDelete(NULL);
}

void Randomiser::loopTask3(void* pvParameters) {
    while (true) {
        Lights::setValue(random(1, 7), 2);
        
        int delayTime = random(baseDelayMs, maxRandomDelay);
        
        if (stopRequested) {
            unsigned long elapsed = millis() - slowdownStart;
            if (elapsed >= slowdownDurationMs) {
                break;
            }
            
            delayTime += ((slowdownDurationMs - baseDelayMs) * elapsed) / slowdownDurationMs;
        }
        
        delay(delayTime);
    }

    stoppedCount = stoppedCount + 1;
    
    vTaskDelete(NULL);
}