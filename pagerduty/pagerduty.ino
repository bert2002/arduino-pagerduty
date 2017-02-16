#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <Wire.h>
#include "SSD1306.h"
#include "SSD1306Spi.h"

#include "images.h"

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include <time.h>

int timezone = 3;
int dst = 0;

SSD1306Spi display(D0, D2, D8);

void setup() {
    Serial.begin(115200);
    display.init();
    display.flipScreenVertically();
    display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
    display.display();
    
    WiFiManager wifiManager;
    wifiManager.autoConnect("PAGERDUTY");

    display.setLogBuffer(5, 30);
}

void GetTime() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  while (!time(nullptr)) {
    delay(1000);
  }
  time_t now = time(nullptr) - 7200;
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(65, 50, ctime(&now));
}

void PagerDuty() {
	String PagerDutyDomain = "";
	String PagerDutyToken = "";
	String PagerDutySha1 = "14 D0 B5 B1 42 FB 25 4A D3 72 71 A6 CC 8F 36 FD 9F 53 B5 40";
  String DeviceID = String(ESP.getChipId());
  StaticJsonBuffer<200> jsonBuffer;
  HTTPClient http;
  http.begin("https://" + PagerDutyDomain + ".pagerduty.com/api/v1/incidents/count?status=triggered", PagerDutySha1 );
  http.addHeader("Content-Type","application/json");
  http.addHeader("Authorization","Token token=" + PagerDutyToken);
  int httpCode = http.GET();
  if(httpCode > 0) {
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
      JsonObject& json = jsonBuffer.parseObject(payload);
      if (!json.success()) {
        display.println("Parsing payload failed");
      } else {
        String total = json["total"];
        display.setFont(ArialMT_Plain_24);
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 22, total + " alerts");
      }
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 22, "PagerDuty failed");
  }
  http.end();
}

void loop() {
    display.clear();
    GetTime();
    PagerDuty();
    display.drawLogBuffer(0, 0);
    display.display();
    delay(2000);
}
