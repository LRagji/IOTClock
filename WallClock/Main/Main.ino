#include <SPI.h>
#include <WiFiNINA.h>
#include "RTC_SAMD51.h"
#include "DateTime.h"
#include "arduino_secrets.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Protomatter.h>
#include <Fonts/FreeSansBold9pt7b.h>  // Large friendly font

#if defined(ADAFRUIT_FEATHER_M4_EXPRESS) || defined(ADAFRUIT_FEATHER_M0_EXPRESS) || defined(ADAFRUIT_FEATHER_M0) || defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS)
// Configure the pins used for the ESP32 connection
#define SPIWIFI SPI      // The SPI port
#define SPIWIFI_SS 13    // Chip select pin
#define ESP32_RESETN 12  // Reset pin
#define SPIWIFI_ACK 11   // a.k.a BUSY or READY pin
#define ESP32_GPIO0 -1
#endif
#include <WiFiUdp.h>

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };


RTC_SAMD51 rtc;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, 4, NEO_GRB + NEO_KHZ800);
uint8_t rgbPins[] = { 7, 8, 9, 10, 11, 12 };
uint8_t addrPins[] = { 17, 18, 19, 20, 21 };
uint8_t clockPin = 14;
uint8_t latchPin = 15;
uint8_t oePin = 16;
Adafruit_Protomatter matrix(
  64,                         // Matrix width in pixels
  6,                          // Bit depth -- 6 here provides maximum color options
  1, rgbPins,                 // # of matrix chains, array of 6 RGB pins for each
  4, addrPins,                // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin,  // Other matrix control pins
  false);                     // HERE IS THE MAGIC FOR DOUBLE-BUFFERING!

#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;


void setup() {
  //Neopixel
  strip.begin();
  strip.clear();
  strip.show();

  //Start with debug
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  // Matrix panel
  ProtomatterStatus matrixStatus = matrix.begin();
  if (matrixStatus != PROTOMATTER_OK) {
    // DO NOT CONTINUE if matrix setup encountered an error.
    Serial.print("Protomatter begin() encountered error, status: ");
    Serial.println((int)matrixStatus);
    while (true)
      ;
  }
  matrix.setFont(&FreeSansBold9pt7b);
  matrix.fillScreen(0);  // Fill background black

  //RTC
  rtc.begin();
  DateTime now = DateTime(F(__DATE__), F(__TIME__));
  rtc.adjust(now);

  //WIFI
  WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
  // check for the WiFi module:
  int wifiStatus = WiFi.status();
  if (wifiStatus == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  // attempt to connect to Wifi network:
  wifiStatus = WL_IDLE_STATUS;
  while (wifiStatus != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    wifiStatus = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to wifi");
}

byte ntpSyncState = 255;

void loop() {
  DateTime now = rtc.now();
  ntpSyncState = syncWithNTP(now, ntpSyncState);

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);

  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour() - 12, DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  matrix.fillScreen(0);
  matrix.setCursor(0, 32);
  matrix.setTextColor(matrix.color565(10, 10, 10));
  matrix.print(String(now.hour()) + ":" + String(now.minute()));
  matrix.show();

  blink(0, strip.Color(127, 127, 127), 50, 4950);
}
