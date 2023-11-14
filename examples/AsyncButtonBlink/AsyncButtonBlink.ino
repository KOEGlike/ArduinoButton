#include <Arduino.h>
#include <async_button.h>

const int btn1pin=0;//put your button pin here
const int btn2pin=1;//put your button pin here


AsyncButton btn1 (btn1pin,[&](){Serial.println("btn 1 short");}, [&](){Serial.println("btn1 long");});
AsyncButton btn2 (btn2pin,[&](){Serial.println("btn 2 short");}, [&](){Serial.println("btn2 long");});

void setup() {
  ButtonManager.link(std::vector<AsyncButton*>{&btn1, &btn2}, [&](){Serial.println("duble");});
}

void loop() {
  ButtonManager.check();
}
