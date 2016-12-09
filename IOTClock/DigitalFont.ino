
byte Segment1Height = 10;
byte Segment1Width = 3;
int CalculateHeight(byte scaleFactor, int value)
{
  int segmentHeight = Segment1Height * scaleFactor; //Segment 1 height
  int segmentWidth = Segment1Width * scaleFactor; //Segment 1 height
  if (value < 0) value = value * -1;
  return segmentWidth + segmentHeight + segmentWidth + segmentHeight + segmentWidth;
}

int CalculateWidth(byte scaleFactor, int value)
{
  int segmentHeight = Segment1Height * scaleFactor; //Segment 1 height
  int segmentWidth = Segment1Width * scaleFactor; //Segment 1 height
  if (value < 0) value = value * -1;
  int widthOfSingleChar = segmentWidth + segmentHeight + segmentWidth;
  return ((widthOfSingleChar +  segmentWidth) * ((String)value).length()) - segmentWidth;
}

void displaySegment(int startX, int startY, uint16_t textColor, uint16_t backColor, byte scaleFactor, int value, byte fixedNumberOfDigits, bool showZeroDigits, Adafruit_ILI9341 &tft)
{
  int fheight = Segment1Height * scaleFactor; //Segment 1 height
  int fwidth = Segment1Width * scaleFactor; //Segment 1 height
  int widthOfSingleChar = fwidth + fheight + fwidth;
  //int heightOfSingleChar = fwidth + fheight + fwidth + fheight + fwidth;
  int space = fwidth;

  if (value < 0) value = value * -1;
  String strValue = (String)value;
  byte disableDigitCount = 0;
  while (fixedNumberOfDigits > strValue.length())
  {
    disableDigitCount++;
    strValue = "0" + strValue;
  }
  byte multiplier = 0;
  while (multiplier < fixedNumberOfDigits)
  {
    draw7Segment(startX + ((widthOfSingleChar + space) * multiplier), startY, textColor, backColor, fheight, fwidth, strValue[multiplier] - 48, !((multiplier < disableDigitCount)& showZeroDigits), tft);
    multiplier++;
  }
}

void draw7Segment(int startX, int startY, uint16_t textColor, uint16_t backColor, int fheight, int fwidth, byte value, bool displayDigit, Adafruit_ILI9341 &tft)
{
  //int fheight = 10 * scaleFactor; //Segment 1 height
  //int fwidth = 3 * scaleFactor; //Segment 1 height

  //tft.drawFastHLine(startX, startY, fwidth + fheight + fwidth, RED);
  //tft.drawFastVLine(startX, startY, fwidth + fheight + fwidth + fheight + fwidth, RED);

  if ((value == 0 || value == 4 || value == 5 || value == 6 || value == 8 || value == 9) & displayDigit)
    tft.fillRect(startX, startY + fwidth, fwidth, fheight, textColor); //Segment 1
  else
    tft.fillRect(startX, startY + fwidth, fwidth, fheight, backColor); //Segment 1

  if ((value == 0 || value == 2 || value == 3 || value == 5 || value == 6 || value == 7 || value == 8 || value == 9)& displayDigit)
    tft.fillRect(startX + fwidth, startY, fheight, fwidth, textColor); //Segment 2
  else
    tft.fillRect(startX + fwidth, startY, fheight, fwidth, backColor); //Segment 2

  if ((value == 0 || value == 1 || value == 2 || value == 3 || value == 4 || value == 7 || value == 8 || value == 9)& displayDigit)
    tft.fillRect(startX + fheight + fwidth, startY + fwidth, fwidth, fheight, textColor); //Segment 3
  else
    tft.fillRect(startX + fheight + fwidth, startY + fwidth, fwidth, fheight, backColor); //Segment 3

  if ((value == 0 || value == 2 || value == 6 || value == 8)& displayDigit)
    tft.fillRect(startX, startY + fheight + fwidth + fwidth, fwidth, fheight, textColor); //Segment 4
  else
    tft.fillRect(startX, startY + fheight + fwidth + fwidth, fwidth, fheight, backColor); //Segment 4

  if ((value == 2 || value == 3 || value == 4 || value == 5 || value == 6 || value == 8 || value == 9)& displayDigit)
    tft.fillRect(startX + fwidth, startY + fheight + fwidth, fheight, fwidth, textColor); //Segment 5
  else
    tft.fillRect(startX + fwidth, startY + fheight + fwidth, fheight, fwidth, backColor); //Segment 5

  if ((value == 0 || value == 1 || value == 3 || value == 4 || value == 5 || value == 6 || value == 7 || value == 8 || value == 9)& displayDigit)
    tft.fillRect(startX + fwidth + fheight, startY + fheight + fwidth + fwidth, fwidth, fheight, textColor); //Segment 6
  else
    tft.fillRect(startX + fwidth + fheight, startY + fheight + fwidth + fwidth, fwidth, fheight, backColor); //Segment 6

  if ((value == 0 || value == 2 || value == 3 || value == 5 || value == 6 || value == 8 || value == 9)& displayDigit)
    tft.fillRect(startX + fwidth, startY + fheight + fwidth + fwidth + fheight, fheight, fwidth, textColor); //Segment 7
  else
    tft.fillRect(startX + fwidth, startY + fheight + fwidth + fwidth + fheight, fheight, fwidth, backColor); //Segment 7
}



