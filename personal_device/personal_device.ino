#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define KEY_PIN_1 2
#define KEY_PIN_2 0
#define KEY_PIN_3 4
#define KEY_PIN_4 16
#define KEY_PIN_5 17
#define KEY_PIN_6 5
#define KEY_PIN_7 18
#define KEY_PIN_8 19

#define KNOB_PIN_1 A0
#define KNOB_PIN_2 A1
#define KNOB_PIN_3 A2
#define KNOB_PIN_4 A3

#define SPEAKER_PIN 9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte colPins[COLS] = {11, 10, 8, 7};
byte rowPins[ROWS] = {6, 5, 4, 3 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int frequencies[ROWS][COLS] = {
  {262, 294, 330, 349},
  {392, 440, 494, 523},
  {587, 659, 698, 784},
  {880, 988, 1047, 1175}
};

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  displayMessage("Hello!");
}

void loop() {

  // Get knob vals
  int knob1Val = analogRead(KNOB_PIN_1);
  int knob2Val = analogRead(KNOB_PIN_2);
  int knob3Val = analogRead(KNOB_PIN_3);
  int knob4Val = analogRead(KNOB_PIN_4);

  Serial.print(" K1:");
  Serial.print(knob1Val);
  Serial.print(" K2:");
  Serial.print(knob2Val);
  Serial.print(" K3:");
  Serial.print(knob3Val);
  Serial.print(" K4:");
  Serial.println(knob4Val);

  char key = keypad.getKey();
  char prevKey = '\0';
  
  if (key) {
    Serial.println(key);
    displayMessage("Got key!");
    for (byte i = 0; i < ROWS; i++) {
      for (byte j = 0; j < COLS; j++) {
        if (keys[i][j] == key) {
          tone(SPEAKER_PIN, frequencies[i][j]);
          prevKey = key;
        }
      }
    }
  } else if (prevKey != '\0') {
    noTone(SPEAKER_PIN);
    prevKey = '\0';
  } else {
    displayMessage("No keys");
  }

  delay(10); // Update every second
}

// void loop() {
//   // Get knob vals
//   int knob1Val = analogRead(KNOB_PIN_1);
//   int knob2Val = analogRead(KNOB_PIN_2);
//   int knob3Val = analogRead(KNOB_PIN_3);
//   int knob4Val = analogRead(KNOB_PIN_4);
//   // Serial.print(" K1:");
//   // Serial.print(knob1Val);
//   // Serial.print(" K2:");
//   // Serial.print(knob2Val);
//   // Serial.print(" K3:");
//   // Serial.print(knob3Val);
//   // Serial.print(" K4:");
//   // Serial.println(knob4Val);

//   char key = keypad.getKey();

//   if (key) {
//     Serial.println(key);
//     displayMessage("Got key!");
//     for (byte i = 0; i < ROWS; i++) {
//       for (byte j = 0; j < COLS; j++) {
//         if (keys[i][j] == key) {
//           tone(SPEAKER_PIN, frequencies[i][j]);
//           delay(500);
//           noTone(SPEAKER_PIN);
//         }
//       }
//     }
//   } else {
//     // Serial.println("None");
//     displayMessage("No keys");
//   }

//   delay(5); // Update every second
// }

void displayMessage(const char* message) {
  display.clearDisplay(); // Clear the buffer

  int textWidth = String(message).length() * 6; // Width of the text in pixels (6 pixels per character)
  int cursorX = (SCREEN_WIDTH - textWidth) / 2; // Calculate the X position for centering the text
  int cursorY = (SCREEN_HEIGHT / 2) - 10; // Calculate the Y position for centering the text

  display.setCursor(cursorX, cursorY); // Set the cursor position
  display.setTextSize(1); // Set the text size
  display.setTextColor(WHITE); // Set the text color
  display.println(message); // Print the valance string

  display.display(); // Update the display
}



