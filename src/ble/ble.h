#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID "12345678-1234-1234-1234-123456789012"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-210987654321"

class ble {
public:
    static void init();
    static void notifyValues(u8_t* diceValues);
    static inline bool connected = false;

private:
    static inline BLECharacteristic* pCharacteristic = nullptr;
};