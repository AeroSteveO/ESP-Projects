#include "esphome.h"
#include <NeoPixelBus.h>
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 121
#define DATA_PIN 2

// esphome dependencies:
// needs: esphome time --> id: current_time
// needs: esphome NeoPixelBus --> id: clockface

int leds_time_it_is[] = {111,112,114,115}; // It Is
// int leds_minutes[] = {121, 121, 121, 121}; // Minutes LEDS
// int leds_time_minutes[][15] = {
//     {101, 100,  99,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // OClock
//     {  7,   8,   9,  10,  41,  40,  39,  38,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // five past
//     { 21,  20,  19,  18,  41,  40,  39,  38,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // ten past
//     { 26,  27,  28,  29,  30,  31,  32,  41,  40,  39,  38,  -1,  -1,  -1,  -1}, // quarter after
//     { 17,  16,  15,  15,  14,  13,  12,  11,  41,  40,  39,  38,  -1,  -1,  -1}, // twenty past
//     {  7,   8,   9,  10,  37,  36,  35,  44,  45,  46,  47,  -1,  -1,  -1,  -1}, // 25 past
//     { 44,  45,  46,  47,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // half past
//     {  7,   8,   9,  10,  41,  40,  39,  38,  44,  45,  46,  47,  -1,  -1,  -1}, // FÜNF, NACH, HALB
//     { 17,  16,  15,  15,  14,  13,  12,  11,  37,  36,  35,  -1,  -1,  -1,  -1}, // twenty to
//     { 22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  -1,  -1,  -1,  -1}, // 25 to
//     { 21,  20,  19,  18,  37,  36,  35,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // quarter to
//     {  7,   8,   9,  10,  37,  36,  35,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}  // five to
// };

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

int hour = -1;
int minute = -1;
int red = 124;
int green = 124;
int blue = 124;
int brightness = 50; // half brightness

// NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(NUM_LEDS, DATA_PIN);
Adafruit_NeoPixel pixels(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);
// NeoPixelRGBLightOutput<NeoGrbFeature, NeoWs2812xMethod> strip(NUM_LEDS, DATA_PIN);

void setPixelColor( uint16_t i, uint8_t r, uint8_t g, uint8_t b, uint16_t uiBrightness) {
    // clockface.setPixelColor(i, r, g, b);
    // clockface.turn_on();
    // strip.SetPixelColor(i, RgbColor((uiBrightness*r/255) , (uiBrightness*g/255), (uiBrightness*b/255)));
    pixels.setPixelColor(i, pixels.Color((uiBrightness*r/255) , (uiBrightness*g/255), (uiBrightness*b/255)));
    //pixels.setBrightness(brightness);

}
void show() {
    pixels.show();
    // strip.Show();
}

void startStrip() {
    pixels.begin();
    // strip.Begin();
}

void clearStrip() {
    pixels.clear();
    // strip.ClearTo(0);
}

class Wordclock : public Component, public CustomAPIDevice {
    public:
        void setup() override {
            startStrip();
            clearStrip();
            brightness = 50; // half brightness

            //auto neopixellight = id(clockface);
            //neopixellight.turn_on();

            // Start all LED with on and default color and brightness to check if everything is working...
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, red, 0, 0, brightness); show(); delay(10); }
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, 0, green, 0, brightness); show(); delay(10); }
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, 0, 0, blue, brightness); show(); delay(10); }
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, 0, 0, 0, brightness); }
            show();
            register_service(&Wordclock::on_setled, "setled", {"number","red", "blue", "green"});
            Serial.println("WordClock Setup Complete!");
        }
        void on_setled(int number, int red, int blue, int green) {

            if (number < NUM_LEDS || number > 0) {
                ESP_LOGD("setled", "Setting led number %d to color %i %i %i", number, red, green, blue );
                setPixelColor(number, red, green, blue, brightness);
                show();
            }
            else {
                ESP_LOGE("setled", "Not a valid LED Number - out of range");
            }
        }
        void loop() override {
            auto time = id(current_time).now();
            int h = time.hour;
            int m = time.minute;
            // https://www.esphome.io/api/classesphome_1_1light_1_1_light_color_values.html LightColorValues Class
            // auto neopixellight = id(clockface).current_values;
            // //convert float 0.0 till 1.0 into int 0 till 255
            // red = (int)(neopixellight.get_red()*255);
            // green = (int)(neopixellight.get_green()*255);
            // blue = (int)(neopixellight.get_blue()*255);
            // brightness = 0;
            // //check if light is on and set brightness
            // if (neopixellight.get_state() > 0 ) {
            //     brightness = (int)(neopixellight.get_brightness()*255);
            //     // long value = (long) (id(illuminance).sample() * 500);
            //     // brightness = map(value, 0, 500, 10, 200);
            //     //ESP_LOGD("loop", "Brightness found as: %i", brightness);
            // }
            // else {
                // ESP_LOGD("loop", "clockface state off - b: %i rgb %i %i %i", brightness, red, green, blue); delay(500);
                //brightness = 128;
                long value = (long) (id(illuminance).sample() * 500);
                brightness = map(value, 0, 500, 10, 150);
                //pixels.setBrightness(brightness);
            // }
            //check if valid time. Blink red,green,blue until valid time is present
            if (time.is_valid() == false) {
                ESP_LOGE("loop", "Got invalid time from current_time Time: %i:%i", h, m );
                setPixelColor(0, 255, 0, 0, brightness); show(); delay(250);
                setPixelColor(0, 0, 255, 0, brightness); show(); delay(250);
                setPixelColor(0, 0, 0, 255, brightness); show(); delay(250);
                setPixelColor(0, 0, 0, 0, brightness);   show();
            }
            else {
                // only update once in a Minute
                if(h != hour || m != minute) {
                   //ESP_LOGD("loop", "Using b: %i rgb %i %i %i", brightness, red, green, blue);
                    hour = h;
                    minute = m;
                    if (hour >= 0 && time.is_valid() == true){
                        int tmp_hour = hour;
                        int tmp_minute = (minute - (minute % 5));
                        if(tmp_minute >= 25) { tmp_hour += 1; }
                        tmp_minute = tmp_minute / 5;
                        tmp_hour = tmp_hour % 12;
                        int minutessum = minute % 5;
                        // Reset all LED
                        for(int i = 0; i < NUM_LEDS; i++) {     setPixelColor(i, 0, 0, 0, brightness); }
                        for(int i = 0; i < 4; i++) {            setPixelColor(leds_time_it_is[i], red, green, blue, brightness); }
//                        for(int i = 0; i < 15; i++) {           if(leds_time_minutes[tmp_minute][i] >= 0) { setPixelColor(leds_time_minutes[tmp_minute][i], red, green, blue, brightness); } }
                        for(int i = 0; i < 6; i++) {            if(leds_time_hours[tmp_hour][i] >= 0) { setPixelColor(leds_time_hours[tmp_hour][i], red, green, blue, brightness); } }
//                        for(int i = 0; i < minutessum; i++) {   setPixelColor(leds_minutes[i], red, green, blue, brightness); }
                        show();
                        ESP_LOGD("loop", "Update Time: %i:%i  Brightness: %i RGB: %i-%i-%i", hour, minute, brightness, red, green, blue);
                        ESP_LOGD("loop", "Using tmp_hour: %i tmp_minute: %i minutessum: %i", tmp_hour, tmp_minute, minutessum);
                    }
                }
            }
        }
};
