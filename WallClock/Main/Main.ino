#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

#if defined(ADAFRUIT_FEATHER_M4_EXPRESS) || defined(ADAFRUIT_FEATHER_M0_EXPRESS) || defined(ADAFRUIT_FEATHER_M0) || defined(ARDUINO_AVR_FEATHER32U4) || defined(ARDUINO_NRF52840_FEATHER) || defined(ADAFRUIT_ITSYBITSY_M0) || defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS) || defined(ARDUINO_AVR_ITSYBITSY32U4_3V) || defined(ARDUINO_NRF52_ITSYBITSY)
  // Configure the pins used for the ESP32 connection
  #define SPIWIFI SPI      // The SPI port
  #define SPIWIFI_SS 13    // Chip select pin
  #define ESP32_RESETN 12  // Reset pin
  #define SPIWIFI_ACK 11   // a.k.a BUSY or READY pin
  #define ESP32_GPIO0 -1
#endif
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;


void setup() {
  //Start with debug
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to wifi");
  Serial.println("\nStarting connection to server...");
}

void loop() {
  getTime();
  delay(30000);
}
