#define LEAP_YEAR(Y) (((1970 + (Y)) > 0) && !((1970 + (Y)) % 4) && (((1970 + (Y)) % 100) || !((1970 + (Y)) % 400)))
#define IST_TZ_SECONDS 19800
#define  tmYearToY2k(Y)      ((Y) - 30)    // offset is from 2000
#define  y2kYearToTm(Y)      ((Y) + 30)  

unsigned long getNTPUnixEpoch(int tz) {
  unsigned int localPort = 2390;         // local port to listen for UDP packets
  IPAddress timeServer(129, 6, 15, 28);  // time.nist.gov NTP server
  const int NTP_PACKET_SIZE = 48;        // NTP time stamp is in the first 48 bytes of the message
  byte packetBuffer[NTP_PACKET_SIZE];    //buffer to hold incoming and outgoing packets
  WiFiUDP Udp;
  Udp.begin(localPort);

  sendNTPpacket(timeServer, packetBuffer, NTP_PACKET_SIZE, &Udp);
  delay(1000);  // wait to see if a reply is available
  unsigned long epoch = readNTPpacket(packetBuffer, NTP_PACKET_SIZE, &Udp);

  Udp.stop();
  return epoch += tz;
}

void sendNTPpacket(IPAddress& address, byte packetBuffer[], byte NTP_PACKET_SIZE, WiFiUDP* Udp) {
  //Serial.println("1");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  //Serial.println("2");
  packetBuffer[0] = 0b11100011;  // LI, Version, Mode
  packetBuffer[1] = 0;           // Stratum, or type of clock
  packetBuffer[2] = 6;           // Polling Interval
  packetBuffer[3] = 0xEC;        // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  //Serial.println("3");
  Udp->beginPacket(address, 123);  //NTP requests are to port 123
  //Serial.println("4");
  Udp->write(packetBuffer, NTP_PACKET_SIZE);
  //Serial.println("5");
  Udp->endPacket();
  //Serial.println("6");
}

unsigned long readNTPpacket(byte packetBuffer[], byte NTP_PACKET_SIZE, WiFiUDP* Udp) {
  unsigned long epoch = 0;
  if (Udp->parsePacket()) {
    //Serial.println("7");
    // We've received a packet, read the data from it
    Udp->read(packetBuffer, NTP_PACKET_SIZE);  // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    //Serial.print("Seconds since Jan 1 1900 = ");
    //Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    //Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    epoch = secsSince1900 - seventyYears;
    // print Unix time:
    //Serial.println(epoch);


    // print the hour, minute and second:
    // Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    // Serial.print((epoch % 86400L) / 3600);  // print the hour (86400 equals secs per day)
    // Serial.print(':');
    // if (((epoch % 3600) / 60) < 10) {
    //   // In the first 10 minutes of each hour, we'll want a leading '0'
    //   Serial.print('0');
    // }
    // Serial.print((epoch % 3600) / 60);  // print the minute (3600 equals secs per minute)
    // Serial.print(':');
    // if ((epoch % 60) < 10) {
    //   // In the first 10 seconds of each minute, we'll want a leading '0'
    //   Serial.print('0');
    // }
    // Serial.println(epoch % 60);  // print the second
  } else {
    //Serial.println("6.5");
  }
  return epoch;
}

byte syncWithNTP(DateTime now, byte elapsedState) {
  byte currentGap = now.minute();
  currentGap -= (currentGap % 10);  //Sync is 10 mins
  if (currentGap != elapsedState) {
    elapsedState = currentGap;
    unsigned long localEpoch = getNTPUnixEpoch(IST_TZ_SECONDS);
    if (localEpoch != IST_TZ_SECONDS) {
      rtc.adjust(DateTime(localEpoch));
      Serial.println("Synced with NTP server...");
    } else {
      Serial.println("Failed to sync with NTP server, will re-attempt later...");
    }
  }
  return elapsedState;
}

//DateTime parseDateTime(unsigned long timeInput) {
  // break the given time_t into time components
  // this is a more compact version of the C library localtime function
  // note that year is offset from 1970 !!!

  // uint8_t monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };  // API starts months from 1, this array starts from 0

  // uint8_t year;
  // uint8_t month, monthLength;
  // uint32_t time;
  // unsigned long days;

  // time = (uint32_t)timeInput;
  // const int Second = time % 60;
  // time /= 60;  // now it is minutes
  // const int Minute = time % 60;
  // time /= 60;  // now it is hours
  // const int Hour = time % 24;
  // time /= 24;  // now it is days
  // const int Wday = ((time + 4) % 7) + 1;  // Sunday is day 1

  // year = 0;
  // days = 0;
  // while ((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
  //   year++;
  // }
  // const int Year = tmYearToY2k(year);  // year is offset from 1970

  // days -= LEAP_YEAR(year) ? 366 : 365;
  // time -= days;  // now it is days in this year, starting at 0

  // days = 0;
  // month = 0;
  // monthLength = 0;
  // for (month = 0; month < 12; month++) {
  //   if (month == 1) {  // february
  //     if (LEAP_YEAR(year)) {
  //       monthLength = 29;
  //     } else {
  //       monthLength = 28;
  //     }
  //   } else {
  //     monthLength = monthDays[month];
  //   }

  //   if (time >= monthLength) {
  //     time -= monthLength;
  //   } else {
  //     break;
  //   }
  // }
  // const int Month = month + 1;  // jan is month 1
  // const int Day = time + 1;     // day of month
  // return DateTime(Year, Month, Day, Hour, Minute, Second);
//}

