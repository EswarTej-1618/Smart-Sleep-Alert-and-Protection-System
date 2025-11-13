/*
 * ESP8266 Drowsiness Monitor with REAL Bluetooth & SMS Alerts
 * Merged and updated: correct HTTPS begin, payload, and logging
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include "index.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Wi-Fi
const char* ssid = "Dr.Doom";
const char* password = "1234567890";

// Bluetooth (SoftwareSerial)
#define BT_RX_PIN D5
#define BT_TX_PIN D6
SoftwareSerial btSerial(BT_RX_PIN, BT_TX_PIN);

// Web server and time
ESP8266WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);

// State tracking
String driverStatus = "AWAKE";
unsigned long stateStartTime = 0;
unsigned long sessionStartTime = 0;
int statusChangeCount = 0;
int alertsCount = 0;
bool smsSent = false;
unsigned long sleepStart = 0;

// ======== CircuitDigest Cloud SMS Function ========
void sendSMSAlert(const String &who, const String &status) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠ WiFi Not Connected - Cannot send SMS");
    return;
  }

  std::unique_ptr<WiFiClientSecure> client(new WiFiClientSecure);
  client->setInsecure(); // use with caution; required on many ESP8266 setups
  HTTPClient http;

  // Change template ID here if your drowsiness template uses a different ID
  const char* url = "https://www.circuitdigest.cloud/send_sms?ID=101";

  if (!http.begin(*client, url)) {
    Serial.println("❌ http.begin failed");
    return;
  }

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "9tb1gmGi8dkV");

  // Build payload to match server template: var1 = who, var2 = status/details
  String payload = String("{") +
    "\"mobiles\":\"your_num\"," +
    "\"var1\":\"" + who + "\"," +
    "\"var2\":\"" + status + "\"" +
  "}";

  Serial.print("SMS payload: ");
  Serial.println(payload);

  int httpCode = http.POST(payload);
  String resp = http.getString();
  Serial.printf("HTTP code: %d\n", httpCode);
  Serial.print("Server response: ");
  Serial.println(resp);

  if (httpCode > 0 && (httpCode / 100) == 2) {
    Serial.println("✅ SMS Alert Sent Successfully!");
    alertsCount++;
  } else {
    Serial.println("❌ SMS POST failed");
  }

  http.end();
}

// ======== Web handlers ========
void handleRoot() {
  server.send_P(200, "text/html", MAIN_page);
}

void handleData() {
  StaticJsonDocument<256> json;
  unsigned long now = millis();
  unsigned long stateDuration = (now - stateStartTime) / 1000;
  unsigned long sessionDuration = (now - sessionStartTime) / 1000;

  json["status"] = driverStatus;
  json["stateDuration"] = stateDuration;
  json["sessionDuration"] = sessionDuration;
  json["changes"] = statusChangeCount;
  json["alerts"] = alertsCount;
  json["time"] = timeClient.getFormattedTime();

  String response;
  serializeJson(json, response);
  server.send(200, "application/json", response);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not Found");
}

// ======== Setup ========
void setup() {
  Serial.begin(115200);
  Serial.println("\nStarting Drowsiness Monitor Server (BT + SMS)...");
  btSerial.begin(9600);
  Serial.println("SoftwareSerial for Bluetooth started at 9600 baud.");

  sessionStartTime = millis();
  stateStartTime = millis();

  // ✅ Added: configure D0 as output for signal to Arduino
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW); // start LOW (AWAKE)

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("Server IP Address: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.onNotFound(handleNotFound);

  timeClient.begin();

  server.begin();
  Serial.println("HTTP server started.");
}

// ======== Loop ========
void loop() {
  server.handleClient();
  timeClient.update();

  // Bluetooth input processing
  if (btSerial.available() > 0) {
    char receivedData = btSerial.read();

    if (receivedData == '1' && driverStatus != "SLEEPING") {
      driverStatus = "SLEEPING";
      stateStartTime = millis();
      statusChangeCount++;
      sleepStart = millis();
      smsSent = false;
      Serial.println("Bluetooth Received: '1' -> Status set to SLEEPING");

      // ✅ Set D0 HIGH when sleeping
      digitalWrite(D0, HIGH);
    } 
    else if (receivedData == '0' && driverStatus != "AWAKE") {
      driverStatus = "AWAKE";
      stateStartTime = millis();
      statusChangeCount++;
      sleepStart = 0;
      smsSent = false;
      Serial.println("Bluetooth Received: '0' -> Status set to AWAKE");

      // ✅ Set D0 LOW when awake
      digitalWrite(D0, LOW);
      
    }
  }

  // SMS alert logic: send readable info to server template
  if (driverStatus == "SLEEPING" && !smsSent && sleepStart != 0) {
    if (millis() - sleepStart > 60000) { // > 60 seconds
      // Build friendly values to match template placeholders
      String who = "Driver 1";                       
      String status = "SLEEPING for >60s";          
      sendSMSAlert(who, status);
      smsSent = true;
    }
  }
}
