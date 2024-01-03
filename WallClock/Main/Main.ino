#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
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


char daysOfTheWeek[7][4] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
char monthsOfYear[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Oct", "Nov", "Dec" };


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
  // Serial.begin(9600);
  // while (!Serial) {
  //   ;  // wait for serial port to connect. Needed for native USB port only
  // }

  // Matrix panel
  ProtomatterStatus matrixStatus = matrix.begin();
  if (matrixStatus != PROTOMATTER_OK) {
    // DO NOT CONTINUE if matrix setup encountered an error.
    Serial.print("Protomatter begin() encountered error, status: ");
    Serial.println((int)matrixStatus);
    while (true)
      ;
  }
  //matrix.setFont(&FreeSansBold9pt7b);
  matrix.fillScreen(0);  // Fill background black
  matrix.show();

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

  // Setup the one minute interupt
  rtc.setAlarm(0, DateTime(0, 0, 0, 0, 0, 0));  // match after every minute
  rtc.enableAlarm(0, rtc.MATCH_SS);
  rtc.attachInterrupt(timeScreenUpdate);
  timeScreenUpdate(0);
}

byte ntpSyncState = 255;

void loop() {
  DateTime now = rtc.now();
  //NTPSYNC
  ntpSyncState = syncWithNTP(now, ntpSyncState);

  //Pulse the Clock Dots
  uint16_t c = now.second() % 2 == 0 ? matrix.color565(10, 10, 10) : matrix.color565(0, 0, 0);
  matrix.fillRect(34, 11, 2, 3, c);
  matrix.fillRect(34, 16, 2, 3, c);

  //Notification
  matrix.setCursor(40, 0);
  matrix.print(now.minute() % 2 == 0 ? daysOfTheWeek[now.dayOfTheWeek()] : monthsOfYear[now.month() - 1]);
  matrix.show();

  blink(0, strip.Color(127, 127, 127), 50, 950);
}

void timeScreenUpdate(uint32_t flag) {
  DateTime now = rtc.now();

  //Hours
  matrix.fillScreen(0);
  matrix.setTextColor(matrix.color565(10, 10, 10));
  matrix.setCursor(0, 3);
  matrix.setTextSize(3);
  String hour = now.hour() > 12 ? String(now.hour() - 12) : String(now.hour());
  String hours = hour.length() == 1 ? (" " + hour) : hour;
  matrix.print(hours);
  //Minutes
  matrix.setCursor(37, 3 + 5);
  matrix.setTextSize(2);
  String minutes = String(now.minute()).length() == 1 ? ("0" + String(now.minute())) : String(now.minute());
  matrix.print(minutes);
  //Date
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.color565(8, 6, 8));
  matrix.setCursor(0, 25);
  String day = String(now.day()).length() == 1 ? ("0" + String(now.day())) : String(now.day());
  String month = String(now.month()).length() == 1 ? ("0" + String(now.month())) : String(now.month());
  matrix.print(day + "/" + month + "/" + String(now.year()));
  matrix.show();
}
