#include <Adafruit_WS2801.h>

uint8_t num_LEDs = 24;
uint8_t led_stairs = 2;
uint8_t clockPin = 2;
uint8_t dataPin = 3;
uint8_t pirUpSensor = 6;
uint8_t pirDownSensor = 7;
uint32_t color = 0;
uint8_t sensorStatus = 0;

#define GO_UP -1            // Direction control - Arduino at top of stairs
#define GO_DOWN 1           // Direction control - Arduino at top of stairs

Adafruit_WS2801 strip = Adafruit_WS2801(num_LEDs, dataPin, clockPin);

uint32_t getColor(uint8_t r, uint8_t g, uint8_t b)
{
  return ((((r << 8) | g) << 8) | b);
}

void lights(int8_t, boolean);
void stripClear();

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing serial communication...");
  
  delay(2000); // Power Up 3 second safety delay.

  // Prepare sensors for read
  pinMode(pirDownSensor, INPUT); // 7
  pinMode(pirUpSensor, INPUT);  // 6

  // Initialize strip
  strip.begin();
  strip.show();
  
  stripClear();

  delay(1000);

  // Run bootup test - Tests every color
  welcomeTestStrip();

  Serial.println("Reading sensor input...");

  delay(1000);

  Serial.println("Attempting test light up");

  lightUp();
}

void loop() {
  /*sensorStatus = digitalRead(pirDownSensor);

  if (sensorStatus == HIGH) {
    lightUp();
    stripClear();
  }

  sensorStatus = digitalRead(pirUpSensor);

  if (sensorStatus == HIGH) {
    lightDown();
    stripClear();
  }*/

  delay(250);
}

void welcomeTestStrip() {
  stripClear();
  
  Serial.println("Beginning strip test");
  
  testReds();

  delay(750);

  stripClear();

  delay(25);
  
  testGreens();

  delay(750);

  stripClear();

  delay(25);
  
  testBlues();

  delay(750);
  
  // Test complete - shut down the strip
  stripClear();

  delay(25);
}

void testReds() {
  Serial.println("Testing the red shades");
  
  for (uint16_t i = 0; i < num_LEDs; i++) {
    strip.setPixelColor(i, 230, 0, 0);
    strip.show();
  }

  delay(2000);
}

void testGreens() {
  Serial.println("Testing the green shades");
  
  for (uint16_t i = 0; i < num_LEDs; i++) {
    strip.setPixelColor(i, 0, 230, 0);
    strip.show();
  }

  delay(2000);
}

void testBlues() {
  Serial.println("Testing the blue shades");
  
  for (uint16_t i = 0; i < num_LEDs; i++) {
    strip.setPixelColor(i, 0, 0, 230);
    strip.show();
  }

  delay(2000);
}

void stripClear() {
  for(uint16_t i = 0; i < num_LEDs; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
  }
}

void lightUp() {
  stripClear();

  Serial.println("Lights go up");

  lights(GO_UP, true); // Turn the lights on with delays
  
  delay(10000); // Wait for 10 seconds to allow the person to climb up
  
  lights(GO_UP, false); // Turn the lights off with delays
}

void lightDown() {
  stripClear();

  Serial.println("Lights go down");

  lights(GO_DOWN, true); // Turn the lights on

  delay(15000); // Wait for 15 seconds to allow the person to climb down

  lights(GO_DOWN, false); // Turn the lights off
}

void lights(int8_t order, boolean mode = true) {
  int i = 0;
  
  color = mode ? getColor(230, 230, 230) : 0; // This is the lightup color
  
  if (order == GO_UP) {
    for (i = 0; i < num_LEDs; i++) {
      strip.setPixelColor(i, color);
      strip.show();

      if (i % led_stairs) {
        delay(250);
      }
    }
  }
  else if (order == GO_DOWN) {
    for (i = num_LEDs; --i >= 0 ; ) {
      strip.setPixelColor(i, color);
      strip.show();

      if (i % led_stairs) {
        delay(300);
      }
    }
  }
}
