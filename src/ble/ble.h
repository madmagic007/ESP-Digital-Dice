#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID    "12345678-1234-1234-1234-123456789012"
#define TX_DIE_VAL      "11111111-1111-1111-1111-111111111111"
#define TX_PARAMS       "22222222-2222-2222-2222-222222222222"
#define RX_PARAMS       "33333333-3333-3333-3333-333333332222"

class ble {
public:
    static void init();
    static void notifyValues(u8_t* diceValues);
    static inline bool connected = false;

private:
    static inline BLECharacteristic* pCharacteristic = nullptr;
};