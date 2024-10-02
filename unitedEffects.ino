#include <FastLED.h>
FASTLED_USING_NAMESPACE
#define BUTTON_PIN_1  2 // Button for switching effects
#define BUTTON_PIN_1  8 // Button for switching solid colors
#define LED_PIN     6
#define NUM_LEDS    60
#define VOLTS       5
#define MAX_MA      2000
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define FRAMES_PER_SECOND  120
CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;

// List of effects to cycle through.  Each is defined as a separate function below.
typedef void (*SimpleEffectList[])();
SimpleEffectList sEffects = {pride, rainbow, rainbowWithGlitter, pacifica_loop, Fire2012WithPalette, cylon, confetti, sinelon, juggle, bpm, waveEffect };

bool gReverseDirection = false;
bool first_buttonPressed = false;
bool second_buttonPressed = false;
uint8_t gHue = 0;  // Global hue variable
uint8_t currentEffect = 0;
uint8_t currentColor = 0;


void setup() {
  delay(1000);
  Serial.begin(9600); 
  FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip).setDither(BRIGHTNESS < 255);
  FastLED.setBrightness(BRIGHTNESS);
  gPal = HeatColors_p;
  //pinMode(BUTTON_PIN_1, INPUT);  // Use external pull-up resistor for the button
  //pinMode(BUTTON_PIN_2, INPUT);  // Use external pull-up resistor for the button
}



CRGB solidColors[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::Cyan, CRGB::Magenta, CRGB::White};

void loop() {
  readButton1();
  readButton2();
  /*bool currentState = digitalRead(BUTTON_PIN);

  if (currentState == pressed){ 
    while(digitalRead(BUTTON_PIN) == pressed){}
    nextPattern(); 
    Serial.println(gCurrentPatternNumber);
    delay(100);*/
  
  random16_add_entropy( random());
  sEffects[currentEffect]();

  /*if (digitalRead(BUTTON_PIN) == LOW && (millis() - lastButtonPress > debounceDelay)) {
    lastButtonPress = millis();
    currentEffect = (currentEffect + 1) % 30;  // Cycle through all effects including wave effects
  }*/

  /*switch (currentEffect) {
    case 0: pride(); break;
    case 1: rainbow(); break;
    case 2: rainbowWithGlitter(); break;
    case 3: pacifica_loop(); break;
    case 4: Fire2012WithPalette(); break;
    case 5: confetti(); break;
    case 6: cylonEffect(CRGB::Red); break;
    case 7: cylonEffect(CRGB::Green); break;
    case 8: cylonEffect(CRGB::Blue); break;
    case 9: cylonEffect(CRGB::Cyan); break;
    case 10: cylonEffect(CRGB::Magenta); break;
    case 11: cylonEffect(CRGB::Yellow); break;
    case 12: cylonEffect(CRGB::White); break;
    case 13: originalCylon(); break;
    case 14: waveEffect(CRGB::Red); break;
    case 15: waveEffect(CRGB::Green); break;
    case 16: waveEffect(CRGB::Blue); break;
    case 17: waveEffect(CRGB::Cyan); break;
    case 18: waveEffect(CRGB::Magenta); break;
    case 19: waveEffect(CRGB::Yellow); break;
    case 20: waveEffect(CRGB::White); break;
    case 21: fill_solid(leds, NUM_LEDS, CRGB::Red); break;
    case 22: fill_solid(leds, NUM_LEDS, CRGB::Orange); break;
    case 23: fill_solid(leds, NUM_LEDS, CRGB::Green); break;
    case 24: fill_solid(leds, NUM_LEDS, CRGB::Blue); break;
    case 25: fill_solid(leds, NUM_LEDS, CRGB::Cyan); break;
    case 26: fill_solid(leds, NUM_LEDS, CRGB::Magenta); break;
    case 27: fill_solid(leds, NUM_LEDS, CRGB::Pink); break;
    case 28: fill_solid(leds, NUM_LEDS, CRGB::Yellow); break;
    case 29: fill_solid(leds, NUM_LEDS, CRGB::White); break;*/
  }

  FastLED.show();
  FastLED.delay(20);
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextEffect()
{
  // add one to the current effect number, and wrap around at the end
  currentEffect = (currentEffect + 1) % ARRAY_SIZE(sEffects);

void nextColor()
{
  // add one to the current color number, and wrap around at the end
  currentColor = (currentColor + 1) % (sizeof(solidColors) / sizeof(CRGB) + 1);
}

void readButton1() {
  bool currentState = digitalRead(BUTTON_PIN_1);

  if (currentState == first_buttonPressed){ 
    while(digitalRead(BUTTON_PIN_1) == first_buttonPressed){}
    nextEffect(); 
    Serial.println(currentEffect);
    delay(100);
}

void readButton2() {
  bool currentState = digitalRead(BUTTON_PIN_2);

  if (currentState == second_buttonPressed){ 
    while(digitalRead(BUTTON_PIN_2) == second_buttonPressed){}
    nextColor(); 
    Serial.println(currentEffect);
    delay(100);
}

// Pride effect
void pride() {
  static uint8_t hue = 0;
  hue += 1;
  fill_rainbow(leds, NUM_LEDS, hue, 7);
}

// Original Cylon effect
void originalCylon() {
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeAll();
    delay(10);
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeAll();
    delay(10);
  }
}

// Rainbow with glitter
void rainbowWithGlitter() {
  rainbow();
  addGlitter(80);
}

// Rainbow effect
void rainbow() {
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

// Confetti effect
void confetti() {
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

// Cylon effect that moves a light back and forth
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

// Fade all LEDs slightly to simulate trailing effect
void fadeAll() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
  }
}

// Add glitter to the rainbow
void addGlitter(fract8 chanceOfGlitter) {
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

// Wave effect for flowing color
void waveEffect() {
  uint8_t wavePhase = millis() >> 4;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = solidColors[currentColor];
    leds[i].fadeLightBy(sin8(wavePhase + (i * 16)));
  }
  wavePhase++;
}



CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };


void pacifica_loop()
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  // Clear out the LED array to a dim background blue-green
  fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  pacifica_deepen_colors();
}

// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps()
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors()
{
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100 
#define COOLING  70 

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

void Fire2012WithPalette()
{
// Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      uint8_t colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}
