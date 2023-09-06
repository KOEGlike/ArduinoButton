#include <Arduino.h>
#include <async_button.hpp>

#define BUTTON_PIN 2
#define LED_PIN 13

AsyncButton btn (BUTTON_PIN,[](){digitalWrite(LED_PIN, HIGH);}, [](){digitalWrite(LED_PIN, LOW);}) ;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  btn.check();
}