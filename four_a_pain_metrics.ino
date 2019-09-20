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

// Default connection is using software SPI, but comment and uncomment one of
// the two examples below to switch between software SPI and hardware SPI:

// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
#define MAXDO   D7  // Orange
#define MAXCS   D6  // Jaune
#define MAXCLK  D5  // vert

#define A (70.0/46.0)
#define B (20 - 7.0 * A)
#define SET_SIZE 5

int setOf[SET_SIZE] = {0, 0, 0, 0, 0};
WifiConnect* myWifi;

Https* clientHttp;

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

void setup() {
  
  Serial.begin(9600);

  myWifi = new WifiConnect("thermocouple");
  clientHttp = new Https();

  
  Serial.println("IOT Metrics");
  // wait for MAX chip to stabilize
  delay(500);

  myWifi->startWPSPBC();
}

void loop() {

  
  // basic readout test, just print the current temp
   double ambient = thermocouple.readInternal();
   
   Serial.print("Ambient: ");
   Serial.println(String(ambient));
   
   

   double c = thermocouple.readCelsius();
   if (!isnan(c)) {
      if (c<0) {
        c = -c;
      }
     for (int i = 1; i< SET_SIZE; i++) {
      setOf[i-1] = setOf[i];
      setOf[SET_SIZE-1] = (int)(c * A + B);
     }

     int lisse = 0;
     for (int i = 0; i< SET_SIZE; i++) {
      lisse += setOf[i];
     }
     lisse /= SET_SIZE;
    
     Serial.print("Four: ");
     Serial.println(String(lisse));

     clientHttp->addValue(lisse);
     clientHttp->flushData();
     
     
   } else {
     Serial.println("Something wrong with thermocouple!");
   }
 
   delay(10000);
}
