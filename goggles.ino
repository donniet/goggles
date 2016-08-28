// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define PIN 0

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, PIN);

//uint32_t first_color = 0xffc0b;
uint32_t first_color = 0x0369cf;
uint32_t color  = first_color;
bool direction = true;
uint32_t frame = 0;

void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.setBrightness(10); // 1/3 brightness
}

uint32_t darken(uint32_t color, uint8_t shift) {
  uint32_t r = (color & 0xFF0000) >> (16+shift);
  uint32_t g = (color & 0x00FF00) >> (8+shift);
  uint32_t b = (color & 0x0000FF) >> shift;

  return (r << 16) | (g << 8) | b;
}

void loop() {
  uint32_t darker = darken(color, 2);
  uint32_t darker_still = darken(color, 3);
  
  uint8_t  i, j;
  uint32_t offset = frame % 16;

  for(i=0; i<16; i++) {
    uint32_t c = 0;
    j = i;
    if (!direction) j = (18-i) % 16;
    if(((offset + j) & 7) == 2) c = darker_still;
    if(((offset + j) & 7) == 1) c = darker; // 4 pixels on...
    if(((offset + j) & 7) == 0) c = color;
    pixels.setPixelColor(   (i+2)%16, c); // First eye
    pixels.setPixelColor((31-i), c); // Second eye (flipped)
  }
  pixels.show();

  frame++;

  if (frame % 16 == 0) {
    direction = !direction;
  }

  if(frame >= 160) {      // Every 10 times around
    color = (color >> 4) | ((color & 0x00000F) << 20);
    if(!color) color = first_color;
    
    for(i=0; i<32; i++) pixels.setPixelColor(i, 0);
    frame = 0;
  }
  delay(50);
}
