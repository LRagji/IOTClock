
void drawFrame(uint16_t *frame) {
  matrix.fillScreen(0);
  int imageHeight = 32;
  int imageWidth = 64;
  int counter = 0;
  for (int yy = 0; yy < imageHeight; yy++) {
    for (int xx = 0; xx < imageWidth; xx++) {
      matrix.drawPixel(xx, yy, frame[counter]);
      counter++;
    }
  }
  matrix.show();
  delay(100);
}

void drawSmallFrame(uint8_t startX, uint8_t startY, uint8_t h, uint8_t w, uint16_t *frame) {
  matrix.fillRect(startX, startY, 64, 16, 0);
  int imageHeight = h;
  int imageWidth = w;
  int counter = 0;
  for (int yy = 0; yy < imageHeight; yy++) {
    for (int xx = 0; xx < imageWidth; xx++) {
      matrix.drawPixel(startX + xx, startY + yy, frame[counter]);
      counter++;
    }
  }
  matrix.show();
}