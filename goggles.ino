// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define PIXEL_PIN 0

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, PIXEL_PIN);

//uint32_t first_color = 0xffc0b;
uint32_t first_color = 0x0369cf;
volatile uint32_t color  = first_color;
uint32_t frame = 0;
uint32_t delay_time = 50;

void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.setBrightness(32);

  pinMode(2, INPUT);
  attachInterrupt(0, changeColor, FALLING);
}

void changeColor() {
  color = (color >> 4) | ((color & 0x00000F) << 20);
  if(!color) color = first_color;
}

uint32_t darken(uint32_t color, uint8_t shift) {
  uint32_t r = (color & 0xFF0000) >> (16+shift);
  uint32_t g = (color & 0x00FF00) >> (8+shift);
  uint32_t b = (color & 0x0000FF) >> shift;

  return (r << 16) | (g << 8) | b;
}

void loop() {
  bool direction = (frame / 128) % 2 == 0 ? false : true;
  
  uint8_t  i, j;
  uint32_t offset = frame % 16;

  for(i=0; i<16; i++) {
    j = i;
    if (!direction) j = (18-i) % 16;

    uint32_t c = color;
    uint8_t darken_amount = (offset+j) & 7;
    if(darken_amount > 0) {
      c = darken(color, darken_amount + 1);
    }
    
    pixels.setPixelColor((i+2)%16, c);
    pixels.setPixelColor(31-i, c);
  }
  pixels.show();

  frame++;

  if(frame % 128 == 0) {      // Every 8 times around
    changeColor();
  }
  delay(delay_time);
}
