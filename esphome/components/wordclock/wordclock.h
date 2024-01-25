#include "esphome.h"
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 121
#define DATA_PIN 2

namespace esphome {
namespace wordclock {
class WordClock : public Component, public CustomAPIDevice {
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