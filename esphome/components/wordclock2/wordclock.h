#pragma once

#ifdef USE_ARDUINO

#include "esphome/core/macros.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/color.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/light/addressable_light.h"

#include "NeoPixelBus.h"

namespace esphome {
namespace wordclock2 {

enum class ESPNeoPixelOrder {
  GBWR = 0b11000110,
  GBRW = 0b10000111,
  GBR = 0b10000111,
  GWBR = 0b11001001,
  GRBW = 0b01001011,
  GRB = 0b01001011,
  GWRB = 0b10001101,
  GRWB = 0b01001110,
  BGWR = 0b11010010,
  BGRW = 0b10010011,
  BGR = 0b10010011,
  WGBR = 0b11011000,
  RGBW = 0b00011011,
  RGB = 0b00011011,
  WGRB = 0b10011100,
  RGWB = 0b00011110,
  BWGR = 0b11100001,
  BRGW = 0b01100011,
  BRG = 0b01100011,
  WBGR = 0b11100100,
  RBGW = 0b00100111,
  RBG = 0b00100111,
  WRGB = 0b01101100,
  RWGB = 0b00101101,
  BWRG = 0b10110001,
  BRWG = 0b01110010,
  WBRG = 0b10110100,
  RBWG = 0b00110110,
  WRBG = 0b01111000,
  RWBG = 0b00111001,
};
int thinking[][2] = {
    {112, 21}, // T: 12
    {109, 24}, // H: 5
    {2, 36}, // I: 9
    {92, 53}, // N: 8
    {10, 119}, // K: 2
    {45, 95}, // I: 9
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

template<typename T_METHOD, typename T_COLOR_FEATURE>
class WordClockLightOutputBase : public light::AddressableLight {
 public:
  NeoPixelBus<T_COLOR_FEATURE, T_METHOD> *get_controller() const { return this->controller_; }

  void clear_effect_data() override {
    for (int i = 0; i < this->size(); i++)
      this->effect_data_[i] = 0;
  }

  /// Add some LEDS, can only be called once.
  void add_leds(uint16_t count_pixels, uint8_t pin) {
    this->add_leds(new NeoPixelBus<T_COLOR_FEATURE, T_METHOD>(count_pixels, pin));
  }
  void add_leds(uint16_t count_pixels, uint8_t pin_clock, uint8_t pin_data) {
    this->add_leds(new NeoPixelBus<T_COLOR_FEATURE, T_METHOD>(count_pixels, pin_clock, pin_data));
  }
  void add_leds(uint16_t count_pixels) { this->add_leds(new NeoPixelBus<T_COLOR_FEATURE, T_METHOD>(count_pixels)); }
  void add_leds(NeoPixelBus<T_COLOR_FEATURE, T_METHOD> *controller) {
    this->controller_ = controller;
    // controller gets initialised in setup() - avoid calling twice (crashes with RMT)
    // this->controller_->Begin();
  }

  // ========== INTERNAL METHODS ==========
  void setup() override {
    for (int i = 0; i < this->size(); i++) {
      (*this)[i] = Color(0, 0, 0, 0);
    }

    this->effect_data_ = new uint8_t[this->size()];  // NOLINT
    this->controller_->Begin();
  }

  void write_state(light::LightState *state) override {
    this->mark_shown_();
    this->controller_->Dirty();

    this->controller_->Show();
  }

  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  int32_t size() const override { return this->controller_->PixelCount(); }

  void set_pixel_order(ESPNeoPixelOrder order) {
    uint8_t u_order = static_cast<uint8_t>(order);
    this->rgb_offsets_[0] = (u_order >> 6) & 0b11;
    this->rgb_offsets_[1] = (u_order >> 4) & 0b11;
    this->rgb_offsets_[2] = (u_order >> 2) & 0b11;
    this->rgb_offsets_[3] = (u_order >> 0) & 0b11;
  }
	void WordClock::update_time() {

		ESPTime time = time_id_->now();
		int h = time.hour;
		int m = time.minute;
		bool isChanged = false;
		bool birthday_changed = false;
		int size = this->size();

		//auto happy_birthday = id(happybirthday).current_values;


		// Brightness set by the light sensor
		//int brightness = 10;//(int)(fastledlight2->get_brightness()*255);
		long value = 100; //(long) (id(illuminance).sample() * 500);
		int scaledBrightness = map(value, 0, 500, 10, brightness);
		
		if (10 < abs(scaledBrightness - prevLightReading) ) {
			isChanged = true;
			prevLightReading = scaledBrightness;
		}

/*            int bdBrightness = happy_birthday.get_brightness() * 100;
		uint8 newRed = (happy_birthday.get_red());
		uint8 newGreen = (happy_birthday.get_green());
		uint8 newBlue = (happy_birthday.get_blue());
		if (bdred != newRed || bdblue != newBlue || bdgreen != newGreen) {
			bdred = newRed;
			bdblue = newBlue;
			bdgreen = newGreen;
			// brightness = newBrightness;
			birthday_changed = true;
		}

		if (isChanged || birthday_changed)
		{
		}
*/

		//uint32 color = pixels->getPixelColor(0);
		//newRed = getNthByte(color, 1);
		//newGreen = getNthByte(color, 2);
		//newBlue = getNthByte(color, 3);
		if (red != newRed || blue != newBlue || green != newGreen) {
			red = newRed;
			blue = newBlue;
			green = newGreen;
			// brightness = newBrightness;
			isChanged = true;
		}



		//check if valid time. Blink red,green,blue until valid time is present
		if (time.is_valid() == false) {
			ESP_LOGE("loop", "Got invalid time from current_time Time: %i:%i", h, m );
			for (int i = 0; i < 8; i++) {
				int random_number = random(1023);
				int rRed = random(255);
				int rGreen = random(255);
				int rBlue = random(255);
				if (random_number %2 == 1) {
					setPixelColor(thinking[i][0], rRed, rGreen, rBlue, scaledBrightness); show();
					delay(250);
					setPixelColor(thinking[i][0], 0, 0, 0, scaledBrightness);   show();
				} else {
					setPixelColor(thinking[i][1], rRed, rGreen, rBlue, scaledBrightness); show();
					delay(250);
					setPixelColor(thinking[i][1], 0, 0, 0, scaledBrightness);   show();
				}
			}
			// setPixelColor(0, 255, 0, 0, scaledBrightness); show(); delay(250);
			// setPixelColor(0, 0, 255, 0, scaledBrightness); show(); delay(250);
			// setPixelColor(0, 0, 0, 255, scaledBrightness); show(); delay(250);
			
			// cycle through the letters in "thinking"

		}
		// happy tit day
		// twenty four seven c ock
		// flashing between c ock and oclock
		// it is c ock
		//
		else {
			// only update once in a Minute
			if(h != ihour || m != iminute || isChanged) {
				ihour = h;
				iminute = m;
				if (ihour >= 0){
					int tmp_hour = ihour;
					int tmp_minute = (iminute - (iminute % 5));
					if(tmp_minute >= 35) { tmp_hour += 1; }
					tmp_minute = tmp_minute / 5;
					tmp_hour = tmp_hour % 12;
					int minutessum = iminute % 5;
					// Reset all LED
					for(int i = 0; i < size; i++) {  setPixelColor(i, 0, 0, 0, scaledBrightness); }

/* if (bdBrightness > 10) {
			for (int j = 0; j < 2; j++)
			{
				for(int i = 0; i < 8; i++) {
					if(leds_happy_birthday[j][i] >= 0) { setPixelColor(leds_happy_birthday[j][i], bdred, bdgreen, bdblue, scaledBrightness); }
				}
			}
} */

					for(int i = 0; i < 4; i++) {         setPixelColor(leds_time_it_is[i], red, green, blue, scaledBrightness); }
					for(int i = 0; i < 15; i++) {        if(leds_time_minutes[tmp_minute][i] >= 0) { setPixelColor(leds_time_minutes[tmp_minute][i], red, green, blue, scaledBrightness); } }
					for(int i = 0; i < 6; i++) {         if(leds_time_hours[tmp_hour][i] >= 0) { setPixelColor(leds_time_hours[tmp_hour][i], red, green, blue, scaledBrightness); } }
					show();
					// ESP_LOGD("loop", "Update Time: %i:%i  Brightness: %i RGB: %i-%i-%i", ihour, iminute, scaledBrightness, red, green, blue);
					// ESP_LOGD("loop", "Using tmp_hour: %i tmp_minute: %i minutessum: %i", tmp_hour, tmp_minute, minutessum);
				}
			}
		}
	}
		
        // void WordClock::setPixelColor( uint16_t i, uint8_t r, uint8_t g, uint8_t b, uint16_t uiBrightness) {
		// 	pixels->setPixelColor(i, pixels->Color((uiBrightness*r/255) , (uiBrightness*g/255), (uiBrightness*b/255)));
		// 	pixels->setPixelColor(i, pixels->Color(r , g, b));
        //     show();
		// }

	void WordClock::setPixelColor( uint16_t i, uint8_t r, uint8_t g, uint8_t b, uint16_t uiBrightness) {
		// pixels->setPixelColor(i, pixels->Color((uiBrightness*r/255) , (uiBrightness*g/255), (uiBrightness*b/255)));
		controller_->SetPixelColor(i, new RgbColor(r , g, b));
		show();
		ESP_LOGI("setPixelColor", "Changing Colors");
		// newRed = r;
		// newGreen = g;
		// newBlue = b;
		// pixels->setBrightness(uiBrightness);
		// brightness = uiBrightness;
	}
	void WordClock::show() {
		controller_->Show();
	}

	void WordClock::clearStrip() {
		controller_->ClearTo(new RgbColor(0,0,0));
	}

 protected:
  NeoPixelBus<T_COLOR_FEATURE, T_METHOD> *controller_{nullptr};
  uint8_t *effect_data_{nullptr};
  uint8_t rgb_offsets_[4]{0, 1, 2, 3};
};

template<typename T_METHOD, typename T_COLOR_FEATURE = NeoRgbFeature>
class WordClockRGBLightOutput : public WordClockLightOutputBase<T_METHOD, T_COLOR_FEATURE> {
 public:
  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
  }

 protected:
  light::ESPColorView get_view_internal(int32_t index) const override {  // NOLINT
    uint8_t *base = this->controller_->Pixels() + 3ULL * index;
    return light::ESPColorView(base + this->rgb_offsets_[0], base + this->rgb_offsets_[1], base + this->rgb_offsets_[2],
                               nullptr, this->effect_data_ + index, &this->correction_);
  }
};

template<typename T_METHOD, typename T_COLOR_FEATURE = NeoRgbwFeature>
class WordClockRBGWLightOutput : public WordClockLightOutputBase<T_METHOD, T_COLOR_FEATURE> {
 public:
  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB_WHITE});
    return traits;
  }

 protected:
  light::ESPColorView get_view_internal(int32_t index) const override {  // NOLINT
    uint8_t *base = this->controller_->Pixels() + 4ULL * index;
    return light::ESPColorView(base + this->rgb_offsets_[0], base + this->rgb_offsets_[1], base + this->rgb_offsets_[2],
                               base + this->rgb_offsets_[3], this->effect_data_ + index, &this->correction_);
  }
};

}  // namespace neopixelbus
}  // namespace esphome

#endif  // USE_ARDUINO