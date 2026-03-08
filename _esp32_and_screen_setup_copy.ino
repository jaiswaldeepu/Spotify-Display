#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>

const char* SSID = "Galaxy_F14_5G_49D9";
const char* PASSWORD = "123412340";

const char* CLIENT_ID = "3802e647269b4075a08c47723c0dbba9";
const char* CLIENT_SECRET = "2b935bf9f6c84fa79a2d64631c87a4d5";

String lastArtist;
String lastTrackname;

#define TFT_CS   1
#define TFT_RST  2
#define TFT_DC   3
#define TFT_SCLK 4
#define TFT_MOSI 5

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
Spotify sp(CLIENT_ID, CLIENT_SECRET);

void setup() {
  Serial.begin(115200);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  Serial.println("TFT Initialized!");

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  tft.setCursor(0, 0);
  tft.write(WiFi.localIP().toString().c_str());  

  sp.begin();
  while (!sp.is_auth()) {
    sp.handle_client();
  }
  Serial.println("Connected to Spotify!");
} 

void loop() {
  String currentArtist = sp.current_artist_names();
  String currentTrackname = sp.current_track_name();

  if (lastArtist != currentArtist && currentArtist != "Something went wrong" && !currentArtist.isEmpty()) {
    tft.fillScreen(ST77XX_BLACK);
    lastArtist = currentArtist;
    Serial.println("Artist: " + lastArtist);
    tft.setCursor(10, 10);
    tft.write(lastArtist.c_str());  
  }

  if (lastTrackname != currentTrackname && currentTrackname != "Something went wrong" && currentTrackname != "null") {
    lastTrackname = currentTrackname;
    Serial.println("Track: " + lastTrackname);
    tft.setCursor(10, 40);
    tft.write(lastTrackname.c_str());  
  }

  delay(2000);
}