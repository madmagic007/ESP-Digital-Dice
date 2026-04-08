// #include "Lights.h"
// #include <Util.h>

// u8_t Lights::diceValues[3] = {0, 0, 0};
// u8_t Lights::dicePins[3] = {DICE_1, DICE_2, DICE_3};
// u8_t Lights::allPipPins[4] = {DICE_PIP_4, DICE_PIP_1_7, DICE_PIP_2_6, DICE_PIP_3_5};

// u8_t Lights::diceValueMasks[6] = {
//     0b0001,
//     0b0100,
//     0b0101,
//     0b0110,
//     0b0111,
//     0b1110,  
// };

// void Lights::setup() {    
//     Util::pinOutput(DICE_PIP_4, HIGH);
//     Util::pinOutput(DICE_PIP_1_7, HIGH);
//     Util::pinOutput(DICE_PIP_2_6, HIGH);
//     Util::pinOutput(DICE_PIP_3_5, HIGH);

//     Util::pinOutput(DICE_1, LOW);
//     Util::pinOutput(DICE_2, LOW);
//     Util::pinOutput(DICE_3, LOW);

//     xTaskCreate(loopTask, "Lights", 2048, NULL, 1, NULL);
// }

// void Lights::setValues(u8_t val1, u8_t val2, u8_t val3) {
//     diceValues[0] = val1;
//     diceValues[1] = val2;
//     diceValues[2] = val3;
// }

// void Lights::setValue(u8_t val, u8_t dice) {
//     diceValues[dice] = val;
// }

// void Lights::loopTask(void* pvParameters) {
//     while (true) {
//         for (u8_t dice = 0; dice < 3; dice++) {
//             u8_t diceValue = diceValues[dice];
//             if (diceValue == 0) continue;

//             u8_t dicePin = dicePins[dice];
//             u8_t currentMask = diceValueMasks[diceValue - 1];

//             u32_t setMask = 0, clrMask = 0;
            
//             for (u8_t i = 0; i < 4; i++) {
//                 u32_t pinBit = (1 << allPipPins[i]);
//                 if (currentMask & (1 << i)) {
//                     clrMask |= pinBit;
//                 } else {
//                     setMask |= pinBit;
//                 }
//             }

//             GPIO.out_w1ts.val = (1 << dicePin);  // active dice gpio on (n channel on)

//             delayMicroseconds(1); 

//             GPIO.out_w1tc.val = clrMask; // active pips gpio low (p channel on)

//             delayMicroseconds(2); // 2us on (10% duty)

//             GPIO.out_w1ts.val = 0x3C0000; // all pips gpio high (p channel off)

//             delayMicroseconds(18); // 18us of (10% duty)

//             GPIO.out_w1tc.val = (1 << dicePin); // active dice gpio off (n channel off)

//             delayMicroseconds(1);
//         }
//     }
// }