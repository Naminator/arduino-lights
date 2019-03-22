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

uint32_t color = 0;
uint8_t sensorStatus = 0;

uint32_t getColor(uint8_t r, uint8_t g, uint8_t b)
{
  return ((((r << 8) | g) << 8) | b);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing serial communication...");
  
  delay(3000); // Power Up 3 second safety delay.

  // Prepare sensors for read
  pinMode(PIN_PIR_DOWN, INPUT); // 5
  pinMode(PIN_PIR_UP, INPUT);  // 7

  digitalWrite(PIN_PIR_DOWN, LOW);
  digitalWrite(PIN_PIR_UP, LOW);

  // Initialize strip
  strip.begin();

  stripClear();
  
  strip.show();

  delay(1000);

  // Run bootup test - Tests every color
  welcomeTestStrip();

  Serial.println("Reading sensor input...");
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
  color = mode ? getColor(230, 230, 230) : 0; // This is the lightup color
  
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
  stripClear();
  
  Serial.println("Beginning strip test");
  
  testReds();

  delay(750);

  stripClear();
  
  testGreens();

  delay(750);

  stripClear();
  
  testBlues();

  delay(750);
  
  // Test complete - shut down the strip
  stripClear();
}

void testReds() {
  Serial.println("Testing the red shades");
  
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 255, 0, 0);
  }

  strip.show();

  delay(2000);
}

void testGreens() {
  Serial.println("Testing the green shades");
  
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0, 255, 0);
  }

  strip.show();

  delay(2000);
}

void testBlues() {
  Serial.println("Testing the blue shades");
  
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0, 0, 255);
  }

  strip.show();

  delay(2000);
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
