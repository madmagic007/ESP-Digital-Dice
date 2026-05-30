#include "ble.h"

void ble::notifyValues(u8_t* diceValues) {
    if (!connected || pCharacteristic == nullptr) return;

    pCharacteristic->setValue(diceValues, 3);
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
        TX_DIE_VAL,
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