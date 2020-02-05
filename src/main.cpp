#include <Arduino.h>
#include <Wire.h> 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <Ticker.h>
#include "ArduinoJson.h"
#include "Display.h"

//SSID of your network 
const char* ssid = "your-wifi-name";
//password of your Network 
const char* pass = "your-wifi-password";

uint8_t lcd_cols = 20; 
uint8_t lcd_rows = 4;
Display lcd(0x27, lcd_cols, lcd_rows);   // init the LCD

StaticJsonDocument<1024> doc;
ESP8266WebServer server(80);        // init Web server
Ticker timer;                       // init timer

void connectWiFi() 
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    lcd.clear();
    lcd.print("Connecting WiFi");
    delay(1000);
    lcd.clear();
    lcd.print("Connecting WiFi.");
    delay(1000);
  }
  lcd.clear();
  lcd.print("IP address:");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
}

// Parse the request and display on lcd
void display() 
{
  Serial.println(server.arg(0));

  // Request does not have any body
  if (!server.args())
  {
    Serial.println("Request does not have a body");
    server.send(400, "");
    return;
  }

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, server.arg(0));

  // Return bad request if parsing failed
  if (error) 
  {
    Serial.println("JSON Parsing failed");
    server.send(400, "");
    return;
  }

  // enable / disable scroll
  if (doc["scroll"] == 0 || doc["scroll"] == 1)
  {
    lcd.setScroll(doc["scroll"]);
  }
  else
  {
    Serial.println("Invalid scroll value");
    server.send(400, "");
    return;
  }

  // print data on each lcd row
  for (uint8_t i = 0; i < lcd_rows; i++)
  {
    const char* text = doc["data"][i]["text"];
    if (text != NULL)
    {
      lcd.printText(i, text, doc["data"][i]["align"]);
      Serial.println(text);
    }
  }
  
  doc.clear();
  server.send(204, "");
}

// API endpoint for clearing the display
void clear()
{
  Serial.println("clear display request received");
  lcd.clear();
  server.send(204, "");
}

void scroll()
{
  lcd.scroll();
}

void setup()
{

  Serial.begin(9600);

  lcd.init();                      // initialize the lcd 
  lcd.backlight();

  timer.attach(0.5, scroll);

  lcd.printText(0, "Network Char LCD");
  lcd.printText(1, "v 1.0");

  delay(2000);

  connectWiFi();

  server.on("/display", display);
  server.on("/clear", clear);
  server.begin();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  server.handleClient();  
}

