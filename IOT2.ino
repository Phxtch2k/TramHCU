#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// ================= WIFI =================
#define WIFI_SSID "HOME01 2.4g"
#define WIFI_PASSWORD "88888888"

// ================= GOOGLE SCRIPT =================
const char* GAS_HOST = "script.google.com";
const char* GAS_PATH = "/macros/s/AKfycbzHx0cOVUIhgMk1_EuTIsNtFBjv1ZyAIbvNn7Q3Oonaga3YHpMreMYnjcnMIXMXSziHcQ/exec";

// ================= BLE =================
String TARGET_MACS[] = {
  "88:4a:ea:6a:47:15",
  "c8:fd:19:82:fa:50",
  "50:8c:b1:6b:01:0c"
};

String LOCATION_NAMES[] = {
  "ประตูเขียว",
  "โรงโภชนาการ 1",
  "หน้าม."
};

int sensorCount = sizeof(TARGET_MACS) / sizeof(TARGET_MACS[0]);

BLEScan* pBLEScan;
int scanTime = 2;

// ================= STATE =================
String foundStation = "";
int foundRSSI = 0;
bool newData = false;

String lastStation = "";
unsigned long lastSendTime = 0;
const int sendInterval = 5000;

// ================= BLE CALLBACK =================
class MyCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice dev) {

    String currentAddr = String(dev.getAddress().toString().c_str());
    currentAddr.toLowerCase();

    for (int i = 0; i < sensorCount; i++) {
      if (currentAddr == TARGET_MACS[i]) {
        foundStation = LOCATION_NAMES[i];
        foundRSSI = dev.getRSSI();
        newData = true;
        break;
      }
    }
  }
};

// ================= SEND FUNCTION =================
void sendToServer(String station, int rssi) {

  WiFiClientSecure client;
  client.setInsecure(); // 🔥 bypass SSL cert

  Serial.println("\n========== DEBUG ==========");
  Serial.println("Connecting to GAS (HTTPS)...");

  if (!client.connect(GAS_HOST, 443)) {
    Serial.println("❌ Connect failed");
    return;
  }

  String json = "{\"station\":\"" + station + "\",\"rssi\":" + String(rssi) + "}";

  client.println("POST " + String(GAS_PATH) + " HTTP/1.1");
  client.println("Host: script.google.com");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(json.length());
  client.println();
  client.println(json);

  Serial.println("✅ Sent to GAS");
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  // WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\n✅ WiFi Connected");

  // BLE
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyCallbacks(), false);
  pBLEScan->setActiveScan(false);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

// ================= LOOP =================
void loop() {

  Serial.println("🔍 Scanning...");
  pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();

  if (newData) {

    Serial.println("--------------------------------");
    Serial.printf("พบ Beacon: %s | RSSI: %d\n", foundStation.c_str(), foundRSSI);

    if (foundStation != lastStation || millis() - lastSendTime > sendInterval) {

      lastStation = foundStation;
      lastSendTime = millis();

      sendToServer(foundStation, foundRSSI);
    }

    newData = false;
  }

  delay(3000);
}