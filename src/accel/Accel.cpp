#include "Accel.h"
#include <PinMapping.h>

void Accel::setup() {
    Wire.begin(ACCEL_SDA, ACCEL_SCL);
    writeRegister(REG_RANGE, 0x00);
    writeRegister(REG_POWER_MODE, 0x00);
    writeRegister(REG_ODR, 0x07);

    xTaskCreate(loopTask, "accelTask", 4096, NULL, 1, NULL);
}

void Accel::writeRegister(u8_t reg, u8_t value) {
    Wire.beginTransmission(ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

u8_t Accel::readRegister(u8_t reg) {
    Wire.beginTransmission(ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    
    Wire.requestFrom(ADDR, 1);
    return Wire.read();
}

int16_t Accel::readAxis(u8_t reg) {
    u8_t lsb = readRegister(reg);
    u8_t msb = readRegister(reg + 1);

    int16_t value = (msb << 4) | (lsb & 0x0F);
    
    if (value & 0x0800) { // sign extend for 12-bit to 16-bit
        value |= 0xF000;
    }
    
    return value;
}

const int WINDOW_SIZE = 100;
float bufferX[WINDOW_SIZE];
float bufferY[WINDOW_SIZE];
int _index = 0;
int count = 0;

float prevX = 0;
float prevY = 0;

bool isShake = false;
bool stopShaken = true;

void Accel::loopTask(void* pvParameters) {
    for (int i = 0; i < WINDOW_SIZE; i++) {
        bufferX[i] = 0;
        bufferY[i] = 0;
    }

    while (true) {
        float rawX = readAxis(AXIS_X) / 1024.0;
        float rawY = readAxis(AXIS_Y) / 1024.0;

        bufferX[_index] = rawX;
        bufferY[_index] = rawY;

        float sumAbs = 0;
        int flips = 0;

        for (int i = 0; i < WINDOW_SIZE; i++) {
            sumAbs += abs(bufferX[i]);
            sumAbs += abs(bufferY[i]);

            if (i > 0) {
                if ((bufferX[i - 1] >= 0 && bufferX[i] < 0) || (bufferX[i - 1] < 0 && bufferX[i] >= 0)) {
                    flips++;
                }
                
                if ((bufferY[i - 1] >= 0 && bufferY[i] < 0) || (bufferY[i - 1] < 0 && bufferY[i] >= 0)) {
                    flips++;
                }
            }
        }

        float avgAbs = sumAbs / WINDOW_SIZE;

        if (avgAbs > 1 && flips > 4) {
            isShake = true;
        } else {
            isShake = false;
        }

        if (isShake && stopShaken) {
            Randomiser::start();
            stopShaken = false;
        }

        if (!isShake && !stopShaken) {
            Randomiser::stop();
            stopShaken = true;
        }

        _index = (_index + 1) % WINDOW_SIZE;
        if (count < WINDOW_SIZE) count++;

        delay(1);
    }
}