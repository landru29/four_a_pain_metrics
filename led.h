#ifndef __LED__H__
#define __LED__H__
#include <ESP8266WiFi.h>


class Led {
  public:
    Led(int pin);
    static int pin;
    static void displayStatus(unsigned int type);
    static void switchOn();
    static void switchOff();
    
};

#endif
