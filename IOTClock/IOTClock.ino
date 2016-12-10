#include <PersistenceDictionary.h>
#include <SDLogger.h>
#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <WiFiUdp.h>
#include <SD.h>
#include <RTCZero.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMonoBold24pt7b.h>

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
#define InternetPingHostName F("PINGH")
#define BUFFPIXEL 20 //Multiplies by 3 RGB

SDLogger logger;
PersistenceDictionary settings;
RTCZero rtc;
Adafruit_ILI9341 tftDisplay = Adafruit_ILI9341(TFT_CS, TFT_DC);
byte lastKnownSecond = 255;


void setup() {
  Serial.begin(115200);
  WaitForSerial(3);
  if (!SD.begin(SD_CS)) {
    Serial.println("Stoppa!!!");
    Stoppa();
  }

  logger.Begin(10, 1000, SD.getUniqueFileName("System/Log", "csv"));
  settings.Begin("User/Set0.csv");
  logger.LogInfo(String(millis()), F("Boot Init"));

  SetScreenBrightness(255);

  tftDisplay.begin();
  tftDisplay.setRotation(3);
  yield();
  tftDisplay.fillScreen(ILI9341_BLACK);
  LoadSplashScreen();
  
  Connect(settings.GetValue(SSIDName), settings.GetValue(SSIDSecret), settings.GetValue(WifiConnectionRetry).toInt(), settings.GetValue(WifiConnectionTimeout).toInt());

  tftDisplay.fillScreen(ILI9341_BLACK);
  tftDisplay.setTextColor(ILI9341_WHITE);
  
  logger.LogInfo(String(millis()), F("Initializing Display completed"));
  SetScreenBrightness(127);
  //tftDisplay.setFont(&FreeMonoBold24pt7b);

  rtc.begin();
  rtc.setEpoch( WiFi.getTime() + 19800); //+5h 30m

  BootComplete();
}

void loop() {
  TimeRun();
  //  logger.LogInfo("Pre", "InfoText");
  //  logger.LogWarn("Pre", "Laukik");
  //  logger.LogError("Pre", "olo");
  //  dic.SetValue("Hello", "World");
  //  dic.GetValue("Hello")

  //  Serial.print("Unix time = ");
  //  Serial.println(rtc.getEpoch());
  //
  //  Serial.print("Seconds since Jan 1 2000 = ");
  //  Serial.println(rtc.getY2kEpoch());
  //
  //  // Print date...
  //  Serial.print(rtc.getDay());
  //  Serial.print("/");
  //  Serial.print(rtc.getMonth());
  //  Serial.print("/");
  //  Serial.print(rtc.getYear());
  //  Serial.print("\t");
  //
  //  // ...and time
  //  print2digits(rtc.getHours());
  //  Serial.print(":");
  //  print2digits(rtc.getMinutes());
  //  Serial.print(":");
  //  print2digits(rtc.getSeconds());
  //
  //  Serial.println();
  //
  //  delay(10000);
}

void print2digits(int number) {
  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number);
}

void BootComplete()
{
  logger.LogInfo(String(rtc.getEpoch()), "Boot completed");
  DisplayTimeFrame();
}

void Stoppa()
{
  logger.LogError(String(millis()), "Processor Halted");
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
    status = WiFi.begin(ssid, secret);
    delay(timeout);
    tries++;
  }
  if (status != WL_CONNECTED & tries >= maxTries)
  {
    logger.LogWarn(String(millis()), "No Network Found");
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

void SetScreenBrightness(byte value)
{
  analogWrite(TFT_LED, 255 - value);
}
