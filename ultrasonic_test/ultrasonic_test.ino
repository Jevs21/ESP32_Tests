#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <NewPing.h>

// #include <HTTPClient.h>

// const char* ssid = "CheZ Lounge 2.0";
// const char* password = "Chez12345";

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const int buzzerPin = 5; // The GPIO pin you connected the buzzer to
#define maxDistance 50

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
NewPing sonar(32, 33, maxDistance);
void setup() {
  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as an output
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

  displayStatus("Hello!");
  delay(100);
}

float modulation = 50.0;

void loop() {
  int distance = sonar.ping_cm();
  int frequency = mapDistanceToFrequency(distance);
  float modFreq = frequency + sin(millis() / 1000.0 * modulation) * modulation;
  displayDistance(distance);

  playTone(frequency, 100);
  delay(10); // Wait for 5 seconds before scanning again
}

void displayStatus(const char* message) {
  display.clearDisplay(); // Clear the buffer
  display.setCursor(0, 0); // Set the cursor position
  display.setTextSize(1); // Set the text size
  display.setTextColor(WHITE); // Set the text color
  display.println(message); // Print the message
  display.display(); // Update the display
}

void displayDistance(int dist) {
  display.clearDisplay(); // Clear the buffer

  String distStr = "Distance: " + String(dist);
  int textWidth = distStr.length() * 6; // Width of the text in pixels (6 pixels per character)
  int cursorX = (SCREEN_WIDTH - textWidth) / 2; // Calculate the X position for centering the text
  int cursorY = (SCREEN_HEIGHT / 2) - 4; // Calculate the Y position for centering the text

  display.setCursor(cursorX, cursorY); // Set the cursor position
  display.setTextSize(1); // Set the text size
  display.setTextColor(WHITE); // Set the text color
  display.println(distStr); // Print the distance string

  int radius = map(dist, 0, 20, 0, min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2); // Map distance (0 to 20 cm) to radius (0 to half of the minimum display dimension)
  int circleX = SCREEN_WIDTH / 2; // Calculate the X position for centering the circle
  int circleY = SCREEN_HEIGHT / 2; // Calculate the Y position for centering the circle

  display.drawCircle(circleX, circleY, radius, WHITE); // Draw the circle around the distance string

  display.display(); // Update the display
}

void playTone(int freq, int duration) {
  if (freq <= 0) {
    delay(duration); // If frequency is not positive, just wait for the duration
  } else {
    int period = 1000000 / freq;
    int pulse = period / 2;

    for (int i = 0; i < (duration * 1000) / period; i++) {
      digitalWrite(buzzerPin, HIGH);
      delayMicroseconds(pulse);
      digitalWrite(buzzerPin, LOW);
      delayMicroseconds(pulse);
    }
  }
}

int mapDistanceToFrequency(int distance) {
  // Map the distance to a frequency range, e.g., 100Hz to 5000Hz
  return map(distance, 0, maxDistance, 100, 5000);
}