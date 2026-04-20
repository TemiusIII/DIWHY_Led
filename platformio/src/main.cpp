#include <ArduinoJson.h>
#include <FastLED.h>
#include <WebSocketsClient.h>
#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#else
#error "Only esp32 and esp8266 are supported by default"
#endif

#define NUM_LEDS 865       // Amount of LEDs in strip
#define PIN_LED 5          // Digital pin of LED (Din)
#define LED_BRIGHTNESS 200 // brigtness of LEDs, TODO: add control via server
#define RIPPLE_SPLIT 4     // how much you want to split ripple effect show

#define SERVER_URL "HOST IP" // something like 192.168.1.150
#define SERVER_PORT 8080     // flask server port

CRGB leds[NUM_LEDS];

const char *ssid = "WIFI SSID";
const char *password = "WIFI PASSWORD";

// some structs for fun
struct hsv_color {
  int hue;
  int saturation;
  int value;
};

struct ripple_params {
  int hue_gap;
  int hue_range;
  int ripple_step;
};

int counter = 0;
int led_mode = 0;
int led_speed = 30;
int new_year_start = 0;
bool new_year_start_color = 1;
long color = 0xFFFFFF;
unsigned long last_update = 0;

hsv_color color_hsv = {0, 0, 100};
ripple_params ripple_settings = {0, 0, 0};

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
  case WStype_CONNECTED:
    Serial.println("Connected to websocket");
    break;

  case WStype_TEXT:
    JsonDocument ws_data;
    DeserializationError error = deserializeJson(ws_data, payload);
    if (!error) {
      JsonObject data = ws_data.as<JsonObject>();
      led_mode = data["led_mode"] | led_mode;
      led_speed = data["speed"] | led_speed;
      color = data["custom_color"] | color;

      JsonObject ripple = data["ripple"];
      if (!ripple.isNull()) {
        ripple_settings.hue_gap = ripple["hue_gap"] | ripple_settings.hue_gap;
        ripple_settings.hue_range =
            ripple["hue_range"] | ripple_settings.hue_range;
        ripple_settings.ripple_step =
            ripple["ripple_step"] | ripple_settings.ripple_step;
      }
    } else {
      Serial.printf("JSON error: %s\n", error.c_str());
    }
    break;
  }
}

void white() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
  }

  FastLED.show();
}

void black() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }

  FastLED.show();
}

void fill_color() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }

  FastLED.show();
}

bool strab_flag = false; // for non-blocking cycles
void strab() {
  if (strab_flag) {
    for (int j = 0; j < NUM_LEDS; ++j) {
      leds[j] = CRGB::White;
    }
  } else {
    for (int j = 0; j < NUM_LEDS; ++j) {
      leds[j] = CRGB::Black;
    }
  }

  strab_flag = !strab_flag;
  FastLED.show();
}

void new_year(int amount = 10) {
  CRGB colors[2] = {CRGB::Red, CRGB::White};
  int cnt = new_year_start++;
  bool red = new_year_start_color;
  if (new_year_start == amount) {
    new_year_start = 0;
    new_year_start_color = !new_year_start_color;
  }

  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = colors[red];
    if (++cnt == amount) {
      cnt = 0;
      red = !red;
    }
  }
  FastLED.show();
}

int rainbow_offset = 0; // for non-blocking cycles
void rainbow(int BRIGHTNESS = 255, int SATURATION = 255) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(i - (rainbow_offset * 2), SATURATION, BRIGHTNESS);
  }

  rainbow_offset = (rainbow_offset + 1) % 255;
  FastLED.show();
}

void rippleTick() {
  int temp;
  if (RIPPLE_SPLIT) { // ESP32 can't handle this mode with big LED_NUM, so you
                      // can split it into chunks(looks not that great)
    for (int k = 0; k < RIPPLE_SPLIT; ++k) {
      for (int i = k * int(NUM_LEDS / RIPPLE_SPLIT);
           i < (k + 1) * int(NUM_LEDS / RIPPLE_SPLIT); i++) {

        temp = inoise8(i * ripple_settings.ripple_step, counter);
        leds[i] = CHSV(
            map(color_hsv.hue, 0, 360, 0, 255) +
                map(temp, 20, 60, 0, ripple_settings.hue_gap), // H
            constrain(map(temp, 20, 60,
                          map(min(100, color_hsv.saturation +
                                           ripple_settings.hue_range),
                              0, 100, 0, 255),
                          map(max(0, color_hsv.saturation -
                                         ripple_settings.hue_range),
                              0, 100, 0, 255)),
                      0, 255), // S
            constrain(
                map(temp, 20, 60,
                    map(max(0, color_hsv.value - ripple_settings.hue_range), 0,
                        100, 0, 255),
                    map(min(100, color_hsv.value + ripple_settings.hue_range),
                        0, 100, 0, 255)),
                0, 255) // V
        );
        counter += 20;
      }
      FastLED.show();
    }
  } else {
    for (int i = 0; i < NUM_LEDS; i++) {

      temp = inoise8(i * ripple_settings.ripple_step, counter);
      leds[i] = CHSV(
          map(color_hsv.hue, 0, 360, 0, 255) +
              map(temp, 20, 60, 0, ripple_settings.hue_gap), // H
          constrain(
              map(temp, 20, 60,
                  map(min(100,
                          color_hsv.saturation + ripple_settings.hue_range),
                      0, 100, 0, 255),
                  map(max(0, color_hsv.saturation - ripple_settings.hue_range),
                      0, 100, 0, 255)),
              0, 255), // S
          constrain(
              map(temp, 20, 60,
                  map(max(0, color_hsv.value - ripple_settings.hue_range), 0,
                      100, 0, 255),
                  map(min(100, color_hsv.value + ripple_settings.hue_range), 0,
                      100, 0, 255)),
              0, 255) // V
      );
      counter += 20;
    }
    FastLED.show();
  }
}

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, PIN_LED>(leds, NUM_LEDS);
  FastLED.setBrightness(LED_BRIGHTNESS);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  webSocket.begin(SERVER_URL, SERVER_PORT, "/ws");
  webSocket.onEvent(webSocketEvent);
  webSocket.enableHeartbeat(3000, 1000, 3);
  webSocket.setReconnectInterval(3000);

  last_update = millis(); // Using that for non-blocking delays, cause webSocket
                          // won't work with delay()
}

void loop() {
  webSocket.loop();

  if (millis() - last_update > led_speed) { // non-blocking speed delay
    last_update = millis();

    switch (led_mode) {
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
    default:
      led_mode = 0;
      break;
    }
  }

  // Can be used for debug, so left here
  // Serial.println(led_mode);
  // delay(200); // For debug
  // Serial.println(color);
  // Serial.println(String(color_hsv.hue) + "   " + String(color_hsv.saturation)
  // + "   " + String(color_hsv.value));
  // Serial.println(String(ripple_settings.hue_gap) + "   " +
  // String(ripple_settings.hue_range) + "   " +
  // String(ripple_settings.ripple_step)); Serial.println("\n\n\n\n\n");
}
