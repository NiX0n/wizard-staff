#include <Adafruit_NeoPixel.h>

constexpr uint8_t NEOPIXEL_PIN = A16;
constexpr uint16_t PIXEL_COUNT = 1;

Adafruit_NeoPixel strip(
    PIXEL_COUNT,
    NEOPIXEL_PIN,
    NEO_GRB + NEO_KHZ800
);

void setup()
{
  Serial.begin(115200);

  while (!Serial)
  {
    delay(10);
  }

  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Before strip.begin()");
  strip.begin();

  //strip.fill(strip.Color(255, 0, 0));
  strip.setBrightness(0x80);
  strip.setPixelColor(0, 0x80, 0, 0);
  strip.show();
  Serial.println("After strip.show()");

  // LED indicattes success
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop(){}