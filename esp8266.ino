#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <FastLED.h>

#define NUM_LEDS 865 // Amount of LEDs in strip
#define PIN_LED 15     // Digital pin
#define LED_BRIGHTNESS 100
#define RIPPLE_SPLIT 4 // how much you want to split ripple effect show(its too heavy for big strips) 0 to not split


#define SERVER_URL actual_server_url


CRGB leds[NUM_LEDS];

const char *ssid = "YOUR_SSID";
const char *password = "YOUR_PASSWORD";
// some structs for fun
struct hsv_color{
    int hue;
    int saturation;
    int value;
};

struct ripple_params{
    int hue_gap;
    int hue_range;
    int ripple_step;
};

// Your Domain name with URL path or IP address with path
int counter = 0;
int led_mode = 0;
int led_speed = 30;
long color = 0xFFFFFF;
String led_link = String("http://") + SERVER_URL + "/led_mode";
String color_link = String("http://") + SERVER_URL + "/color";
String color_hsv_link = String("http://") + SERVER_URL + "/color_hsv";
String led_speed_link = String("http://") + SERVER_URL + "/speed";
String ripple_val_link = String("http://") + SERVER_URL + "/ripple_values";

hsv_color color_hsv = {0, 0, 100};
ripple_params ripple_settings = {0, 0, 0};

void white(){
  for (int j = 0; j < 2; ++j){
    for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB::White;
      }

      FastLED.show();
      delay(led_speed);
  }


}

void black(){
  for (int j = 0; j < 2; ++j){
    for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB::Black;
    }

    FastLED.show();
    delay(led_speed);
  }

}

void fill_color(){
    for (int j = 0; j < 2; ++j){
      for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = color;
      }
      FastLED.show();
      delay(led_speed);
    }
}


void strab(){
  for (int i = 0; i < 10; ++i){
    for (int j = 0; j < NUM_LEDS; ++j){
    leds[j] = CRGB::White;

  }
  FastLED.show();
  delay(led_speed);


  for (int j = 0; j < NUM_LEDS; ++j){
    leds[j] = CRGB::Black;

  }
  FastLED.show();
  delay(led_speed);
  }
}


void new_year(int wait = led_speed, int amount = 10) {

    CRGB colors[2] = {CRGB::Red,CRGB::White};



    int cnt = 0;
    bool red = 1;

    for (int i = 0; i < amount; ++i){
      for (int j = 0; j < NUM_LEDS; ++j){
        leds[j] = colors[1];
      }
      for (int j = i; j < NUM_LEDS; ++j){
          if (red){
            leds[j] = colors[0];
          } else {
            leds[j] = colors[1];
          }

          cnt += 1;

          if (cnt == amount){
            cnt = 0;
            red = !red;
          }
        }
        FastLED.show();
          delay(wait);
      }
}

void rainbow(int wait = led_speed, int BRIGHTNESS = 255 , int SATURATION = 255) {
  for (int j = 0; j < 255; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(i - (j * 2), SATURATION, BRIGHTNESS);
    }
    FastLED.show();
    delay(wait);
  }
}

void rippleTick() {
    int temp;
    if (RIPPLE_SPLIT) {
        for (int k = 0; k < RIPPLE_SPLIT; ++k) {
          for(int i = k * int(NUM_LEDS / RIPPLE_SPLIT); i < (k+1) * int(NUM_LEDS / RIPPLE_SPLIT); i++) {

            temp = inoise8(i * ripple_settings.ripple_step, counter);
            leds[i] = CHSV(
                 map(color_hsv.hue, 0, 360, 0, 255) + map(temp, 20, 60, 0, ripple_settings.hue_gap), // H
                 constrain(map(temp, 20, 60, map(min(100, color_hsv.saturation + ripple_settings.hue_range), 0, 100, 0, 255),
                  map(max(0, color_hsv.saturation - ripple_settings.hue_range), 0, 100, 0, 255)), 0, 255), // S
                 constrain(map(temp, 20, 60, map(max(0, color_hsv.value - ripple_settings.hue_range), 0, 100, 0, 255),
                  map(min(100, color_hsv.value + ripple_settings.hue_range), 0, 100, 0, 255)), 0, 255)  // V
               );
            counter += 20;

          }
            FastLED.show();
        }
    } else {
        for(int i = 0; i < NUM_LEDS; i++) {

            temp = inoise8(i * ripple_settings.ripple_step, counter);
            leds[i] = CHSV(
                 map(color_hsv.hue, 0, 360, 0, 255) + map(temp, 20, 60, 0, ripple_settings.hue_gap), // H
                 constrain(map(temp, 20, 60, map(min(100, color_hsv.saturation + ripple_settings.hue_range), 0, 100, 0, 255),
                  map(max(0, color_hsv.saturation - ripple_settings.hue_range), 0, 100, 0, 255)), 0, 255), // S
                 constrain(map(temp, 20, 60, map(max(0, color_hsv.value - ripple_settings.hue_range), 0, 100, 0, 255),
                  map(min(100, color_hsv.value + ripple_settings.hue_range), 0, 100, 0, 255)), 0, 255)  // V
               );
            counter += 20;

        }
            FastLED.show();
    }

    //  delay(led_speed);
}


String get_data(String link){
if(WiFi.status()== WL_CONNECTED){
  WiFiClient client;
  HTTPClient http;

  http.begin(client, link.c_str());

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String payload = http.getString();
    return(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);

  }
  // Free resources
  http.end();
} else {
  Serial.println("Unknown Error");

    }
    return "0xFFFFFF";
}

void update_hsv(){
    String temp = get_data(color_hsv_link);
    // Serial.println("HSV raw data " + temp);
    int space1 = temp.indexOf(' ');
    if (space1 == -1) {
        Serial.println("Error while fetching hue color");
        return;
    }

    int space2(temp.indexOf(' ', space1 + 1));
    if (space2 == -1) {
        Serial.println("Error while fetching hue color");
        return;
    }

    color_hsv.hue = temp.substring(0, space1).toInt();
    color_hsv.saturation = temp.substring(space1 + 1, space2).toInt();
    color_hsv.value = temp.substring(space2 + 1).toInt();
}

void update_ripple_settings(){
    String temp = get_data(ripple_val_link);
    // Serial.println("Ripple settings raw data " + temp);
    int space1 = temp.indexOf(' ');
    if (space1 == -1) {
        Serial.println("Error while fetching ripple settings");
        return;
    }

    int space2(temp.indexOf(' ', space1 + 1));
    if (space2 == -1) {
        Serial.println("Error while fetching ripple settings");
        return;
    }

    ripple_settings.hue_gap = temp.substring(0, space1).toInt();
    ripple_settings.hue_range = temp.substring(space1 + 1, space2).toInt();
    ripple_settings.ripple_step = temp.substring(space2 + 1).toInt();

}



void update_variables() {
  led_speed = get_data(led_speed_link).toInt();
  led_mode = get_data(led_link).toInt();

   if (led_mode  == 5 || led_mode == 6)
    color = strtol(get_data(color_link).c_str(), NULL, 16);

   if (led_mode == 6) {
    update_hsv();
    update_ripple_settings();
   }
 }

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, PIN_LED>(leds, NUM_LEDS);
  FastLED.setBrightness(LED_BRIGHTNESS);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());


}

void loop() {
  update_variables();

  switch(led_mode){
    case 0:
      black();
      break;
    case 1:
      white();
      break;
    case 2:
      new_year();
      break;
    case 3:
      rainbow();
      break;
    case 4:
      strab();
      break;
    case 5:
      fill_color();
      break;
    case 6:
      rippleTick();
      break;
  }
// Serial.println(led_mode);
//delay(200); // For debug
//Serial.println(color);
//Serial.println(String(color_hsv.hue) + "   " + String(color_hsv.saturation) + "   " + String(color_hsv.value));
//Serial.println(String(ripple_settings.hue_gap) + "   " + String(ripple_settings.hue_range) + "   " + String(ripple_settings.ripple_step));
//Serial.println("\n\n\n\n\n");
}