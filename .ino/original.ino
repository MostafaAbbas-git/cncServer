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
const char *instructions_url = "http://192.168.1.15:4000/instructions"; // instructions endpoint

const String sending_flag = 255;
//const String sending_flag = "GO";

void setup()
{
  Serial.begin(9600);
  //  toUno.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    // Serial.print(".");
  }
  //   Serial.println("");
  //   Serial.print("WiFi Succesfully connected: ");
  //   Serial.println(ssid);
  //  toUno.print("FROM ESP");
  //     Serial.print("\nConnected, IP address: ");
  //     Serial.println(WiFi.localIP());
}

void loop()
{

  // Local client
  WiFiClient client;

  HTTPClient http;

  if (WiFi.status() == WL_CONNECTED)
  {

    // GET REQUEST to get the instructions from the server

    if (http.begin(client, instructions_url))
    {

      int httpCode = http.GET();
      // Check the returning code
      if (httpCode > 0)
      {
        // Get the request response payload
        String payload = "{}";
        payload = http.getString();

        //        Serial.print("payload = ");
        //        Serial.println(payload);

        DynamicJsonBuffer jsonBuffer;

        JsonObject &root = jsonBuffer.parseObject(payload);

        JsonArray &state = root["state"];
        JsonArray &x = root["x"];
        JsonArray &y = root["y"];

        const char arrayLength = y.size();

        // Serial.print("Length = ");
        // Serial.println(arrayLength);


        // send starting flag to arduino via serial print to start the communication
               Serial.println(sending_flag);
        // Serial.println("GO");
        //        toUno.write(toUno.read());
        delay(500);
        Serial.println(arrayLength);
        delay(500);

        for (char i = 0; i < arrayLength; i++)
        {
          char currentValue = x[i];
          Serial.println(currentValue);
          delay(200);
        }

        for (char i = 0; i < arrayLength; i++)
        {
          char currentValue = y[i];
          Serial.println(currentValue);
          delay(200);
        }

        for (char i = 0; i < arrayLength; i++)
        {
          char currentValue = state[i];
          Serial.println(currentValue);
          delay(200);
        }
      }

      else
      {
        Serial.print("Error code: ");
        Serial.println(httpCode);
      }

      http.end();
    }
    else
    {
      Serial.printf("[HTTP GET] Couldn't begin.\n");
    }
  }
  else
  {
    Serial.printf("[HTTP] Unable to connect to server. Wifi not connected.\n");
  }
  // delay 10 secs before next iteration
  delay(10000);
}