void TimeRun()
{
  int seconds = rtc.getSeconds();
  if (seconds == 0)
  {
    seconds = 60;
  }
  if (seconds != lastKnownSecond)
  {
    lastKnownSecond = seconds;
    if (lastKnownSecond % 1 == 0)
    {
      //Run at 1 Sec
      ToggleSecondDots();
      //CheckAlarm();
    }

    if (lastKnownSecond % 60 == 0)
    {
      //Run at Every Minute
      CheckInternet();
      DisplayTimeFrame();
    }
  }
}

void DisplayTimeFrame()
{
  //  tftDisplay.setCursor(0, 0);
  //  tftDisplay.fillScreen(ILI9341_BLACK);
  //  tftDisplay.setTextColor(ILI9341_RED);
  //  tftDisplay.setTextSize(6);
  //  tftDisplay.print(rtc.getHours());
  //  tftDisplay.print(":");
  //  tftDisplay.println(rtc.getMinutes());
  //  tftDisplay.setTextSize(3);
  //  tftDisplay.setTextColor(ILI9341_WHITE);
  //  tftDisplay.print(rtc.getDay());
  //  tftDisplay.print("/");
  //  tftDisplay.print(rtc.getMonth());
  //  tftDisplay.print("/");
  //  tftDisplay.println(rtc.getYear());
  tftDisplay.fillScreen(ILI9341_BLACK);

  int hours = rtc.getHours();
  if (hours > 12)  hours -= 12;
  if (hours == 0)  hours = 12;
  displaySegment(10, 62, ILI9341_GREEN, ILI9341_BLACK, 4, hours  , 2, true , tftDisplay);
  displaySegment(10 + CalculateWidth(4, 88) + 20, 62, ILI9341_GREEN, ILI9341_BLACK, 4, rtc.getMinutes(), 2, false, tftDisplay);
}

void CheckInternet()
{
  String hostName = settings.GetValue(InternetPingHostName);
  int pingResult;
  pingResult = WiFi.ping(hostName);

  if (pingResult < 0) {
    logger.LogWarn(String(rtc.getEpoch()), "Internet Check FAILED! Error code: " + String(pingResult));
    //Show error
  }
}

void ToggleSecondDots()
{
  int x, y, h;
  x = CalculateWidth(4, 88) + 10 + 5;
  h = (CalculateHeight(4, 88) / 4);
  y = (tftDisplay.height() / 2) - h;
  if (rtc.getSeconds() % 2 == 0)
  {
    tftDisplay.fillRect(x, y, 10, 10, ILI9341_BLACK);
    tftDisplay.fillRect(x, y + (h * 2), 10, 10, ILI9341_BLACK);
  }
  else
  {
    tftDisplay.fillRect(x, y, 10, 10, ILI9341_GREEN);
    tftDisplay.fillRect(x, y + (h * 2), 10, 10, ILI9341_GREEN);
  }
}
