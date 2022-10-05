#include <FastLED.h>
#include <LiquidCrystal.h>
#define DEBUG
#define V05_PIN A0
#define V12_PIN A1
#define LED_PIN 34
#define BUT_PIN 21
#define DELAY_TIME 25
#define TOTAL_LEDS 300
bool led_status = false;
CRGB led_strip[TOTAL_LEDS];
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
CRGB::HTMLColorCode seb_colours[4] = { CRGB::HTMLColorCode::White, CRGB::HTMLColorCode::Yellow, CRGB::HTMLColorCode::Green, CRGB::HTMLColorCode::Blue };

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
#ifdef DEBUG
  Serial.begin(9600);
#endif
  pinMode(BUT_PIN, INPUT);
  pinMode(V05_PIN, INPUT); 
  pinMode(V12_PIN, INPUT); 
  pinMode(LED_PIN, OUTPUT);    
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led_strip, TOTAL_LEDS);
}

void led_reset(int colour = 0, bool reset = false) { 
  memset(led_strip, colour, TOTAL_LEDS * 3); 
  if (reset == true) { FastLED.show(); }
}

void dprint(char message[] = "", bool newline = false) {
#ifdef DEBUG  
  if (newline) { Serial.println(message); }
  else { Serial.print(message); }
#else
  return;
#endif
}

void dprint(bool message = false, bool newline = false) {
#ifdef DEBUG  
  if (newline) { Serial.println(message); }
  else { Serial.print(message); }
#else
  return;
#endif
}

void dprint(int message = 0, bool newline = false) {
#ifdef DEBUG  
  if (newline) { Serial.println(message); }
  else { Serial.print(message); }
#else
  return;
#endif
}

void dprint(float message = 0, bool newline = false) {
#ifdef DEBUG  
  if (newline) { Serial.println(message); }
  else { Serial.print(message); }
#else
  return;
#endif
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void show_voltage() {  
  int v05 = analogRead(V05_PIN);   
  int v12 = analogRead(V12_PIN);
  lcd.setCursor(0, 0);
  lcd.print("5vDC Bus: ");
  lcd.print(fmap(v05, 0, 1024, 0, 26));
  lcd.print("v");
  lcd.setCursor(0, 1);
  lcd.print("LED Status: ");
  if (led_status == true) { lcd.print("ON "); }
  else { lcd.print("OFF"); }
#ifdef DEBUG
  dprint("RAW 5V: ");
  dprint(v05);
  dprint(", V05: ");
  dprint(fmap(v05, 0, 1024, 0, 26));    
  dprint("v, RAW 12V: ");
  dprint(v12);
  dprint(", V12: ");
  dprint(fmap(v12, 0, 1024, 0, 26));        
  dprint("v.", true);
#endif
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
  show_voltage();
  if (led_status == false) { return; }
  int num_leds = end - start;
  int collision = num_leds / 2;
  int changeover = collision / 3;
  for (int count = 0; count <= collision; count++) {
    led_reset();
    CRGB::HTMLColorCode this_colour = seb_colours[0];    
    led_strip[start + count] = this_colour;
    led_strip[end - count] = this_colour;
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
    FastLED.show();    
    delay(timing);    
  }
}

void particle_explode(int start = 0, int end = TOTAL_LEDS - 1, int timing = DELAY_TIME) {
  show_voltage();
  if (led_status == false) { return; }
  int num_leds = end - start;
  int collision = num_leds / 2;
  int changeover = collision / 3;
  for (int count = 0; count <= collision; count++) {
    led_reset();
    CRGB::HTMLColorCode this_colour = seb_colours[0];    
    led_strip[start + collision + count] = this_colour;
    led_strip[end - collision - count] = this_colour;
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
    FastLED.show();    
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
  led_reset(0, true);
  if (digitalRead(BUT_PIN) == LOW) { 
    if (led_status == true) { led_status = false; }
    else { led_status = true; }
    delay(1000);
  }
  dprint(led_status, true);
  blow_shit_up();    
}


