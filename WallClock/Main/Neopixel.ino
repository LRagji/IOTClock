
void blink(uint32_t p,uint32_t c, int onTime, int offTime) {
  strip.setPixelColor(p, c);
  strip.show();
  delay(onTime);
  strip.clear();
  strip.show();
  delay(offTime);
}