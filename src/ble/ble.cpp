#include "ble.h"

void ble::notifyValues(u8_t* diceValues) {
    if (!connected || pCharacteristic == nullptr) return;

    uint8_t packed = (diceValues[0] << 5) | (diceValues[1] << 2) | diceValues[2];

    pCharacteristic->setValue(&packed, 1);
    pCharacteristic->notify();
}

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        BLEDevice::getAdvertising()->stop();
        ble::connected = true;
    }

    void onDisconnect(BLEServer* pServer) {
        BLEDevice::getAdvertising()->start();
        ble::connected = false;
    }
};

void ble::init() {
    BLEDevice::init("ESP32-C6");
    
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    
    BLEService *pService = pServer->createService(SERVICE_UUID);
    
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );
    pService->start();
    
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMaxPreferred(0x12);
    pAdvertising->start();
}