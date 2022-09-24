#include <string.h>
#include <FastLED.h>
#define DEBUG
#define HARRY
#define CHASER
//#define OLD_SKOOL
//#define SEBASTIEN
#define VCC_PIN 0
#define LED_PIN 26
#define DELAY_TIME 25
#define TOTAL_LEDS 300
CRGB led_strip[TOTAL_LEDS];
#ifdef SEBASTIEN
CRGB::HTMLColorCode seb_colours[4] = { CRGB::HTMLColorCode::Green, CRGB::HTMLColorCode::Red, CRGB::HTMLColorCode::Blue, CRGB::HTMLColorCode::Orange };
#endif
#ifdef HARRY
CRGB::HTMLColorCode seb_colours[4] = { CRGB::HTMLColorCode::White, CRGB::HTMLColorCode::Blue, CRGB::HTMLColorCode::Purple, CRGB::HTMLColorCode::Red };
#endif
#ifdef OLD_SKOOL
CRGB::HTMLColorCode seb_colours[4] = { CRGB::HTMLColorCode::White, CRGB::HTMLColorCode::Blue, CRGB::HTMLColorCode::Orange, CRGB::HTMLColorCode::Red };
#endif

void setup() {
  // put your setup code here, to run once:
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led_strip, TOTAL_LEDS);
}

#ifdef HARRY
void led_reset(int colour = 1) { memset(led_strip, colour, TOTAL_LEDS * 3); }
#else
void led_reset(int colour = 0) { memset(led_strip, colour, TOTAL_LEDS * 3); }
#endif

void dprint(char* message = "", bool newline = false) {
#ifdef DEBUG  
  if (newline) { Serial.println(message); }
  else { Serial.print(message); }
#else
  return;
#endif
}

void show_voltage() {
  float vcc = analogRead(VCC_PIN);   
  int val1 = vcc / 3.9;
  int val5 = (int)val1;
  int val3 = val5 / 100;
  int val2 = (val5 % 100) / 10;
  int val4 = val5 % 10;
  char vccstr[32];
  dprint("Raw VCC: ");
  itoa(val3, vccstr, 10);
  dprint(vccstr);
  itoa(val2, vccstr, 10);
  dprint(vccstr);
  dprint(".");
  itoa(val4, vccstr, 10);
  dprint(vccstr, true);  
}

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
    CRGB::HTMLColorCode this_colour = seb_colours[0];    
#ifdef OLD_SKOOL
    if (count >= collision - 1) { this_colour = seb_colours[0]; }
    else if (count > changeover * 2) { this_colour = seb_colours[3]; }
    else if (count < changeover) { this_colour = seb_colours[1]; }
    else { this_colour = seb_colours[2]; }   
#endif
    led_strip[start + count] = this_colour;
    led_strip[end - count] = this_colour;
#ifdef CHASER    
    if (count > 0) { 
      led_strip[start + count - 1] = seb_colours[1]; 
      led_strip[end - count + 1] = seb_colours[1];
    }
    if (count > 1) { 
      led_strip[start + count - 2] = seb_colours[2]; 
      led_strip[end - count + 2] = seb_colours[2];
    }
    if (count > 2) { 
      led_strip[start + count - 3] = seb_colours[3]; 
      led_strip[end - count + 3] = seb_colours[3];
    }
#endif
    FastLED.show();
    show_voltage();
    delay(timing);    
  }
}

void particle_explode(int start = 0, int end = TOTAL_LEDS - 1, int timing = DELAY_TIME) {
  int num_leds = end - start;
  int collision = num_leds / 2;
  int changeover = collision / 3;
  for (int count = 0; count <= collision; count++) {
    led_reset();
    CRGB::HTMLColorCode this_colour = seb_colours[0];    
#ifdef OLD_SKOOL
    if (count <= 1) { this_colour = seb_colours[0]; }
    else if (count > changeover * 2) { this_colour = seb_colours[3]; }
    else if (count < changeover) { this_colour = seb_colours[1]; }
    else { this_colour = seb_colours[2]; }   
#endif
    led_strip[start + collision + count] = this_colour;
    led_strip[end - collision - count] = this_colour;
#ifdef CHASER    
    if (count > 0) { 
      led_strip[start + collision + count - 1] = seb_colours[1];
      led_strip[end - collision - count + 1] = seb_colours[1];
    }
    if (count > 1) { 
      led_strip[start + collision + count - 2] = seb_colours[2];
      led_strip[end - collision - count + 2] = seb_colours[2];
    }
    if (count > 2) { 
      led_strip[start + collision + count - 3] = seb_colours[3];
      led_strip[end - collision - count + 3] = seb_colours[3];
    }
#endif
    FastLED.show();
    show_voltage();
    delay(timing);    
  }
}

void blow_shit_up(int start = 0, int end = TOTAL_LEDS - 1, int timing = DELAY_TIME) {
  particle_collide(start, end, timing);
  particle_explode(start, end, timing);
}

void loop() {
  // put your main code here, to run repeatedly:
  //ramp_up();
  //ramp_down();
  //particle_collide();
  //particle_explode();
  blow_shit_up();
}
