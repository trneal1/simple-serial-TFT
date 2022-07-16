/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include "wire.h"

#include <ESP8266Wifi.h>
#include <ESP8266mDNS.h>

const char *ssid = "TRNNET-2G";
const char *password = "ripcord1";

#include <WiFiUDP.h>

WiFiUDP Udp;

WiFiServer Tcp(8001);

// For the Adafruit shield, these are the default.
#define TFT_DC D1
#define TFT_CS D8

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void connect()
{
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println(WiFi.localIP());
  Udp.begin(8001);
  Tcp.begin();
}

void check_net()
{

  static char buffer[30][81] = {0};

  static WiFiClient client;

  if (!client)
  {
    client = Tcp.available();
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);

    tft.setTextSize(2);
    tft.print("TCP disconnected");
    delay(2000);
  }

  if (client)
  {

    int test = client.available();
    if (test > 0)
    {

      for (int i = 29; i >= 1; i--)
      {
        strcpy(buffer[i], buffer[i - 1]);
      }
      memset(buffer[0], 0, 81);
      // client.read(buffer[0],80);

      client.readBytesUntil('#', buffer[0], 80);

      Serial.println("characters avail");
      Serial.println(test);
      Serial.println(buffer[0]);

      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(0, 0);
      tft.setTextSize(1);
      for (int i = 0; i <= 29; i++)
        tft.print(buffer[i]);
    }
  }
}

void setup()
{
  Serial.begin(9600);

  connect();

  SPI.setFrequency(40000000);
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(3);
  tft.setCursor(0, 0);

  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.println(WiFi.localIP());

  delay(5000);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
}

void loop(void)
{
  check_net();
}
