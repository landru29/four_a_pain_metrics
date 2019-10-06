#include "warp10.h"
#include "config.h"
#include "malloc.h"
#include <ESP8266HTTPClient.h>

#define URL "/api/v0/exec"


WarpTen::WarpTen() {

}

const char* WarpTen::url = "/api/v0/exec";

unsigned long WarpTen::getTimestamp() {
  HTTPClient http;

  Serial.println("Requesting Timestamp");
  int beginResult = http.begin(String("https://") + WARP10_HOST + this->url, FINGERPRINT);
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.POST("NOW");
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

  int responseLength =  response.length();
  char* timestampStr = (char*)malloc(responseLength-6);
  timestampStr[responseLength-6] = 0;
  response.substring(1, responseLength-7).toCharArray(timestampStr, responseLength - 7);

  long result = atol(timestampStr);
  free(timestampStr);
  return result;
}
