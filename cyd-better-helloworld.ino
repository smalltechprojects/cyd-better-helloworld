#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

TFT_eSPI tft = TFT_eSPI();

// Callback function used by the decoder to "push" image blocks to the screen
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  // Stop rendering if the image goes off-screen
  if (y >= tft.height()) return false;
  tft.pushImage(x, y, w, h, bitmap);
  return true;
}

void setup() {
  Serial.begin(115200);

  // Turn on the Backlight (CYD pin 21)
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);

  // Initialize Display
  tft.begin();
  tft.setRotation(1); // 1 = Landscape (USB port on the right)
  tft.fillScreen(TFT_BLACK);

  // Initialize SD Card (Pin 5 is the CS pin for the CYD SD slot)
  if (!SD.begin(5)) {
    Serial.println("SD Card Mount Failed!");
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_RED);
    tft.println("Insert SD Card!");
    while (1) delay(1000);
  }

  // Configure the JPEG Decoder
  TJpgDec.setJpgScale(1);      // 1:1 scale
  TJpgDec.setSwapBytes(true);  // Corrects color byte order for ESP32
  TJpgDec.setCallback(tft_output);

  //print hello world
  Serial.println("Setup Complete.");
  tft.setCursor(10,50);
  tft.println("hello world");
  delay(4000);

//printer hello world centered
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  int screenWidth = tft.width();
  int screenHeight = tft.height();
  tft.drawCentreString("hello world centered",tft.width()/2, tft.height()/2-10, 2);
  delay(4000);

 // Display Image 1
  Serial.println("Loading helloworld.jpg...");
  tft.fillScreen(TFT_BLACK);
  // Parameters: X, Y, Filename
  TJpgDec.drawSdJpg(0, 0, "/helloworld.jpg");
  delay(4000);

}

void loop() {
 
}