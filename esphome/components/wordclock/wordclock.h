#pragma once

#include "esphome.h"
#include <Adafruit_NeoPixel.h>
#include "esphome/core/component.h"
//#include "esphome/components/output/float_output.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/light/addressable_light.h"


#define NUM_LEDS 121
#define DATA_PIN 2

namespace esphome {
namespace wordclock {
	
class WordClock : public light::LightOutput, public Component {
    public:
		light::LightTraits get_traits() override;
        void setup() override;
        void on_setled(int number, int red, int blue, int green);
		//void set_output(output::FloatOutput *output) { output_ = output; }
		void write_state(light::LightState *state) override;
        void loop() override;
		void dump_config() override;
		light::AddressableLight *clock_face{nullptr};
		void set_clock_face_lights(light::AddressableLight *inputClockFace) { clock_face = inputClockFace; }
	protected:
		void setPixelColor( uint16_t i, uint8_t r, uint8_t g, uint8_t b, uint16_t uiBrightness);
		void show();
		void startStrip();
		void clearStrip();
		//output::FloatOutput *output_;
};

} // namespace wordclock
} // namespace esphome