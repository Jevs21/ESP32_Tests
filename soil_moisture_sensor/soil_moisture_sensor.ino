#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SOIL_SENSOR_PIN 36 // Soil moisture sensor connected to A0 (GPIO 36)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

static const unsigned char PROGMEM waterDroplet_bmp[] = {
0b00000001, 0b00000000, 
0b00000001, 0b00000000, 
0b00000010, 0b10000000, 
0b00000010, 0b10000000, 
0b00000100, 0b01000000, 
0b00000100, 0b01000000, 
0b00001000, 0b00100000, 
0b00001000, 0b00100000, 
0b00010000, 0b00010000, 
0b00010000, 0b00010000, 
0b00010000, 0b01010000, 
0b00010000, 0b10010000, 
0b00001000, 0b00100000, 
0b00000111, 0b11000000, 
};
void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
}

void loop() {
  // int soilMoisture = analogRead(SOIL_SENSOR_PIN); // Read soil moisture value
  int soilMoisture = getAverageSoilMoisture(10);
  int norm = normalizeSoilMoisture(soilMoisture, 2000, 3600);

  Serial.println(norm);
  displayMoisture(norm);
  
 

  delay(1000); // Update every second
}

void displayMoisture(int val) {
  display.clearDisplay(); // Clear the buffer

  String valStr = "Moisture: " + String(val) + "%";
  int textWidth = valStr.length() * 6; // Width of the text in pixels (6 pixels per character)
  int cursorX = (SCREEN_WIDTH - textWidth) / 2; // Calculate the X position for centering the text
  int cursorY = (SCREEN_HEIGHT / 2) - 20; // Calculate the Y position for centering the text

  display.setCursor(cursorX, cursorY); // Set the cursor position
  display.setTextSize(1); // Set the text size
  display.setTextColor(WHITE); // Set the text color
  display.println(valStr); // Print the valance string
  display.drawBitmap((SCREEN_WIDTH / 2) - 14, (SCREEN_HEIGHT - 16) / 2, waterDroplet_bmp, 16, 14, WHITE); // Display water droplet

  display.display(); // Update the display
}

int getAverageSoilMoisture(int samples) {
  int sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(SOIL_SENSOR_PIN);
    delay(10);
  }
  return sum / samples;
}

int normalizeSoilMoisture(int rawValue, int minValue, int maxValue) {
  int normalizedValue = map(rawValue, minValue, maxValue, 100, 0);
  normalizedValue = constrain(normalizedValue, 0, 100); // Ensure the value stays within the 0-100 range
  return normalizedValue;
}
