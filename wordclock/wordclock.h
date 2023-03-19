#include "esphome.h"
#include <NeoPixelBus.h>

#define NUM_LEDS 121
#define DATA_PIN 2

// esphome dependencies:
// needs: esphome time --> id: current_time
// needs: esphome fastled --> id: fastledlight

int leds_time_it_is[] = {0, 1, 3, 4, 5}; // ES IST
int leds_minutes[] = {124, 123, 122, 121}; // Minutes LEDS
int leds_skiped[] = {110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120}; // Check Line 101
int leds_time_minutes[][15] = {
    {101, 100,  99,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // UHR
    {  7,   8,   9,  10,  41,  40,  39,  38,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // FÜNF, NACH
    { 21,  20,  19,  18,  41,  40,  39,  38,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // ZEHN, NACH
    { 26,  27,  28,  29,  30,  31,  32,  41,  40,  39,  38,  -1,  -1,  -1,  -1}, // VIERTEL, NACH
    { 17,  16,  15,  15,  14,  13,  12,  11,  41,  40,  39,  38,  -1,  -1,  -1}, // ZWANZIG, NACH
    {  7,   8,   9,  10,  37,  36,  35,  44,  45,  46,  47,  -1,  -1,  -1,  -1}, // FÜNF, VOR, HALB
    { 44,  45,  46,  47,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // HALB
    {  7,   8,   9,  10,  41,  40,  39,  38,  44,  45,  46,  47,  -1,  -1,  -1}, // FÜNF, NACH, HALB
    { 17,  16,  15,  15,  14,  13,  12,  11,  37,  36,  35,  -1,  -1,  -1,  -1}, // ZWANZIG, VOR
    { 22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  -1,  -1,  -1,  -1}, // DREIVIERTEL
    { 21,  20,  19,  18,  37,  36,  35,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // ZEHN, VOR
    {  7,   8,   9,  10,  37,  36,  35,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}  // FÜNF, VOR
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

//CRGB leds[NUM_LEDS];
int hour = -1;
int minute = -1;
int red = 124;
int green = 124;
int blue = 124;
int brightness = 128; // half brightness
//NeoPixelBusLg<NeoGrbFeature, NeoWs2812xMethod> strip(NUM_LEDS, DATA_PIN); // i guess this is not right?
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(NUM_LEDS, DATA_PIN);

void setPixelColor( uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint16_t brightness) {
	strip.SetPixelColor(n, RgbColor((brightness*r/255) , (brightness*g/255), (brightness*b/255)));
}

class Wordclock : public Component, public CustomAPIDevice {
    public:
        void setup() override {
            strip.Begin();
            // Start all LED with on and default color and brightness to check if everything is working...
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, red, 0, 0, brightness); strip.Show(); delay(10); }
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, 0, green, 0, brightness); strip.Show(); delay(10); }
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, 0, 0, blue, brightness); strip.Show(); delay(10); }
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, 0, 0, 0, brightness); }
            strip.Show();
            register_service(&Wordclock::on_setled, "setled", {"number","red", "blue", "green"});
        }
        void on_setled(int number, int red, int blue, int green) {
            if (number < NUM_LEDS || number > 0) {
                ESP_LOGD("setled", "Setting led number %d to color %i %i %i", number, red, green, blue );
                setPixelColor(number, red, green, blue, brightness);
                strip.Show();
                }
            else { ESP_LOGE("setled", "Not a valid LED Number - out of range"); }
        }
        void loop() override {
            auto time = id(current_time).now();
            int h = time.hour;
            int m = time.minute;
            // https://www.esphome.io/api/classesphome_1_1light_1_1_light_color_values.html LightColorValues Class
            auto fastledlight2 = id(clockface).current_values;
            //convert float 0.0 till 1.0 into int 0 till 255
            red = (int)(fastledlight2.get_red()*255);
            green = (int)(fastledlight2.get_green()*255);
            blue = (int)(fastledlight2.get_blue()*255);
            brightness = 0;
            //check if light is on and set brightness
            if (fastledlight2.get_state() > 0 ) { brightness = (int)(fastledlight2.get_brightness()*255); }
            else { ESP_LOGD("loop", "clockface state off - b: %i rgb %i %i %i", brightness, red, green, blue); delay(500);}
            //check if valid time. Blink red,green,blue until valid time is present
            if (time.is_valid() == false) {
                ESP_LOGE("loop", "Got invalid time from current_time Time: %i:%i", h, m );
                setPixelColor(0, 255, 0, 0, brightness); strip.Show(); delay(250);
                setPixelColor(0, 0, 255, 0, brightness); strip.Show(); delay(250);
                setPixelColor(0, 0, 0, 255, brightness); strip.Show(); delay(250);
                setPixelColor(0, 0, 0, 0, brightness);   strip.Show();
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
                        // Reset all LED, but skip LED 110 till 120
                        for(int i = 0; i < NUM_LEDS; i++) {     if(i < 110 || i > 120) setPixelColor(i, 0, 0, 0, brightness); }
                        for(int i = 0; i < 5; i++) {            setPixelColor(leds_time_it_is[i], red, green, blue, brightness); }
                        for(int i = 0; i < 15; i++) {           if(leds_time_minutes[tmp_minute][i] >= 0) { setPixelColor(leds_time_minutes[tmp_minute][i], red, green, blue, brightness); } }
                        for(int i = 0; i < 6; i++) {            if(leds_time_hours[tmp_hour][i] >= 0) { setPixelColor(leds_time_hours[tmp_hour][i], red, green, blue, brightness); } }
                        for(int i = 0; i < minutessum; i++) {   setPixelColor(leds_minutes[i], red, green, blue, brightness); }
                        strip.Show();
                        ESP_LOGD("loop", "Update Time: %i:%i  Brightness: %i RGB: %i-%i-%i", hour, minute, brightness, red, green, blue);
                        ESP_LOGD("loop", "Using tmp_hour: %i tmp_minute: %i minutessum: %i", tmp_hour, tmp_minute, minutessum);
                    }
                }
            }
        }
};
