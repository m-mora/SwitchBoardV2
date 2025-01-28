#pragma once

#include <Arduino.h>

#define ELEGANTOTA_USE_ASYNC_WEBSERVER 1
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define MSG_ACTION 0x8000
#define MSG_NO_ACTION 0x0000
#define ZONE1ON 0x0001
#define ZONE2ON 0x0002
#define ZONE3ON 0x0004
#define ZONE4ON 0x0008
#define ZONE1OFF 0x0010
#define ZONE2OFF 0x0020
#define ZONE3OFF 0x0040
#define ZONE4OFF 0x0080
#define MSG_EXIT 0x0100

#include "security.h"
String hostname = "ESP_Irrigation";

AsyncWebServer server(80);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(telegramToken, secured_client);

/*
    Will connect to Wifi and will prepare to update the code
    throught OTA (Over the Air)

    - create a file called "security.h" in this directory, do not save it
       to the repo
        - file content:
            const  char* ssid = "your_wifi_ssid"
            const  char* password = "your_wifi_password"
*/

class MyWifi
{
private:
  uint16_t msgActions = 0;
  void _newMessage(int numNewMessages);
  void _call_query(int i, int numMessages);
  void _options(String chat_id);
  void _start(String chat_id, String from_name);
  void _menu(String chat_id, String from_name);

public:
  MyWifi() {};
  ~MyWifi() {};
  void init();
  uint16_t loop();
  void show(Adafruit_SSD1306 *d);
  uint16_t _update_telegram();
};

void MyWifi::init()
{
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname.c_str());
  WiFi.begin(ssid, password);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  while (WiFi.status() != WL_CONNECTED)
  {
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

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "Hello, goto /update"); });

  ElegantOTA.begin(&server); // Start ElegantOTA

  server.begin();
  Serial.println("HTTP server started");
  bot.sendMessage(myTelegramID, "On Line");
  this->_start(myTelegramID,"");
  // bot.sendMessage(myTelegramID, "Type /start ");
}

uint16_t MyWifi::loop()
{
  ElegantOTA.loop();
  _update_telegram();
  return this->msgActions;
}

void MyWifi::show(Adafruit_SSD1306 *d)
{
  d->clearDisplay();
  d->setTextColor(SSD1306_WHITE);
  d->setTextSize(1);
  d->setCursor(0, 0);
  d->print("IP = ");
  d->println(WiFi.localIP());
  d->setCursor(0, 16);
  d->println("mac = ");
  d->println(WiFi.macAddress());
  d->println("Gateway:");
  d->println(ssid);
  d->println(WiFi.gatewayIP());
  d->println(WiFi.getHostname());
  d->display();
  return;
}

void MyWifi::_newMessage(int numNewMessages)
{
  for (int i = 0; i < numNewMessages; i++)
  {
    String from_name = bot.messages[i].from_name;
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    if (chat_id != myTelegramID)
    {
      bot.sendMessage(chat_id, "Usuario no autorizado");
    }

    // Inline buttons with callbacks when pressed will raise a callback_query message
    if (bot.messages[i].type == "callback_query")
    {
      this->_call_query(i, numNewMessages);
    }
    else
    {
      if (text == "/menu")
      {
        this->_menu(chat_id,from_name);
      }
      if (text == "/options")
      {
        this->_options(chat_id);
      }

      if (text == "/start")
      {
        this->_start(chat_id, from_name);
      }

      if (text == "/ip")
      {
        bot.sendMessage(chat_id, WiFi.localIP().toString());
      }
    }
  }
}

uint16_t MyWifi::_update_telegram()
{
  this->msgActions = 0;
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  while (numNewMessages)
  {
    // Serial.println("got response");
    this->_newMessage(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
  return this->msgActions;
}

void MyWifi::_call_query(int i, int numNewMessages)
{
  String msg;
  msg = bot.messages[i].text;
  // Serial.print("Call back button pressed by: ");
  // Serial.print(bot.messages[i].from_id);
  // Serial.printf(" - %s\n", bot.messages[i].from_name);
  // Serial.print("Data on the button: ");
  // Serial.println(msg);
  bot.sendMessage(bot.messages[i].from_id, msg, "");
  if (msg == "TurnOn Zone1")
  {
    this->msgActions = MSG_ACTION | ZONE1ON;
  }
  else if (msg == "TurnOn Zone2")
  {
    this->msgActions = MSG_ACTION | ZONE2ON;
  }
  else if (msg == "TurnOn Zone3")
  {
    this->msgActions = MSG_ACTION | ZONE3ON;
  }
  else if (msg == "TurnOn Zone4")
  {
    this->msgActions = MSG_ACTION | ZONE4ON;
  }
  else if (msg == "TurnOff Zone1")
  {
    this->msgActions = MSG_ACTION | ZONE1OFF;
  }
  else if (msg == "TurnOff Zone2")
  {
    this->msgActions = MSG_ACTION | ZONE2OFF;
  }
  else if (msg == "TurnOff Zone3")
  {
    this->msgActions = MSG_ACTION | ZONE3OFF;
  }
  else if (msg == "TurnOff Zone4")
  {
    this->msgActions = MSG_ACTION | ZONE4OFF;
  }
  else if (msg == "Exit")
  {
    this->msgActions = MSG_ACTION | MSG_EXIT;
  }
}

void MyWifi::_options(String chat_id)
{
  String keyboardJson = "[[{ \"text\" : \"Turn On Zone 1\", \"callback_data\" : \"TurnOn Zone1\" }], \
                               [{ \"text\" : \"Turn Off Zone 1\", \"callback_data\" : \"TurnOff Zone1\" }], \
                               [{ \"text\" : \"Turn On Zone 2\", \"callback_data\" : \"TurnOn Zone2\" }],[ \
                               { \"text\" : \"Turn Off Zone 2\", \"callback_data\" : \"TurnOff Zone2\" }], \
                               [{ \"text\" : \"Turn On Zone 3\", \"callback_data\" : \"TurnOn Zone3\" }], \
                               [{ \"text\" : \"Turn Off Zone 3\", \"callback_data\" : \"TurnOff Zone3\" }], \
                               [{ \"text\" : \"Turn On Zone 4\", \"callback_data\" : \"TurnOn Zone4\" }], \
                               [{ \"text\" : \"Turn Off Zone 4\", \"callback_data\" : \"TurnOff Zone4\" }], \
                               [{ \"text\" : \"Exit\", \"callback_data\" : \"Exit\" }]]";
  bot.sendMessageWithInlineKeyboard(chat_id, "Selecciona una de las siguientes opciones\n Selecciona *Exit* al terminar", "Markdown", keyboardJson);
}

void MyWifi::_start(String chat_id, String from_name)
{
  String welcome = "Bienvenido al sistema de SwitchV2, " + from_name + ".\n";
  welcome += "/menu : para ver las opciones\n";

  bot.sendMessage(chat_id, welcome, "Markdown");
}

void MyWifi::_menu(String chat_id, String from_name)
{
  String msg = "Teclea la *opcion* deseada:\n";
  msg += "/options : interaccion con los switches\n";
  msg += "/ip : IP de la tarjeta\n";
  msg += "/start : Mensaje de bienvenida\n";
  msg += "/menu : este menu\n";

  bot.sendMessage(chat_id, msg, "Markdown");
}