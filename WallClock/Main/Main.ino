#define CRYSTALLESS 1

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
// #include "RTC_SAMD51.h"
// #include "DateTime.h"
#include "RTClib.h"  //Using a DS3231 RTC connected via I2C and Wire lib
#include "arduino_secrets.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Protomatter.h>
// #include <Fonts/FreeSansBold9pt7b.h>  // Large friendly font
#include "frames.h"
// #include <ArduinoJson.h>
#include "arduino_secrets.h"

#if defined(ADAFRUIT_FEATHER_M4_EXPRESS) || defined(ADAFRUIT_FEATHER_M0_EXPRESS) || defined(ADAFRUIT_FEATHER_M0) || defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS)
// Configure the pins used for the ESP32 connection
#define SPIWIFI SPI      // The SPI port
#define SPIWIFI_SS 13    // Chip select pin
#define ESP32_RESETN 12  // Reset pin
#define SPIWIFI_ACK 11   // a.k.a BUSY or READY pin
#define ESP32_GPIO0 -1
#endif

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
//char monthsOfYear[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Oct", "Nov", "Dec" };


RTC_DS3231 rtc;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(31, 4, NEO_GRB + NEO_KHZ800);
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


char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
String alertMessage = "";


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
    while (1) delay(100);
  }

  matrix.fillScreen(0);    // Fill background black
  drawFrame(smart_frame);  //Welcome screen
  matrix.show();

  //RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(100);
  }
  if (rtc.lostPower() == true) {  //Go to internet if RTC has lost its battery power

    //WIFI
    WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
    // check for the WiFi module:
    int wifiStatus = WiFi.status();
    if (wifiStatus == WL_NO_MODULE) {
      Serial.println("Communication with WiFi module failed!");
      // don't continue
      while (1) delay(100);
    }

    //attempt to connect to Wifi network : wifiStatus = WL_IDLE_STATUS;
    while (wifiStatus != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      wifiStatus = WiFi.begin(ssid, pass);
      uint epoch = millis();
      uint8_t counter = 0;
      while (millis() - epoch < 10000) {  // wait 10 seconds for connection:
                                          // matrix.fillScreen(0);             // Fill background black
                                          // matrix.fillRect(0, counter % 32, 5, 7, matrix.color565(0, 10, 0));
                                          // matrix.show();
                                          // counter++;
        // drawFrame(fire_frame_1);
        // drawFrame(fire_frame_2);
        // drawFrame(fire_frame_3);
        // drawFrame(fire_frame_4);
        // drawFrame(fire_frame_5);
        // drawFrame(fire_frame_6);
        // drawFrame(fire_frame_7);
        // drawFrame(fire_frame_8);
        // drawFrame(fire_frame_9);
      }
    }
    //Push for inital sync
    syncWithNTP();

    //Display Message RTC Battery Low
    alertMessage = "Batt Low !";
  }

  // Setup the one minute interupt
  //rtc.setAlarm(0, DateTime(0, 0, 0, 0, 0, 0));  // match after every minute
  //rtc.enableAlarm(0, rtc.MATCH_SS);
  //rtc.attachInterrupt(timeScreenUpdate);
}

byte minute = 255;
uint32_t digitColor = strip.Color(100,10, 0);
uint32_t matrixColor = matrix.color565(255, 165, 0);

void loop() {
  delay(1000);  //Since there is no per second alram that we can set with RTC.

  DateTime now = rtc.now();

  //read_price(0, 0, "MSFT");

  //Pulse dots in between time digits
  strip.setPixelColor(29, now.second() % 2 == 0 ? digitColor : 0);
  strip.setPixelColor(30, now.second() % 2 == 0 ? digitColor : 0);

  //Display Alert Message
  if (alertMessage != "") {
    matrix.setCursor(0, 16);
    matrix.setTextColor(matrix.color565(255, 0, 0));
    matrix.print(alertMessage);
  }

  //Per Minute update
  if (minute != now.minute()) {
    minute = now.minute();

    //Hours
    uint8_t hh = now.hour();
    hh = hh > 12 ? (hh - 12) : (hh == 0 ? 12 : hh);
    if (hh > 9) {
      displayDigit(extractDigit(hh, 10), 3, digitColor);
    } else {
      displayDigit(8, 3, 0);  //Switch off that digit
    }
    displayDigit(extractDigit(hh, 1), 2, digitColor);
    //Minutes
    if (now.minute() > 9) {
      displayDigit(extractDigit(now.minute(), 10), 1, digitColor);
    } else {
      displayDigit(0, 1, digitColor);  //Switch digit to zero
    }
    displayDigit(extractDigit(now.minute(), 1), 0, digitColor);

    //Date
    matrix.fillScreen(0);
    matrix.setTextSize(1);
    matrix.setTextColor(matrixColor);
    matrix.setCursor(2, 0);
    String day = String(now.day()).length() == 1 ? ("0" + String(now.day())) : String(now.day());
    String month = String(now.month()).length() == 1 ? ("0" + String(now.month())) : String(now.month());
    matrix.print(day + "/" + month + "/" + String(now.year()));

    //Display Day of the Week or Month name depending on the minutes.
    matrix.setCursor(40, 0);
    uint16_t h = 0, w = 0;
    getTextHeightWidth(daysOfTheWeek[now.dayOfTheWeek()], &w, &h);
    matrix.setCursor(floor(32 - w / 2), 8);
    matrix.print(daysOfTheWeek[now.dayOfTheWeek()]);

    // //Display Temp
    // float temp = rtc.getTemperature();
    // if (temp > 99.99) {
    //   matrix.print(">99!");
    // } else if (temp < 0.00) {
    //   matrix.print("<0!");
    // } else {
    //   String strTemp = temp < 10 ? ("0" + String(temp * 100)) : String(temp * 100);
    //   matrix.setCursor(32, 25);
    //   matrix.print(strTemp.substring(0, 2));
    //   matrix.drawPixel(44, 31, matrixColor);
    //   matrix.setCursor(46, 25);
    //   matrix.print(strTemp.substring(2, 4));
    //   matrix.drawPixel(58, 25, matrixColor);
    //   matrix.setCursor(58, 25);
    //   matrix.print("c");
    // }

    drawSmallFrame(now.minute(), 15, 16, 16, mario_frame);         // Mario with Time
    matrix.drawFastHLine(0, 31, 64, matrix.color565(91, 64, 55));  // Walking Surface
  }

  //Display
  matrix.show();
  strip.show();
}

void getTextHeightWidth(const char *string, uint16_t *w, uint16_t *h) {
  matrix.setTextWrap(false);
  int16_t x1 = 0, y1 = 0;
  matrix.getTextBounds(string, 0, 0, &x1, &y1, w, h);
  matrix.setTextWrap(true);
}

uint8_t extractDigit(uint8_t number, uint8_t place) {
  return (number / place) % 10;
}