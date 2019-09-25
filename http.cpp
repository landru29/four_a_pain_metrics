#include "http.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <malloc.h>
#include <base64.h>
#include <string.h>
#include "config.h"

#define HOST "opentsdb.gra1.metrics.ovh.net"
#define URL "/api/put"

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
// echo | openssl s_client -showcerts -servername opentsdb.gra1.metrics.ovh.net -connect opentsdb.gra1.metrics.ovh.net:443 2>/dev/null | openssl x509 -noout -fingerprint -sha1 -inform pem | sed -e "s/.*=//g" | sed -e "s/\:/ /g"
#define fingerprint "E5 7E C6 FA FC 55 F7 9A 98 0B 41 A6 63 DF C2 F8 84 F3 4E B1"

Https::Https() {
  this->sensor = (Sensor*) malloc(1);
  this->bufferSize=0;
}

void Https::addValueAndFlush(int val, int ambient) {
  this->addValue(val, ambient);
  this->flushData();
}

void Https::addValue(int val, int ambient) {
  this->bufferSize++;
  this->sensor = (Sensor*)realloc(this->sensor, this->bufferSize * sizeof(Sensor));

  Sensor sensor;
  sensor.oven = val;
  sensor.ambient = ambient;
  
  this->sensor[this->bufferSize-1] = sensor;
}


void Https::flushData() {
  String body("[");
  for(int i = 0; i<this->bufferSize; i++) {
   body += String("{") + 
        "\"metric\":\"temperature\"," +
        "\"value\":" + String(this->sensor[i].oven) + "," +
        "\"tags\":{}" +
    "},";
    body += String("{") + 
        "\"metric\":\"ambient\"," +
        "\"value\":" + String(this->sensor[i].ambient) + "," +
        "\"tags\":{}" +
    "}";
   
   if (i < this->bufferSize-1) {
    body += String(",");
   }
  }
  body += String("]");
  this->sensor = (Sensor*)realloc(this->sensor, 1);
  this->bufferSize=0;

  String token = String("DESC:") + TOKEN;
  String base64Token = String("basic ") + base64::encode(token, false);

  Serial.println("\n------------------------------------------------");
  Serial.println(String("POST ") + HOST + URL);
  //Serial.println(base64Token);
  Serial.println(body);
  Serial.println("------------------------------------------------\n");

  HTTPClient http;
  
  int beginResult = http.begin(String("https://") + HOST + URL, fingerprint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", base64Token);
  
  int httpCode = http.POST(body);
  String response = http.getString();

  Serial.print("beginResult: ");
  Serial.print(beginResult);
  Serial.println();
  
  Serial.print("http: ");
  Serial.print(httpCode);
  Serial.println();
  
  Serial.print("response: ");
  Serial.println(response);
  Serial.println();
  
  http.end();

}
