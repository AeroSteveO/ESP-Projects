#include "esphome.h"
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 121
#define DATA_PIN 2

// esphome dependencies:
// needs: esphome time --> id: current_time
int thinking[][2] = {
    {112, 21}, // T: 12
    {109, 24}, // H: 5
    {2, 36}, // I: 9
    {92, 53}, // N: 8
    {10, 119}, // K: 2
    {46, 95}, // I: 9
    {11, 32}, // N: 8
    {110, 49}  // G: 2
};
int leds_time_it_is[] = {111,112,114,115}; // It Is

int leds_time_minutes[][15] = {
    {  5,   6,   7,   8,   9,  10,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // OClock
    { 81,  80,  79,  78,  72,  73,  74,  75,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // five past
    { 95,  96,  97,  72,  73,  74,  75,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // ten past
    {100, 101, 102, 103, 104, 105, 106,  72,  73,  74,  75,  -1,  -1,  -1,  -1}, // quarter past
    { 89,  90,  91,  92,  93,  94,  72,  73,  74,  75,  -1,  -1,  -1,  -1,  -1}, // twenty past
    { 89,  90,  91,  92,  93,  94,  72,  73,  74,  75,  81,  80,  79,  78,  -1}, // 25 past
    { 83,  84,  85,  86,  72,  73,  74,  75,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // half past
    { 89,  90,  91,  92,  93,  94,  63,  64,  81,  80,  79,  78,  -1,  -1,  -1}, // 25 to
    { 89,  90,  91,  92,  93,  94,  63,  64,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // twenty to
    {100, 101, 102, 103, 104, 105, 106,  63,  64,  -1,  -1,  -1,  -1,  -1,  -1}, // quarter to
    { 95,  96,  97,  63,  64,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // 10 to
    { 81,  80,  79,  78,  63,  64,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}  // five to
};

int leds_time_hours[][6] = {
    { 56,  57,  58,  59,  60,  61}, // twelve
    { 38,  39,  40,  -1,  -1,  -1}, // one
    { 40,  41,  42,  -1,  -1,  -1}, // two
    { 23,  24,  25,  26,  27,  -1}, // three
    { 16,  17,  18,  19,  -1,  -1}, // four
    { 44,  45,  46,  47,  -1,  -1}, // five
    {  1,   2,   3,  -1,  -1,  -1}, // six
    { 11,  12,  13,  14,  15,  -1}, // seven
    { 47,  48,  49,  50,  51,  -1}, // eight
    { 34,  35,  36,  37,  -1,  -1}, // nine
    { 51,  52,  53,  -1,  -1,  -1}, // tem
    { 27,  28,  29,  30,  31,  32}  // eleven
};

int leds_happy_birthday[][8] = {
    {109, 88, 87, 66, 65, -1, -1, -1}, // happy
    {120, 99, 98, 77, 76, 55, 54, 33} // birthday
};

int hour = -1;
int minute = -1;
int red = 124;
int green = 124;
int blue = 124;

int bdred = 0;
int bdgreen = 0;
int bdblue = 0;

int brightness = 50; // half brightness
int prevLightReading = 0;

Adafruit_NeoPixel pixels(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);


namespace esphome {
namespace wordclock {
class Wordclock : public Component, public CustomAPIDevice {
    public:
        void setup() override;
        void on_setled(int number, int red, int blue, int green);
        void loop() override;
	protected:
		void setPixelColor( uint16_t i, uint8_t r, uint8_t g, uint8_t b, uint16_t uiBrightness);
		void show();
		void startStrip();
		void clearStrip();
};

} // namespace wordclock
} // namespace esphome