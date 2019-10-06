#include "http.h"
#include <ESP8266HTTPClient.h>
#include <malloc.h>
#include <base64.h>
#include <string.h>
#include "config.h"


Https::Https() {
  this->sensor = (Sensor*) malloc(1);
  this->bufferSize=0;
}

const char* Https::url = "/api/put";

void Https::addValueAndFlush(int val, int ambient, long timestamp) {
  this->addValue(val, ambient, timestamp);
  this->flushData();
}

void Https::addValue(int val, int ambient, long timestamp) {
  this->bufferSize++;
  this->sensor = (Sensor*)realloc(this->sensor, this->bufferSize * sizeof(Sensor));

  Sensor sensor;
  sensor.oven = val;
  sensor.ambient = ambient;
  sensor.timestamp = timestamp;
  
  this->sensor[this->bufferSize-1] = sensor;
}

int Https::getBufferSize() {
  return this->bufferSize;
}

void Https::flushData() {
  String body("[");
  for(int i = 0; i<this->bufferSize; i++) {
   String timestampLine = String();
   if (this->sensor[i].timestamp>0) {
     timestampLine = String("\"timestamp\":") + String(this->sensor[i].timestamp) + ",";
   }
   body += String("{") + 
        "\"metric\":\"temperature\"," +
        "\"value\":" + String(this->sensor[i].oven) + "," +
        timestampLine +
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
  Serial.println(String("POST ") + OPENTSDB_HOST + this->url);
  //Serial.println(base64Token);
  Serial.println(body);
  Serial.println("------------------------------------------------\n");

  HTTPClient http;
  
  int beginResult = http.begin(String("https://") + OPENTSDB_HOST + this->url, FINGERPRINT);
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
