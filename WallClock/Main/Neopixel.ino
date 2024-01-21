
void blink(uint32_t p, uint32_t c, int onTime, int offTime) {
  strip.setPixelColor(p, c);
  strip.show();
  delay(onTime);
  strip.clear();
  strip.show();
  delay(offTime);
}

void displayDigit(byte value, byte digit, uint32_t color) {

  byte antiClockwise[] = { 0, 2, 1, 6, 5, 4, 3, 7 };
  switch (value) {
    case 0:
      strip.setPixelColor((digit * 7) + (digit != 1 ? antiClockwise[1] : 1), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[2] : 2), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[3] : 3), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[4] : 4), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[5] : 5), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[6] : 6), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[7] : 7), 0);
      break;
    case 1:
      strip.setPixelColor((digit * 7) + (digit != 1 ? antiClockwise[1] : 1), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[2] : 2), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[3] : 3), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[4] : 4), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[5] : 5), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[6] : 6), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[7] : 7), 0);
      break;
    case 2:
      strip.setPixelColor((digit * 7) + (digit != 1 ? antiClockwise[1] : 1), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[2] : 2), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[3] : 3), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[4] : 4), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[5] : 5), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[6] : 6), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[7] : 7), color);
      break;
    case 3:
      strip.setPixelColor((digit * 7) + (digit != 1 ? antiClockwise[1] : 1), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[2] : 2), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[3] : 3), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[4] : 4), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[5] : 5), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[6] : 6), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[7] : 7), color);
      break;
    case 4:
      strip.setPixelColor((digit * 7) + (digit != 1 ? antiClockwise[1] : 1), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[2] : 2), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[3] : 3), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[4] : 4), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[5] : 5), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[6] : 6), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[7] : 7), color);
      break;
    case 5:
      strip.setPixelColor((digit * 7) + (digit != 1 ? antiClockwise[1] : 1), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[2] : 2), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[3] : 3), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[4] : 4), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[5] : 5), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[6] : 6), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[7] : 7), color);
      break;
    case 6:
      strip.setPixelColor((digit * 7) + (digit != 1 ? antiClockwise[1] : 1), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[2] : 2), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[3] : 3), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[4] : 4), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[5] : 5), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[6] : 6), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[7] : 7), color);
      break;
    case 7:
      strip.setPixelColor((digit * 7) + (digit != 1 ? antiClockwise[1] : 1), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[2] : 2), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[3] : 3), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[4] : 4), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[5] : 5), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[6] : 6), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[7] : 7), 0);
      break;
    case 8:
      strip.setPixelColor((digit * 7) + (digit != 1 ? antiClockwise[1] : 1), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[2] : 2), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[3] : 3), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[4] : 4), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[5] : 5), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[6] : 6), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[7] : 7), color);
      break;
    case 9:
      strip.setPixelColor((digit * 7) + (digit != 1 ? antiClockwise[1] : 1), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[2] : 2), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[3] : 3), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[4] : 4), 0);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[5] : 5), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[6] : 6), color);
      strip.setPixelColor(digit * 7 + (digit != 1 ? antiClockwise[7] : 7), color);
      break;
  };
  strip.show();
}