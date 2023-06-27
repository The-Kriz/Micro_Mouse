
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 1
#define BRIGHTNES 10
#define DATA_PIN 3
CRGB leds[NUM_LEDS];

void setup() { 
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
    FastLED.setBrightness(BRIGHTNES);
}

void loop() { 
  // Turn the LED on, then pause
  leds[0] = CRGB::Green;
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(500);
}
