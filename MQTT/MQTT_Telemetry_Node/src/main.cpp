/**************************************************************
 _    _       _       _____      _  _  _             _     _
\ \  / /     (_)     (____ \    | || || |           | |   | |
 \ \/ / ____  _ ____  _   \ \   | || || | ___   ____| | _ | |
  )  ( |    \| |  _ \| |   | |  | ||_|| |/ _ \ / ___) |/ || |
 / /\ \| | | | | | | | |__/ /   | |___| | |_| | |   | ( (_| |
/_/  \_\_|_|_|_|_| |_|_____/     \______|\___/|_|   |_|\____|

***************************************************************
 *! Project    : MQTT Telemetry Node for Test.
 * Purpose    : IoT Connectivity & MQTT Communication
 *              - Connecting ESP32 to Wi-Fi network
 *              - Publishing device status and telemetry via MQTT
 *              - MQTT Last Will and Testament (LWT) implementation
 *              - Retained online/offline status messages
 *              - Automatic Wi-Fi and MQTT reconnection
 *              - Periodic telemetry publishing
 *
 ** Author    : XminD Team (education.xmindworld@gmail.com)
 * Date       : 2026-08-20
 *
 ** XminD Official Channels: https://linkshub.xmindworld.ir
 ***************************************************************/


#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

const char* DEVICE_ID       = "xnode-aero-01";
const char* TOPIC_STATUS    = "xmind/xnode/xnode-aero-01/status";
const char* TOPIC_TELEMETRY = "xmind/xnode/xnode-aero-01/telemetry";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

unsigned long lastMqttReconnectAttempt = 0;
const unsigned long MQTT_RECONNECT_INTERVAL = 5000; 

unsigned long lastTelemetryTime = 0;
const unsigned long TELEMETRY_INTERVAL = 5000; 

unsigned long lastWifiReconnectAttempt = 0;
const unsigned long WIFI_RECONNECT_INTERVAL = 10000;

void setupWiFi() {
  Serial.print("[Wi-Fi] Connecting to ");
  Serial.println(Secrets::WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(Secrets::WIFI_SSID, Secrets::WIFI_PASS);

  unsigned long startAttemptTime = millis();
  const unsigned long WIFI_TIMEOUT = 15000; // 

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\n[Wi-Fi] Connection failed! Restarting ESP...");
    ESP.restart();
  }

  Serial.println("\n[Wi-Fi] Connected successfully!");
  Serial.print("[Wi-Fi] IP Address: ");
  Serial.println(WiFi.localIP());
}

bool reconnectMQTT() {
  unsigned long currentMillis = millis();

  // در بار اول (0) بدون معطلی اجرا می‌شه
  if (lastMqttReconnectAttempt != 0 && (currentMillis - lastMqttReconnectAttempt < MQTT_RECONNECT_INTERVAL)) {
    return false;
  }

  lastMqttReconnectAttempt = currentMillis;
  Serial.println("[MQTT] Attempting connection to broker...");

  const char* lwtPayload = "{\"state\":\"offline\",\"reason\":\"unexpected_disconnect\"}";

  if (mqttClient.connect(DEVICE_ID, Secrets::MQTT_USER, Secrets::MQTT_PASS, TOPIC_STATUS, 1, true, lwtPayload)) {
    Serial.println("[MQTT] Connected successfully!");
    
    const char* onlinePayload = "{\"state\":\"online\",\"firmware\":\"v1.0.4\"}";
    mqttClient.publish(TOPIC_STATUS, onlinePayload, true);
    return true;
  } else {
    Serial.printf("[MQTT] Failed, rc=%d. Will try again in %lu ms.\n", mqttClient.state(), MQTT_RECONNECT_INTERVAL);
    return false;
  }
}

void publishTelemetry() {
  float temperature = 24.6;
  float humidity = 58.2;

  char payloadBuffer[128];
  snprintf(payloadBuffer, sizeof(payloadBuffer), 
           "{\"temp\":%.1f,\"hum\":%.1f}", 
           temperature, humidity);

  boolean success = mqttClient.publish(TOPIC_TELEMETRY, payloadBuffer, false);

  if (success) {
    Serial.print("[Telemetry Published] ");
    Serial.println(payloadBuffer);
  } else {
    Serial.println("[Telemetry Error] Failed to publish message.");
  }
}

void setup() {
  Serial.begin(115200);
  
  setupWiFi();
  mqttClient.setServer(Secrets::MQTT_BROKER, Secrets::MQTT_PORT);
  mqttClient.setBufferSize(512); 
}

void loop() {
  unsigned long currentMillis = millis();

  if (WiFi.status() != WL_CONNECTED) {
    if (currentMillis - lastWifiReconnectAttempt >= WIFI_RECONNECT_INTERVAL) {
      lastWifiReconnectAttempt = currentMillis;
      Serial.println("[Wi-Fi] Connection lost. Attempting to reconnect...");
      WiFi.reconnect();
    }
    return;
  }

  if (!mqttClient.connected()) {
    reconnectMQTT(); 
  } else {
    mqttClient.loop();
  }

  if (currentMillis - lastTelemetryTime >= TELEMETRY_INTERVAL) {
    lastTelemetryTime = currentMillis;
    if (mqttClient.connected()) {
      publishTelemetry();
    }
  }
}
