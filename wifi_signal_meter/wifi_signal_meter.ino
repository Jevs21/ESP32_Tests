#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <WiFi.h>
// #include <HTTPClient.h>

// const char* ssid = "CheZ Lounge 2.0";
// const char* password = "Chez12345";

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // For ESP8266
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  displayStatus("Scanning Wi-fi (5s refresh)...");
}

void loop() {
  
  int n = WiFi.scanNetworks();
  
  display.clearDisplay();
  display.setCursor(0, 0);
  
  if (n == 0) {
    display.println("No networks found");
  } else {
    for (int i = 0; i < n; ++i) {
      display.print(i + 1);
      display.print(": ");
      display.print(WiFi.SSID(i));
      display.print(" (");
      display.print(WiFi.RSSI(i));
      display.println(")");
      delay(10);
    }
  }
  display.display();
  delay(5000); // Wait for 5 seconds before scanning again
}

void displayStatus(const char* message) {
  display.clearDisplay(); // Clear the buffer
  display.setCursor(0, 0); // Set the cursor position
  display.setTextSize(1); // Set the text size
  display.setTextColor(WHITE); // Set the text color
  display.println(message); // Print the message
  display.display(); // Update the display
}




