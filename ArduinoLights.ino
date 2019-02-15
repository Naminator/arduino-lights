#include <Adafruit_WS2801.h>
#include <SPI.h>

#define NUM_LEDS 24         // Total number of LEDs.
#define LEDS_PER_STAIR 2    // Number of LEDs per stair. 

#define PIN_LED_DATA 3      // LED Data pin
#define PIN_LED_CLOCK 2     // LED Clock pin

#define PIN_PIR_DOWN 6      // PIR Downstairs Pin
#define PIN_PIR_UP 7        // PIR Upstairs Pin

#define GO_UP -1            // Direction control - Arduino at top of stairs
#define GO_DOWN 1           // Direction control - Arduino at top of stairs

Adafruit_WS2801 strip = Adafruit_WS2801(NUM_LEDS, PIN_LED_DATA, PIN_LED_CLOCK);

uint8_t hue = 0;
uint32_t color = 0;
uint8_t sensorStatus = 0;

uint32_t getColor(uint8_t r, uint8_t g, uint8_t b)
{
  return ((((r << 8) | g) << 8) | b);
}

void setup() {
  delay (3000); // Power Up 3 second safety delay.

  // Prepare sensors for read
  pinMode(PIN_PIR_DOWN, INPUT); // 5
  pinMode(PIN_PIR_UP, INPUT);  // 7

  digitalWrite(PIN_PIR_DOWN, LOW);
  digitalWrite(PIN_PIR_UP, LOW);

  // Initialize strip
  strip.begin();
  strip.show();

  // Run bootup test - Tests every color
  welcomeTestStrip();
}

void loop() {
  sensorStatus = digitalRead(PIN_PIR_DOWN);

  if (sensorStatus == HIGH) {
    lightUp();
    stripClear();
  }

  sensorStatus = digitalRead(PIN_PIR_UP);

  if (sensorStatus == HIGH) {
    lightDown();
    stripClear();
  }
}

void lights(int8_t order, boolean mode = true) {
  int i = 0;
  hue = mode ? 230 : 0;
  color = getColor(hue, hue, hue); // This is the lightup color

  if (order == GO_UP) {
    for (i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, color);
      strip.show();

      if (i % LEDS_PER_STAIR) {
        delay(250);
      }
    }
  }
  else if (order == GO_DOWN) {
    for (i = NUM_LEDS; --i >= 0 ; ) {
      strip.setPixelColor(i, color);
      strip.show();

      if (i % NUM_LEDS) {
        delay(300);
      }
    }
  }
}

void welcomeTestStrip() {

  void testReds();

  delay(750);

  stripClear();
  
  void testGreens();

  delay(750);

  stripClear();
  
  void testBlues();

  delay(750);
  
  // Test complete - shut down the strip
  stripClear();
}

void testReds() {
  for(uint8_t i = 0; i < NUM_LEDS; i++) {
    testColor(getColor(i, 0, 0));
    delay(2);
  }
}

void testGreens() {
  for(uint8_t i = 0; i < NUM_LEDS; i++) {
    testColor(getColor(0, i, 0));
    delay(2);
  }
}

void testBlues() {
  for(uint8_t i = 0; i < NUM_LEDS; i++) {
    testColor(getColor(0, 0, i));
    delay(2);
  }
}

void testColor(uint32_t color) {
  for(uint8_t i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }

  strip.show();
}

void stripClear() {
  for(uint8_t i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0);
  }

  strip.show();
}

void lightUp() {
  stripClear();

  lights(GO_UP, true); // Turn the lights on with delays
  
  delay(10000); // Wait for 10 seconds to allow the person to climb up
  
  lights(GO_UP, false); // Turn the lights off with delays
}

void lightDown() {
  stripClear();

  lights(GO_DOWN, true); // Turn the lights on

  delay(15000); // Wait for 15 seconds to allow the person to climb down

  lights(GO_DOWN, false); // Turn the lights off
}
