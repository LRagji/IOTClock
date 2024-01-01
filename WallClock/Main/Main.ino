#include <SPI.h>
#include <WiFiNINA.h>
#include "RTC_SAMD51.h"
#include "DateTime.h"
#include "arduino_secrets.h"

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

#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;


void setup() {
  //Start with debug
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  rtc.begin();
  DateTime now = DateTime(F(__DATE__), F(__TIME__));
  rtc.adjust(now);

  WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
  // check for the WiFi module:
  int status = WiFi.status();
  if (status == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  // attempt to connect to Wifi network:
  status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

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

  delay(3000);
}
