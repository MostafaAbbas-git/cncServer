#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
// HTTP libraries
#include <ESP8266HTTPClient.h>

// http: local
#include <WiFiClient.h>

#include <ArduinoJson.h> //v5.13.5

#define ssid "Mostafa"
#define password "123456789"

//SoftwareSerial toUno(9, 10);
const char *instructions_url = "http://192.168.43.180:4000/instructions"; // instructions endpoint

const String sending_flag = "GO";
//const String sending_flag = "GO";

void setup()
{
  Serial.begin(9600);
}

void loop()
{
Serial.write(400);
Serial.write("GO");
Serial.write("a");
Serial.write(20);
  // delay 1 sec before next iteration
  delay(1000);
}