#include <Arduino.h>
#include "USBKeyboard.h"

USBKeyboard Keyboard;

// ---- Pin assignments for XIAO RP2040 ----
const int buttonPins[4] = {26, 27, 28, 29};
const int ledPins[4]    = {5, 7, 0, 1};

// ---- Settings ----
const unsigned long ledFlashTime = 500;  // 0.5 second

// ---- Debounce management ----
bool lastState[4] = {false, false, false, false};
unsigned long lastChange[4] = {0, 0, 0, 0};
const unsigned long debounceDelay = 30;

void flashLED(int ledPin) {
  digitalWrite(ledPin, HIGH);
  delay(ledFlashTime);
  digitalWrite(ledPin, LOW);
}

void sendPrintScreen() {
  Keyboard.write(KEY_PRINT_SCREEN);
}

void sendRandomShortcut() {
  // Example: F13 (useful for OBS, custom bindings)
  Keyboard.write(KEY_F13);
}

void sendCtrlV() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.write('v');
  Keyboard.releaseAll();
}

void sendCtrlC() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.write('c');
  Keyboard.releaseAll();
}

void setup() {
  // Buttons with internal pullups
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // LEDs as outputs
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  USB.begin();
}

void loop() {
  for (int i = 0; i < 4; i++) {

    bool pressed = (digitalRead(buttonPins[i]) == LOW); // active low
    unsigned long now = millis();

    if (pressed != lastState[i]) {
      if (now - lastChange[i] > debounceDelay) {
        lastState[i] = pressed;
        lastChange[i] = now;

        if (pressed) {
          // Flash LED
          flashLED(ledPins[i]);

          // Do action depending on the button
          switch (i) {
            case 0:
              sendPrintScreen();
              break;
            case 1:
              sendRandomShortcut();
              break;
            case 2:
              sendCtrlV();
              break;
            case 3:
              sendCtrlC();
              break;
          }
        }
      }
    }
  }
}
