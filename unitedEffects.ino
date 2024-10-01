#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    60
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
uint8_t gHue = 0;  // Global hue variable

#define BUTTON_PIN  2
bool buttonPressed = false;

void setup() {
  delay(1000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Use internal pull-up resistor for the button
}

void loop() {
  static int currentEffect = 0;
  static unsigned long lastButtonPress = 0;
  const unsigned long debounceDelay = 50;
  
  if (digitalRead(BUTTON_PIN) == LOW && (millis() - lastButtonPress > debounceDelay)) {
    lastButtonPress = millis();
    currentEffect = (currentEffect + 1) % 12;  // Cycle through all effects
  }

  switch (currentEffect) {
    case 0: rainbow(); break;
    case 1: confetti(); break;
    case 2: sinelon(); break;
    case 3: bpm(); break;
    case 4: juggle(); break;
    case 5: cylonEffect(CRGB::Red); break;
    case 6: cylonEffect(CRGB::Green); break;
    case 7: cylonEffect(CRGB::Blue); break;
    case 8: cylonEffect(CRGB::Cyan); break;
    case 9: cylonEffect(CRGB::Magenta); break;
    case 10: cylonEffect(CRGB::Yellow); break;
    case 11: cylonEffect(CRGB::White); break;
  }

  FastLED.show();
  FastLED.delay(20);
}

void rainbow() {
  fill_rainbow(leds, NUM_LEDS, 0, 7);
}

void confetti() {
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(random8(64), 200, 255);
}

void sinelon() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(0, 255, 192);
}

void bpm() {
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void cylonEffect(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
    FastLED.show();
    fadeAll();
    delay(10);
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = color;
    FastLED.show();
    fadeAll();
    delay(10);
  }
}

void fadeAll() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
  }
}
