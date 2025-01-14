#pragma once

#include <Arduino.h>

#define ELEGANTOTA_USE_ASYNC_WEBSERVER 1
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <WiFi.h>

#include "security.h"
String hostname = "ESP_Irrigation";

AsyncWebServer server(80);

/*
    Will connect to Wifi and will prepare to update the code
    throught OTA (Over the Air)

    - create a file called "security.h" in this directory, do not save it
       to the repo
        - file content:
            const  char* ssid = "your_wifi_ssid"
            const  char* password = "your_wifi_password"
*/

class MyWifi {
 private:
 public:
  MyWifi() {};
  ~MyWifi() {};
  void init();
  void loop();
  void show(Adafruit_SSD1306* d);
};

void MyWifi::init() {
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname.c_str());
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.dnsIP());
  Serial.println(WiFi.macAddress());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "Hello, goto /update");
  });

  ElegantOTA.begin(&server);  // Start ElegantOTA

  server.begin();
  Serial.println("HTTP server started");
}

void MyWifi::loop() { ElegantOTA.loop(); }

void MyWifi::show(Adafruit_SSD1306* d) {
  d->clearDisplay();
  d->setTextColor(SSD1306_WHITE);
  d->setCursor(0, 0);
  d->print("IP = ");
  d->println(WiFi.localIP());
  d->setCursor(0, 17);
  d->println("mac = ");
  d->println(WiFi.macAddress());
  d->println("Gateway:");
  d->println(ssid);
  d->println(WiFi.gatewayIP());
  d->println(WiFi.getHostname());
  d->display();
  return;
}
// void WiFiInit()
// {
//     WiFi.mode(WIFI_STA);
//     WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
//     WiFi.setHostname(hostname);
//     WiFi.begin(ssid, password);

//     while (WiFi.status() != WL_CONNECTED)
//     {
//         delay(500);
//         Serial.print(".");
//     }
//     Serial.println("");
//     Serial.print("Connected to ");
//     Serial.println(ssid);
//     Serial.print("IP address: ");
//     Serial.println(WiFi.localIP());
//     Serial.println(WiFi.dnsIP());
//     Serial.println(WiFi.macAddress());

//     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
//               { request->send(200, "text/plain", "Hello, goto /update"); });

//     ElegantOTA.begin(&server); // Start ElegantOTA

//     server.begin();
//     Serial.println("HTTP server started");
// }

// void OTAloop()
// {
//     ElegantOTA.loop();
// }