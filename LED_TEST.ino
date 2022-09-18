#include <FastLED.h>
//#define DEBUG
#define LED_PIN 26
#define DELAY_TIME 25
#define TOTAL_LEDS 300
CRGB led_strip[TOTAL_LEDS];

void setup() {
  // put your setup code here, to run once:
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led_strip, TOTAL_LEDS);
}

void led_reset(int colour = 0) { memset(led_strip, colour, TOTAL_LEDS * 3); }

void ramp_up(int start = 0, int end = TOTAL_LEDS - 1, int timing = DELAY_TIME) {
  int num_leds = end - start;
  for (int led = start; led <= end; led++) {    
    led_reset();
    int this_colour = map(led, 0, num_leds, 0, 256);        
    led_strip[led].r = this_colour;
    led_strip[led].b = 255 - this_colour;
    FastLED.show();
    delay(timing);    
  }
}

void ramp_down(int start = TOTAL_LEDS - 1, int end = 0, int timing = DELAY_TIME) {
  int num_leds = start - end;
  for (int led = start; led >= end; led--) {
    led_reset();
    int this_colour = map(led, 0, num_leds, 0, 256);        
    led_strip[led].g = this_colour;
    led_strip[led].b = 255 - this_colour;
    FastLED.show();
    delay(timing);    
  }
}

void particle_collide(int start = 0, int end = TOTAL_LEDS - 1, int timing = DELAY_TIME) {
  int num_leds = end - start;
  int collision = num_leds / 2;
  int changeover = collision / 3;
  for (int count = 0; count <= collision; count++) {
    led_reset();
    CRGB::HTMLColorCode this_colour = CRGB::White;    
    if (count == collision) { this_colour = CRGB::White; }
    else if (count > changeover * 2) { this_colour = CRGB::Green; }
    else if (count < changeover) { this_colour = CRGB::Red; }
    else { this_colour = CRGB::Blue; }   
    led_strip[start + count] = this_colour;
    led_strip[end - count] = this_colour;
    FastLED.show();
    delay(timing);    
  }
}

void particle_explode(int start = 0, int end = TOTAL_LEDS - 1, int timing = DELAY_TIME) {
  int num_leds = end - start;
  int collision = num_leds / 2;
  int changeover = collision / 3;
  for (int count = 0; count <= collision; count++) {
    led_reset();
    CRGB::HTMLColorCode this_colour = CRGB::White;    
    if (count == 0) { this_colour = CRGB::White; }
    else if (count > changeover * 2) { this_colour = CRGB::Red; }
    else if (count < changeover) { this_colour = CRGB::Green; }
    else { this_colour = CRGB::Blue; }   
    led_strip[start + collision + count] = this_colour;
    led_strip[end - collision - count] = this_colour;
    FastLED.show();
    delay(timing);    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int low = 90;
  int high = 175;
  int timer = 50;
  ramp_up(low, high, timer);
  ramp_down(high, low, timer);
  particle_collide(low, high, timer);
  particle_explode(low, high, timer);
}
