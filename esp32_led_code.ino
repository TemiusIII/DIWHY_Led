#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <FastLED.h>


#define NUM_LEDS 899
#define PIN_LED 5
CRGB leds[NUM_LEDS];

const char* ssid = "Vitalix_Net_TP-LINK";
const char* password = "vit_4957840750";

//Your Domain name with URL path or IP address with path
int led_mode = 2;
int speed = 30;
long color = 0xFFFFFF;
String led_link = "http://192.168.1.8:8080/led_mode";
String color_link = "http://192.168.1.8:8080/color";
String speed_link = "http://192.168.1.8:8080/speed";


void white(){
  for (int j = 0; j < 2; ++j){
    for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB::White;
      }

      FastLED.show();
      delay(30);
  }


}

void black(){
  for (int j = 0; j < 2; ++j){
    for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB::Black;
    }

    FastLED.show();
    delay(30);
  }

}

void fill_color(){
    for (int j = 0; j < 2; ++j){
      for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = color;
      }
      FastLED.show();
      delay(30);
    }
}


void strab(){
  for (int i = 0; i < 10; ++i){
    for (int j = 0; j < NUM_LEDS; ++j){
    leds[j] = CRGB::White;

  }
  FastLED.show();
  delay(speed);


  for (int j = 0; j < NUM_LEDS; ++j){
    leds[j] = CRGB::Black;

  }
  FastLED.show();
  delay(speed);
  }
}


void new_year(int wait = speed, int amount = 10){

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

void rainbow(int wait = speed, int BRIGHTNESS = 255 , int SATURATION = 255) {
  for (int j = 0; j < 255; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(i - (j * 2), SATURATION, BRIGHTNESS);
    }
    FastLED.show();
    delay(wait);
  }
}



String get_data(String link){
if(WiFi.status()== WL_CONNECTED){
  WiFiClient client;
  HTTPClient http;

  String serverPath = link;

        // Your Domain name with URL path or IP address with path
  http.begin(client, serverPath.c_str());

        // If you need Node-RED/server authentication, insert user and password below
        //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

        // Send HTTP GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode>0) {
    // Serial.print("HTTP Response code: ");
    // Serial.println(httpResponseCode);
    String payload = http.getString();
    return(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);

  }
  // Free resources
  http.end();
} else {
  Serial.println("Error ebat");

}

}



void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, PIN_LED>(leds, NUM_LEDS);
  FastLED.setBrightness(100);

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

void update_variables(){
  speed = get_data(speed_link).toInt();
  led_mode = get_data(led_link).toInt();
  color = strtol(get_data(color_link).c_str(), NULL, 16);
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

}
// Serial.println(led_mode);
// delay(100);

}