#include <Arduino.h>
#include "wifi.h"
#include "http.h"
#include "warp10.h"
#include "led.h"

#define ARDUINO 100
#include <Adafruit_MAX31855.h>


#define MAXDO   D7  // Orange
#define MAXCS   D6  // Jaune
#define MAXCLK  D5  // vert

#define A (70.0/46.0)
#define B (20 - 7.0 * A)

#define NaN 10000

#define SET_SIZE 60
int ovenSet[SET_SIZE];
int ambientSet[SET_SIZE];


WifiConnect* myWifi;
Https* clientHttp;
WarpTen* warp10;
long ts=0;

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
  warp10 = new WarpTen();


  Serial.println("IOT Metrics");
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

void wait(int seconds) {
  delay(seconds * 1000);
  ts += seconds;
}

void loop() {
   Led::switchOff();

   ts = warp10->getTimestamp();

  
  int oven = 0;
  int ambient = 0;


   for(int i=0; i<SET_SIZE; i++) {
      Led::switchOn();
      do {
        oven = getOven();
        if (oven == NaN) {
          wait(1000);
        }
      } while (oven == NaN);

      do {
        ambient = getAmbient();
        if (ambient == NaN) {
          wait(1000);
        }
      } while (ambient == NaN);
      
      Led::switchOff();
      wait(1000);
   }
   
   if ((oven != NaN) && (ambient != NaN)) {
     Led::switchOn();
     clientHttp->addValue(oven, ambient, ts);
     Led::switchOff();
   }

   if (clientHttp->getBufferSize() > 10) {
    clientHttp->flushData();
   }

}
