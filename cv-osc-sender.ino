// #define ARDUINOOSC_DEBUGLOG_ENABLE
//
#include <FastLED.h>
#define NUM_LEDS 1
#define DATA_PIN 18
CRGB leds[NUM_LEDS];

// Please include ArduinoOSCWiFi.h to use ArduinoOSC on the platform
// which can use both WiFi and Ethernet
#include <ArduinoOSCWiFi.h>
// this is also valid for other platforms which can use only WiFi
// #include <ArduinoOSC.h>

// WiFi stuff
const char* ssid = "xxxx";
const char* pwd = "xxxx";
const IPAddress ip(192, 168, 1, 250);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);

// for ArduinoOSC
const char* host = "192.168.1.134";
const int send_port = 10000;
const int publish_port = 10000;
int analogVal;

void setup() {
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
    Serial.begin(115200);
    Serial.println("Starting...");
    startupBlink();
    setLED(CRGB::Yellow);
    delay(2000);

    // WiFi stuff (no timeout setting for WiFi)
#ifdef ESP_PLATFORM
    WiFi.disconnect(true, true);  // disable wifi, erase ap info
    delay(1000);
    WiFi.mode(WIFI_STA);
#endif
    WiFi.begin(ssid, pwd);
    WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.print("WiFi connected, IP = ");
    Serial.println(WiFi.localIP());
    setLED(CRGB::Green);

    // publish osc messages (default publish rate = 30 [Hz])

    OscWiFi.publish(host, publish_port, "/publish/analogInput", analogVal)
        ->setFrameRate(60.f);
    OscWiFi.publish(host, publish_port, "/publish/func", &millis, &micros)
        ->setIntervalMsec(500.f);
}



void loop() {
   OscWiFi.update();  // should be called to receive + send osc
   analogVal = analogRead(32);
}

void setLED(CRGB color) {
  leds[0] = color;
  FastLED.show();
}

void startupBlink() {
  for(int i=0; i<3; i++) {
      setLED(CRGB::Purple);
      delay(200);
      setLED(CRGB::Black);
      delay(200);
  }
}

