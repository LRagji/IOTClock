
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