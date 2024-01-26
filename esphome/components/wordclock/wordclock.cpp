#include "esphome.h"
#include <Adafruit_NeoPixel.h>
#include "esphome/core/log.h"
#include "wordclock.h"
//#include <TimeLib.h>     // for update/display of time

namespace esphome {
namespace wordclock {

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

static const char *TAG = "wordclock.WordClock";
        void WordClock::setup() {
            startStrip();
            clearStrip();
            brightness = 50; // half brightness

            // Start all LED with on and default color and brightness to check if everything is working...
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, red, 0, 0, brightness); show(); delay(10); }
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, 0, green, 0, brightness); show(); delay(10); }
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, 0, 0, blue, brightness); show(); delay(10); }
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, 0, 0, 0, brightness); }
            show();
            randomSeed(analogRead(0));
            //register_service(&WordClock::on_setled, "setled", {"number","red", "blue", "green"});
            Serial.println("WordClock Setup Complete!");
        }
		
        void WordClock::on_setled(int number, int red, int blue, int green) {

            if (number < NUM_LEDS || number > 0) {
                ESP_LOGD("setled", "Setting led number %d to color %i %i %i", number, red, green, blue );
                setPixelColor(number, red, green, blue, brightness);
                show();
            }
            else {
                ESP_LOGE("setled", "Not a valid LED Number - out of range");
            }
        }
		
		
		light::LightTraits WordClock::get_traits() {
			auto traits = light::LightTraits();
			traits.set_supported_color_modes({light::ColorMode::RGB});
//			traits.set_supports_brightness(true); // these are deprecated and gone
//			traits.set_supports_rgb(true);
//			traits.set_supports_rgb_white_value(false);
//			traits.set_supports_color_temperature(false);
			return traits;
		}

		void WordClock::write_state(light::LightState *state) {

		}
		
		
		void WordClock::dump_config() {
			ESP_LOGCONFIG(TAG, "WordClock");
		}
		
        void WordClock::loop() {
            auto time = now();
            int h = time.hour;
            int m = time.minute;
            bool isChanged = false;
            bool birthday_changed = false;
            //auto fastledlight2 = clock_face; //id(clockface).current_values;
            //auto happy_birthday = id(happybirthday).current_values;

int newRed;
int newBlue;
int newGreen;

            // Brightness set by the light sensor
            int brightness = 10;//(int)(fastledlight2->get_brightness()*255);
            long value = 100; //(long) (id(illuminance).sample() * 500);
            int scaledBrightness = map(value, 0, 500, 10, brightness);
            
            if (10 < abs(scaledBrightness - prevLightReading) ) {
                isChanged = true;
                prevLightReading = scaledBrightness;
            }

/*            int bdBrightness = happy_birthday.get_brightness() * 100;
            int newRed = (int)(happy_birthday.get_red()*255);
            int newGreen = (int)(happy_birthday.get_green()*255);
            int newBlue = (int)(happy_birthday.get_blue()*255);
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


            //convert float 0.0 till 1.0 into int 0 till 255
            newRed = 100; //(int)(fastledlight2->get(0).get_red()*255);
            newGreen = 100; //(int)(fastledlight2->get(0).get_green()*255);
            newBlue = 100; //(int)(fastledlight2->get(0).get_blue()*255);
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
                if(h != hour || m != minute || isChanged) {
                    hour = h;
                    minute = m;
                    if (hour >= 0 && time.is_valid() == true){
                        int tmp_hour = hour;
                        int tmp_minute = (minute - (minute % 5));
                        if(tmp_minute >= 35) { tmp_hour += 1; }
                        tmp_minute = tmp_minute / 5;
                        tmp_hour = tmp_hour % 12;
                        int minutessum = minute % 5;
                        // Reset all LED
                        for(int i = 0; i < NUM_LEDS; i++) {  setPixelColor(i, 0, 0, 0, scaledBrightness); }

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
                        // ESP_LOGD("loop", "Update Time: %i:%i  Brightness: %i RGB: %i-%i-%i", hour, minute, scaledBrightness, red, green, blue);
                        // ESP_LOGD("loop", "Using tmp_hour: %i tmp_minute: %i minutessum: %i", tmp_hour, tmp_minute, minutessum);
                    }
                }
            }
        }
		
		void WordClock::setPixelColor( uint16_t i, uint8_t r, uint8_t g, uint8_t b, uint16_t uiBrightness) {
			pixels.setPixelColor(i, pixels.Color((uiBrightness*r/255) , (uiBrightness*g/255), (uiBrightness*b/255)));
		}
		void WordClock::show() {
			pixels.show();
		}

		void WordClock::startStrip() {
			pixels.begin();
		}

		void WordClock::clearStrip() {
			pixels.clear();
		}

} // namespace WordClock
} // namespace esphome