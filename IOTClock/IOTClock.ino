#include <PersistenceDictionary.h>
#include <SDLogger.h>
#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <WiFiUdp.h>
#include <SD.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMonoBold12pt7b.h>

#define TFT_DC 12
#define TFT_CS 7
#define SD_CS 6
#define TFT_LED 5
#define BUZZ 4
#define SSIDName F("SSID")
#define SSIDSecret F("SSIDP")
#define SSIDNameBackup F("SSIDB")
#define SSIDSecretBackup F("SSIDPB")
#define WifiConnectionRetry F("WIFIR")
#define WifiConnectionTimeout F("WIFIT")

SDLogger logger;
PersistenceDictionary settings;
Adafruit_ILI9341 tftDisplay = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
  Serial.begin(115200);
  WaitForSerial(3);
  if (!SD.begin(SD_CS)) {
    Serial.println("Stoppa!!!");
    Stoppa();
  }

  logger.Begin(10, 1000, GetUniqueFileName("System/Log", "csv"));
  settings.Begin("User/Set0.csv");
  logger.LogInfo(String(millis()), F("System Init"));
  SetScreenBrightness(0);
  Connect(settings.GetValue(SSIDName), settings.GetValue(SSIDSecret), settings.GetValue(WifiConnectionRetry).toInt(), settings.GetValue(WifiConnectionTimeout).toInt());

  tftDisplay.begin();
  tftDisplay.setRotation(3);
  tftDisplay.fillScreen(ILI9341_BLACK);
  tftDisplay.setFont(&FreeMonoBold12pt7b);
  tftDisplay.setTextColor(ILI9341_WHITE);
  yield();
  logger.LogInfo(String(millis()), F("Initializing Display completed"));
  SetScreenBrightness(255);
  tftDisplay.setTextSize(3);
  tftDisplay.setCursor(0, 0);
  tftDisplay.print("Hello World!!");
}

void loop() {
  //  logger.LogInfo("Pre", "InfoText");
  //  logger.LogWarn("Pre", "Laukik");
  //  logger.LogError("Pre", "olo");
  //  dic.SetValue("Hello", "World");
  //  dic.GetValue("Hello")
}

void Stoppa()
{
  while (true) {
    yield;
  };
}

void WaitForSerial(byte maxTries)
{
  byte retries = 0;
  while (!Serial & retries < maxTries)
  {
    delay(1000);
    retries++;
  }
}

void Connect(String ssid, String secret, int maxTries, int timeout)
{
  int status = 0;
  byte tries = 0;
  while ( status != WL_CONNECTED & tries < maxTries) {
    logger.LogInfo(String(millis()), "Attempting to connect to SSID: " + ssid + " " + String(tries));
    Serial.println(ssid);
    status = WiFi.begin(ssid, secret);
    delay(timeout);
    tries++;
  }
  if (status != WL_CONNECTED & tries >= maxTries)
  {
    logger.LogInfo(String(millis()), "No Network Found");
    if (settings.GetValue(SSIDNameBackup) != ssid)
    {
      Connect(settings.GetValue(SSIDNameBackup), settings.GetValue(SSIDSecretBackup), settings.GetValue(WifiConnectionRetry).toInt(), settings.GetValue(WifiConnectionTimeout).toInt());
    }
    else
    {
      Stoppa();
    }
  }
  else
  {
    IPAddress localAddr = WiFi.localIP();
    byte oct1 = localAddr[0];
    byte oct2 = localAddr[1];
    byte oct3 = localAddr[2];
    byte oct4 = localAddr[3];
    char s[16];
    sprintf(s, "%d.%d.%d.%d", oct1, oct2, oct3, oct4);
    logger.LogInfo(String(millis()), "SSID: " + ssid + " " + String(s) + " " + WiFi.RSSI() + "dBm");
  }
}

String GetUniqueFileName(String expectedName, String expectedExtention)
{
  long counter = 0;
  while (SD.exists(expectedName + String(counter) + "." + expectedExtention))
  {
    counter += 1;
  }
  return expectedName  + String(counter) + "." + expectedExtention;
}

void SetScreenBrightness(byte value)
{
  analogWrite(TFT_LED, 255 - value);
}
