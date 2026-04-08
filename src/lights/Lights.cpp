
#include "Lights.h"
#include <Util.h>

u8_t Lights::diceValues[3] = {0, 0, 0};
u8_t Lights::dicePins[3] = {DICE_1, DICE_2, DICE_3};
u8_t Lights::allPipPins[4] = {DICE_PIP_4, DICE_PIP_1_7, DICE_PIP_2_6, DICE_PIP_3_5};

u8_t Lights::diceValueMasks[6] = {
    0b0001,
    0b0100,
    0b0101,
    0b0110,
    0b0111,
    0b1110,  
};

void Lights::setup() {    
    Util::pinOutput(DICE_PIP_4, HIGH);
    Util::pinOutput(DICE_PIP_1_7, HIGH);
    Util::pinOutput(DICE_PIP_2_6, HIGH);
    Util::pinOutput(DICE_PIP_3_5, HIGH);
    
    Util::pinOutput(DICE_1, LOW);
    Util::pinOutput(DICE_2, LOW);
    Util::pinOutput(DICE_3, LOW);
    
    timer = timerBegin(1000000); // 1mhz?
    timerAttachInterrupt(timer, &Lights::timerISR);
    timerAlarm(timer, 1, false, 0);
}

void Lights::setValue(u8_t val, u8_t dice) {
    diceValues[dice] = val;
}

void Lights::setValues(u8_t val1, u8_t val2, u8_t val3) {
    diceValues[0] = val1;
    diceValues[1] = val2;
    diceValues[2] = val3;
}

void Lights::displayBatteryValue() {
    float batteryVoltage = Util::readBatteryVoltage();
    float percentage = (batteryVoltage - 3.0) / (4.2 - 3.0) * 100.0;
    u8_t diceSum = round(percentage / 100.0 * 18.0);
    
    u8_t die1 = diceSum >= 6 ? 6 : diceSum;
    diceSum -= die1;
    
    u8_t die2 = diceSum >= 6 ? 6 : diceSum;
    diceSum -= die2;
    
    u8_t die3 = diceSum >= 6 ? 6 : diceSum;

    setValues(die1, die2, die3);
} 

void Lights::setBrightness(u8_t val) {
    if (val > 100) val = 100;
    if (val < 1) val = 1;
    
    u32_t on = val;
    u32_t off = 100 - val;
    
    if (off == 0) {
        timeOn = 2;
        timeOff = 0;
        return;
    }
    
    u32_t a = on, b = off;
    while (b != 0) {
        u32_t t = b;
        b = a % b;
        a = t;
        
        Serial.println("in loop");
    }
    u32_t gcd = a;
    
    Serial.println("after loop");
    
    on /= gcd;
    off /= gcd;
    
    const u32_t minPeriod = 8; // too fast firing timer causes hang
    u32_t total = on + off;
    if (total < minPeriod) {
        u32_t scale = (minPeriod + total - 1) / total;
        on *= scale;
        off *= scale;
    }
    
    timeOn = (u8_t)on;
    timeOff = (u8_t)off;
}

/*
0: turn on cur dice, turn on pips, activate stage 1
1: turn off all pips, activate stage 2
2: cur dice off, increment cur dice, activate stage 0 after 1us
*/

void IRAM_ATTR Lights::timerISR() {
    switch (stage) {
        case 0: {
            u8_t diceValue = diceValues[curDice];
            if (diceValue != 0) {
                
                u8_t dicePin = dicePins[curDice];
                u8_t currentMask = diceValueMasks[diceValue - 1];
                
                u32_t setMask = 0, clrMask = 0;
                for (u8_t i = 0; i < 4; i++) {
                    u32_t pinBit = (1 << allPipPins[i]);
                    if (currentMask & (1 << i)) {
                        clrMask |= pinBit;
                    } else {
                        setMask |= pinBit;
                    }
                }
                
                GPIO.out_w1ts.val = (1 << dicePin);  // active dice gpio on (n channel on)
                GPIO.out_w1tc.val = clrMask; // active pips gpio low (p channel on)
            }
            
            stage = 1;
            timerWrite(timer, 0);
            timerAlarm(timer, timeOn, false, 0);
            
            break;
        }
        case 1: {
            GPIO.out_w1ts.val = 0x3C0000; // all pips gpio high (p channel off)
            
            stage = 2;
            timerWrite(timer, 0);
            timerAlarm(timer, timeOff, false, 0);
            
            break;
        }
        case 2: {
            u8_t dicePin = dicePins[curDice];
            GPIO.out_w1tc.val = (1 << dicePin); // active dice gpio off (n channel off)
            
            curDice = (curDice + 1) % 3;
            
            stage = 0;
            timerWrite(timer, 0);
            timerAlarm(timer, 1, false, 0); // 1us
            
            break;
        }
    }
}