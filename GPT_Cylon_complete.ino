#include <FastLED.h>

#define LED_PIN 6       // Pin where the LED strip is connected
#define NUM_LEDS 60     // Number of LEDs in the strip
#define BUTTON_PIN 2    // Pin where the button is connected
#define BRIGHTNESS 255  // Max LED brightness

CRGB leds[NUM_LEDS];

int currentEffect = 0;  // Track the current LED effect
bool pressed = false;
//unsigned long lastDebounceTime = 0;
//unsigned long debounceDelay = 50;
//int lastButtonState = LOW;

void setup() {
  // Initialize LED strip
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // Initialize button
  pinMode(BUTTON_PIN, INPUT);  // Use internal pull-up resistor

  Serial.begin(9600);  // Optional: for debugging
}

void loop() {
  checkButton();  // Check if the button is pressed

  // Switch between 7 Cylon effects for different colors
  switch (currentEffect) {
    case 0: cylonEffect(CRGB::Red); break;
    case 1: cylonEffect(CRGB::Green); break;
    case 2: cylonEffect(CRGB::Blue); break;
    case 3: cylonEffect(CRGB::Cyan); break;
    case 4: cylonEffect(CRGB::Magenta); break;
    case 5: cylonEffect(CRGB::Yellow); break;
    case 6: cylonEffect(CRGB::White); break;
    case 7: cylonRainbow(); break;
    default: cylonEffect(CRGB::Red); break;  // Default effect
  }

  FastLED.show();  // Display the LED pattern
  delay(30);       // Adjust delay for the desired speed of effect
}

/*void checkButton() {
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonState == LOW && !buttonPressed) {  // Button is pressed (active-low)
      currentEffect = (currentEffect + 1) % 8;   // Cycle through 8 effects
      buttonPressed = true;
    } else if (buttonState == HIGH) {
      buttonPressed = false;  // Reset button state when released
    }
  }

  lastButtonState = buttonState;
}*/

void checkButton() {
  bool currentState = digitalRead(BUTTON_PIN);

  if (currentState == pressed) {
    while (digitalRead(BUTTON_PIN) == pressed) {}
    currentEffect = (currentEffect + 1) % 8;   // Cycle through 8 effects
    Serial.println(currentEffect);
    delay(100);
  }
}

// Cylon effect that moves a light back and forth
void cylonEffect(CRGB color) {
  static uint8_t hue = 0;

  // Slide the LED in one direction
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;  // Set the LED to the specified color
    FastLED.show();   // Show the LEDs
    fadeAll();        // Fade all LEDs gradually
    delay(10);        // Adjust the speed of the Cylon effect
  }

  // Slide the LED in the opposite direction
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = color;  // Set the LED to the specified color
    FastLED.show();   // Show the LEDs
    fadeAll();        // Fade all LEDs gradually
    delay(10);        // Adjust the speed of the Cylon effect
  }
}

// Fade all LEDs slightly to simulate trailing effect
void fadeAll() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);  // Gradual fading of LEDs
  }
}

void cylonRainbow() {
  static uint8_t hue = 0;

  // First slide the LED in one direction
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue++, 255, 255);  // Set the LED to the current hue
    FastLED.show();                   // Show the LEDs
    fadeAll();                        // Fade all LEDs gradually
    delay(10);                        // Adjust the speed of the Cylon effect
  }

  // Then slide the LED in the other direction
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = CHSV(hue++, 255, 255);  // Set the LED to the current hue
    FastLED.show();                   // Show the LEDs
    fadeAll();                        // Fade all LEDs gradually
    delay(10);                        // Adjust the speed of the Cylon effect
  }
}
