#include "ble.h"

void ble::notifyValues(u8_t* diceValues) {
    if (!connected || dieValCharacteristic == nullptr) return;

    dieValCharacteristic->setValue(diceValues, 3);
    dieValCharacteristic->notify();
}

void ble::notifyParam(String key, u8_t value) {
    if (!connected || txCharacteristic == nullptr) return;

    String data = key + "|" + String(value);

    txCharacteristic->setValue(data);
    txCharacteristic->notify();
}

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        BLEDevice::getAdvertising()->stop();
        ble::connected = true;
        ble::lastResponse = millis();
        
        Serial.println("pc connected");
        
    }

    void onDisconnect(BLEServer* pServer) {
        BLEDevice::getAdvertising()->start();
        ble::connected = false;

        Serial.println("pc disconnected");
    }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        String rxValue = pCharacteristic->getValue();
        ble::lastResponse = millis();

        char key[32];
        uint8_t value;
        
        int parsed = sscanf(rxValue.c_str(), "%31[^|]|%hhu", key, &value);
        if (parsed != 2) return;       

        if (strcmp(key, "brightness") == 0) {
            Lights::setBrightness(value);
        }
        
    }
};

void ble::init() {
    BLEDevice::init("ESP-Digital-Dice");
    
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    
    BLEService *pService = pServer->createService(SERVICE_UUID);
    
    dieValCharacteristic = pService->createCharacteristic(
        TX_DIE_VAL,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );
    
    txCharacteristic = pService->createCharacteristic(
        TX_PARAMS,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );

    rxCharacteristic = pService->createCharacteristic(
        RX_PARAMS,
        BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
    );
    rxCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

    pService->start();
    
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMaxPreferred(0x12);
    pAdvertising->start();

    xTaskCreate(watchdogTask, "watchdog", 2048, NULL, 1, NULL);
}

void ble::watchdogTask(void* _) {
    while(true) {
        if (connected) {
            notifyParam("ping", 0);

            if (millis() - lastResponse > HEARTBEAT_TIMEOUT) {
                Serial.println("pc lost");
                BLEDevice::getAdvertising()->stop();
                delay(500);
                BLEDevice::getAdvertising()->start();
                connected = false;
            }
        }
        delay(1000);
    }
    vTaskDelete(NULL);
}