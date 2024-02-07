#pragma once

#include "esphome.h"
#include <Adafruit_NeoPixel.h>
#include "esphome/core/component.h"
//#include "esphome/components/output/float_output.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/light/addressable_light.h"
#include "esphome/components/time/real_time_clock.h"

#define NUM_LEDS 121
#define DATA_PIN 2

namespace esphome {
namespace wordclock {
	
class WordClock : public light::AddressableLight { // , public Component
    public:
        void update_time();
		void add_leds(uint8 num_leds, uint8 data_pin) { pixels = new Adafruit_NeoPixel(num_leds, data_pin, NEO_GRB + NEO_KHZ800); }
		void set_time_id(time::RealTimeClock *time_id) { this->time_id_ = time_id; }

		
		
        void setup() override;
		light::LightTraits get_traits() override;
		void write_state(light::LightState *state) override;
		void dump_config() override;

		int32_t size() const override { return this->pixels->numPixels(); }
		void clear_effect_data() override {
			// we aren't making use of effects, so nothing to do here
			for (int i = 0; i < this->size(); i++) {
				this->effect_data_[i] = 0;
			}
		}
		  
	protected:
		Adafruit_NeoPixel *pixels; //(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);
		uint8_t *effect_data_{nullptr};
		time::RealTimeClock  *time_id_{nullptr};

		uint8_t getNthByte(uint32_t bytes, uint8_t byteToGet) const {
			uint8_t nthByte = (bytes >> (8*byteToGet)) & 0xff;
			return nthByte;
		}
		
		
		light::ESPColorView get_view_internal(int32_t index) const override {  // NOLINT
			uint32 color = pixels->getPixelColor(index);
			
			uint8_t white = getNthByte(color, 0);
			uint8_t red = getNthByte(color, 1);
			uint8_t green = getNthByte(color, 2);
			uint8_t blue = getNthByte(color, 3);
			
			return light::ESPColorView(&red, &green, &blue,
				&white, this->effect_data_ + index, nullptr);
		}
		void setPixelColor( uint16_t i, uint8_t r, uint8_t g, uint8_t b, uint16_t uiBrightness);
		void show();
		void startStrip();
		void clearStrip();
};

} // namespace wordclock
} // namespace esphome