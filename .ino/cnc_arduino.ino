#include <SoftwareSerial.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Global variables
String starting_flag;
int x_values[100];
int y_values[100];
int state[100];
int oldLength = 0;
SoftwareSerial esp(9, 10); // any 2 pins

void setup()
{
    // put your setup code here
    esp.begin(9600);
}

void loop()
{
    if (esp.available() >= 0)
    {
        // flag for recieving array sizes
        starting_flag = esp.read();

        if (starting_flag == "GO")
        {
            get_data_from_stream();
            // Start drawing using x_values[], y_values[], state[]
        }
    }
}

void get_data_from_stream()
{
    int array_length = 0;
    array_length = esp.read();

    if (array_length == oldLength)
    {
        Serial.print("array_length == oldLength");
        return;
    }

    oldLength = array_length;
    for (byte i = 0; i < array_length; i++)
    {
        x_values[i] = esp.read();
    }

    for (byte i = 0; i < array_length; i++)
    {
        y_values[i] = esp.read();
    }

    for (byte i = 0; i < array_length; i++)
    {
        state[i] = esp.read();
    }
}