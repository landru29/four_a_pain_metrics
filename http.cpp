#include "http.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <malloc.h>
#include <base64.h>
#include <string.h>
#include "config.h"

#define HOST "opentsdb.gra1.metrics.ovh.net"
#define URL "/api/put"

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
// echo | openssl s_client -showcerts -servername opentsdb.gra1.metrics.ovh.net -connect opentsdb.gra1.metrics.ovh.net:443 2>/dev/null | openssl x509 -noout -fingerprint -sha1 -inform pem | sed -e "s/.*=//g" | sed -e "s/\:/ /g"
const char* fingerprint = "E5 7E C6 FA FC 55 F7 9A 98 0B 41 A6 63 DF C2 F8 84 F3 4E B1";

Https::Https() {
  this->values = (int*) malloc(1);
  this->bufferSize=0;
}

void Https::addValue(int val) {
  this->bufferSize++;
  this->values = (int*)realloc(this->values, this->bufferSize * sizeof(int));
  this->values[this->bufferSize-1] = val;
}


void Https::flushData() {
  String body("[");
  for(int i = 0; i<this->bufferSize; i++) {
   body += String("{\"metric\":\"temperature\",\"value\":") + String(this->values[i]) + String(",\"tags\":{}}");
   if (i < this->bufferSize-1) {
    body += String(",");
   }
  }
  body += String("]");
  this->values = (int*)realloc(this->values, 1);
  this->bufferSize=0;

  String token = String("DESC:") + TOKEN;
  String base64Token = base64::encode(token);

  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(HOST);
  if (!client.connect(HOST, 443)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, HOST)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }

  Serial.print("requesting URL: ");
  Serial.println(URL);

  client.print(String("POST ") + URL + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Content-Length: " + String(body.length())  + "\r\n" +
               "Authorization: basic " + base64Token + "\r\n" +
               "Content-Type: application/json\r\n\r\n" + body);

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
     Serial.println(line);
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

}
