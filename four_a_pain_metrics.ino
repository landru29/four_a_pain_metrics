/*************************************************** 
  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K

  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269

  These displays use SPI to communicate, 3 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>
#include <Arduino.h>
#include "wifi.h"
#include "http.h"
#include <Adafruit_MAX31855.h>
#include "led.h"

// Default connection is using software SPI, but comment and uncomment one of
// the two examples below to switch between software SPI and hardware SPI:

// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
#define MAXDO   D7  // Orange
#define MAXCS   D6  // Jaune
#define MAXCLK  D5  // vert

#define A (70.0/46.0)
#define B (20 - 7.0 * A)

#define NaN 10000

#define SET_SIZE 10
int ovenSet[SET_SIZE];
int ambientSet[SET_SIZE];


WifiConnect* myWifi;

Https* clientHttp;

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

void setup() {
  
  Serial.begin(9600);

  for(int i=0; i<SET_SIZE; i++) {
    ovenSet[i] = 0;
    ambientSet[i] = 0;
  }

  myWifi = new WifiConnect("thermocouple");
  clientHttp = new Https();

  
  Serial.println("IOT Metrics");
  // wait for MAX chip to stabilize
  delay(500);

  myWifi->startWPSPBC();
}

int lisser(int value, int* lisseur) {
    for (int i = 1; i< SET_SIZE; i++) {
      lisseur[i-1] = lisseur[i];
      lisseur[SET_SIZE-1] = value;
     }

     int lisse = 0;
     for (int i = 0; i< SET_SIZE; i++) {
      lisse += lisseur[i];
     }
     
     return lisse / SET_SIZE;
}


int getOven() {
   double c = thermocouple.readCelsius();
   if (!isnan(c)) {
      if (c<0) {
        c = -c;
      }

     int oven = lisser(c * A + B, ovenSet);
    
     Serial.print("Four: ");
     Serial.println(String(oven));

     return oven;
     
     
   }
   return NaN;
}

int getAmbient() {
  double c = thermocouple.readInternal();
  if (!isnan(c)) {
      if (c<0) {
        c = -c;
      }

     int ambient = lisser((int)c, ambientSet);
    
     Serial.print("Ambient: ");
     Serial.println(String(ambient));

     return ambient;
     
     
   }
   return NaN;
}

void loop() {
   Led::switchOff();
  
  int oven = 0;
  int ambient = 0;


   for(int i=0; i<SET_SIZE; i++) {
      Led::switchOn();
      do {
        oven = getOven();
        if (oven == NaN) {
          delay(1000);
        }
      } while (oven == NaN);

      do {
        ambient = getAmbient();
        if (ambient == NaN) {
          delay(1000);
        }
      } while (ambient == NaN);
      
      Led::switchOff();
      delay(1000 * 1);
   }
   
   if ((oven != NaN) && (ambient != NaN)) {
     Led::switchOn();
     clientHttp->addValue(oven, ambient);
     clientHttp->flushData();
     Led::switchOff();
   }

   Serial.println("*****************************************\nsleeping ...\n");

 
   delay(10000);
}
