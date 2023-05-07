#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// void setup() {
//   Serial.begin(9600);
//   Serial.println("Initializing...");

//   Wire.begin(); // Initialize the I2C bus
//   display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize the OLED display with the I2C address of 0x3C
//   display.display(); // Clear the display
//   Serial.println("Initialized.");
// }
int todayContributions = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  Wire.begin(); // Initialize the I2C bus
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize the OLED display with the I2C address of 0x3C
  display.display(); // Clear the display

  WiFi.begin(ssid, password); // Connect to Wi-Fi
  displayStatus("Connecting to Wi-Fi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  displayStatus("Connected to Wi-Fi");
  Serial.println("Connected to Wi-Fi.");

  todayContributions = getTodayContributions(githubUsername, githubToken);
  Serial.print("Today's contributions: ");
  Serial.println(todayContributions);
}

void displayStatus(const char* message) {
  display.clearDisplay(); // Clear the buffer
  display.setCursor(0, 0); // Set the cursor position
  display.setTextSize(1); // Set the text size
  display.setTextColor(WHITE); // Set the text color
  display.println(message); // Print the message
  display.display(); // Update the display
}

void loop() {
  // Serial.println("Printing message...");
  display.clearDisplay(); // Clear the buffer
  display.setCursor(0, 0); // Set the cursor position
  display.setTextSize(1); // Set the text size
  display.setTextColor(WHITE); // Set the text color
  display.println("Hello, world!"); // Print the message
  display.print("Contributions: ");
  display.println(todayContributions);
  display.display(); // Update the display
  // Serial.println("Message printed.");
  delay(1000); // Wait for a second
}
int getTodayContributions(const char* username, const char* token) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Error: Not connected to Wi-Fi.");
    return -1;
  }

  HTTPClient http;
  http.begin("https://api.github.com/graphql");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "bearer " + String(token));

  String query = "{\"query\":\"query { user(login: \\\"" + String(username) + "\\\") "
                 "{ contributionsCollection { contributionCalendar { totalContributions } } } }\"}";

  int httpResponseCode = http.POST(query);

  if (httpResponseCode != 200) {
    Serial.print("Error fetching contributions data: ");
    Serial.println(httpResponseCode);
    http.end();
    return -1;
  }

  String payload = http.getString();
  Serial.print("Payload: ");
  Serial.println(payload);
  http.end();

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("Error deserializing JSON: ");
    Serial.println(error.c_str());
    return -1;
  }

  JsonObject userData = doc["data"]["user"];
  if (userData.isNull()) {
    Serial.println("Error: User data not found.");
    return -1;
  }

  JsonObject contributionsCollection = userData["contributionsCollection"];
  if (contributionsCollection.isNull()) {
    Serial.println("Error: Contributions collection not found.");
    return -1;
  }

  JsonObject contributionCalendar = contributionsCollection["contributionCalendar"];
  if (contributionCalendar.isNull()) {
    Serial.println("Error: Contribution calendar not found.");
    return -1;
  }

  int todayContributions = contributionCalendar["totalContributions"];
  return todayContributions;
}



