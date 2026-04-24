#include <Arduino.h>
#include <NimBLEDevice.h>
#include "uuid_defs.h"

// Constants
const int LED_PIN = 2; // Built-in LED on most ESP32 dev boards

// Forward declarations
class MyServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer) {
        Serial.println("Device connected");
    }
    void onDisconnect(NimBLEServer* pServer) {
        Serial.println("Device disconnected");
        NimBLEDevice::startAdvertising();
    }
};

class DriveCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        Serial.print("Drive Control Received: ");
        Serial.println(value.c_str());

        if (value == "GO") {
            digitalWrite(LED_PIN, HIGH);
            Serial.println("Action: LED ON");
        } else if (value == "STOP") {
            digitalWrite(LED_PIN, LOW);
            Serial.println("Action: LED OFF");
        }
    }
};

class TelemetryCallbacks : public NimBLECharacteristicCallbacks {
    void onRead(NimBLECharacteristic* pCharacteristic) {
        uint32_t uptime = millis() / 1000;
        pCharacteristic->setValue(uptime);
        Serial.printf("Telemetry Uptime Read: %u seconds\n", uptime);
    }
};

// Global variables
NimBLECharacteristic* pTelemetryDataChar;

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    Serial.println("Starting BLE GATT Structure Lab...");

    // 1. Initialize BLE Device
    NimBLEDevice::init("ESP32-GATT-Lab");

    // 2. Create Server
    NimBLEServer* pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // 3. Create Drive Service (動力服務)
    NimBLEService* pDriveService = pServer->createService(DRIVE_SERVICE_UUID);
    NimBLECharacteristic* pDriveControlChar = pDriveService->createCharacteristic(
        DRIVE_CONTROL_CHAR_UUID,
        NIMBLE_PROPERTY::WRITE
    );
    pDriveControlChar->setCallbacks(new DriveCallbacks());
    pDriveService->start();

    // 4. Create Telemetry Service (遙測服務)
    NimBLEService* pTelemetryService = pServer->createService(TELEMETRY_SERVICE_UUID);
    
    // Uptime Characteristic (Read)
    NimBLECharacteristic* pUptimeChar = pTelemetryService->createCharacteristic(
        TELEMETRY_UPTIME_CHAR_UUID,
        NIMBLE_PROPERTY::READ
    );
    pUptimeChar->setCallbacks(new TelemetryCallbacks());

    // Data Characteristic (Notify)
    pTelemetryDataChar = pTelemetryService->createCharacteristic(
        TELEMETRY_DATA_CHAR_UUID,
        NIMBLE_PROPERTY::NOTIFY
    );
    // Add BLE2902 Descriptor for Notify (Required for many clients)
    pTelemetryDataChar->createDescriptor("2902", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
    
    pTelemetryService->start();

    // 5. Start Advertising
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(DRIVE_SERVICE_UUID);
    pAdvertising->addServiceUUID(TELEMETRY_SERVICE_UUID);
    pAdvertising->start();

    Serial.println("GATT Server is ready and advertising.");
}

void loop() {
    static unsigned long lastNotify = 0;
    if (millis() - lastNotify > 500) { // Every 500ms
        lastNotify = millis();
        
        if (pTelemetryDataChar->getSubscribedCount() > 0) {
            int virtualSensorValue = random(0, 100);
            pTelemetryDataChar->setValue(virtualSensorValue);
            pTelemetryDataChar->notify();
            // Serial.printf("Notified: %d\n", virtualSensorValue);
        }
    }
}
