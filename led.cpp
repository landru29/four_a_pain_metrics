
#include "led.h"


int Led::pin = 0;


Led::Led(int pin) {
  Led::pin = pin;
  pinMode(pin, OUTPUT);
}


void Led::displayStatus(unsigned int type) {

  for(int i=0; i<16; i++) {
    unsigned char bit = type & 1;
    if (bit == 1) {
      digitalWrite(Led::pin, HIGH);
    } else {
      digitalWrite(Led::pin, LOW);
    }
    type >>=1;
    delay(300);
  }
}

void Led::switchOn() {
  digitalWrite(Led::pin, LOW);
}

void Led::switchOff() {
  digitalWrite(Led::pin, HIGH);
}
