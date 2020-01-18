#include <Arduino.h>
#include <Wire.h> 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <Ticker.h>
#include "ArduinoJson.h"
#include "Display.h"

//SSID of your network 
const char* ssid = "Kitty";
//password of your Network 
const char* pass = "meeoooow";

const uint8_t lcd_rows = 4;
const uint8_t lcd_cols = 20; 
Display lcd(0x27, lcd_cols, lcd_rows);   // init the LCD
const char* lcd_data[lcd_rows];
int lcd_data_length[lcd_rows];

ESP8266WebServer server(80);        // init Web server
Ticker timer;                       // init timer

void connectWiFi() 
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  lcd.setCursor(0,0);
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
  //lcd.print(WiFi.localIP());
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
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, server.arg(0));

  // Return bad request if parsing failed
  if (error) 
  {
    Serial.println("JSON Parsing failed");
    server.send(400, "");
    return;
  }

  lcd.clear();

  for (int i = 0; i < lcd_rows; i++)
  {
    lcd.setCursor(0,i);
    const char* text = doc["data"][i]["text"];
    lcd.print(text);
    Serial.println(text);
    lcd_data[i] = text;
    lcd_data_length[i] = strlen(text);
  }

  server.send(204, "");
}

void scroll()
{
  Serial.println("Scroll called");
  for (uint8_t i = 0; i < lcd_rows; i++)
  {
    Serial.println("In main for loop");
    Serial.println(strlen(lcd_data[i]));
    Serial.println("checking data length");
    if(strlen(lcd_data[i]) > 0)
    {
      Serial.println("condition okay");
      for (uint8_t j = 0; j < lcd_cols; j++)
      {
        lcd.setCursor(j, i);
        lcd.print(" ");
      }     
      lcd.setCursor(0, i);
      lcd.printf("%.20s", lcd_data[i]);
      lcd_data[i]++;
    }
    else
    {
      lcd_data[i] = lcd_data[i] - lcd_data_length[i];
    }
    Serial.println("main for loop ended");
  }
  Serial.println("Scroll ended");
}

void setup()
{

  Serial.begin(9600);
  Serial.println("Remote Network Display");

  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  connectWiFi();

  server.on("/display", display);
  server.begin();

  timer.attach(2, scroll);

  lcd_data[0] = "Line 0";
  lcd_data[1] = "Line 1";
  lcd_data[2] = "Line 2";
  lcd_data[3] = "Line 3";

  lcd_data_length[0] = 6;
  lcd_data_length[1] = 6;
  lcd_data_length[2] = 6;
  lcd_data_length[3] = 6;
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  server.handleClient();  
}

