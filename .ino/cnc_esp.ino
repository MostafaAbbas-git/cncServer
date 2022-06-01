#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <ESP8266WiFi.h>

// HTTP libraries
#include <ESP8266HTTPClient.h>

// http: local
#include <WiFiClient.h>

#include <ArduinoJson.h> //v5.13.5

#define ssid "NETWORK_NAME"
#define password "NETWORK_PASSWORD"

const char *instructions_url = "http://192.168.1.13:4000/instructions"; // instructions endpoint

const String sending_flag = "GO";

void setup()
{
    Serial.begin(9600);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi Succesfully connected");
    Serial.println(ssid);
    Serial.print("\nConnected, IP address: ");
    Serial.println(WiFi.localIP());
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

                StaticJsonBuffer<1024> jsonBuffer;

                String json = payload;
                JsonObject &myObject = jsonBuffer.parseObject(json);

                // Test if parsing succeeds.
                if (!myObject.success())
                {
                    Serial.println("parseObject() failed");
                    return;
                }

                Serial.print("json = ");
                Serial.println(json);

                const int arrayLength = sizeof(myObject["x"]) / sizeof(myObject["x"][0]);

                Serial.print("Length = ");
                Serial.println(arrayLength);

                // send starting flag to arduino via serial print to start the communication
                Serial.println(sending_flag);

                delay(500);
                Serial.println(arrayLength);
                delay(500);

                for (int i = 0; i < arrayLength; i++)
                {
                    int currentValue = myObject["x"][i];
                    Serial.println(currentValue);
                    delay(200);
                }

                for (int i = 0; i < arrayLength; i++)
                {
                    int currentValue = myObject["y"][i];
                    Serial.println(currentValue);
                    delay(200);
                }

                for (int i = 0; i < arrayLength; i++)
                {
                    int currentValue = myObject["state"][i];
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
    // delay 1 sec before next iteration
    delay(1000);
}
