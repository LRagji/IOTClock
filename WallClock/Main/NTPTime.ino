#define LEAP_YEAR(Y) (((1970 + (Y)) > 0) && !((1970 + (Y)) % 4) && (((1970 + (Y)) % 100) || !((1970 + (Y)) % 400)))
#define IST_TZ_SECONDS 19800
#define tmYearToY2k(Y) ((Y)-30)  // offset is from 2000
#define y2kYearToTm(Y) ((Y) + 30)

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

  } else {
    //Serial.println("6.5");
  }
  return epoch;
}

void syncWithNTP() {
  unsigned long localEpoch = getNTPUnixEpoch(IST_TZ_SECONDS);
  if (localEpoch != IST_TZ_SECONDS) {
    rtc.adjust(DateTime(localEpoch));
    Serial.println("Synced with NTP server...");
  } else {
    Serial.println("Failed to sync with NTP server, will re-attempt later...");
  }
}