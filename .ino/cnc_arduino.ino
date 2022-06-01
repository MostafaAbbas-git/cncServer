#include <Servo.h>
#include <Sleep_n0m1.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define EN 8    // stepper motor enable , active low
#define X_DIR 5 // X -axis stepper motor direction control
#define Y_DIR 6 // y -axis stepper motor direction control
#define X_STP 2 // x -axis stepper control
#define Y_STP 3 // y -axis stepper control
int pos = 0;
int countx = 0;
int county = 0;
int xt = 0;
int yt = 0;
bool xd;
bool yd;
int x_values[100];
int y_values[100];
int state[100];
int oldLength = 0;
String starting_flag;
Servo myservo;
SoftwareSerial esp(9, 10);

void setup()
{ // The stepper motor used in the IO pin is set to output
    pinMode(X_DIR, OUTPUT);
    pinMode(X_STP, OUTPUT);
    pinMode(Y_DIR, OUTPUT);
    pinMode(Y_STP, OUTPUT);
    pinMode(EN, OUTPUT);
    digitalWrite(EN, LOW);
    myservo.attach(11);
    esp.begin(9600);
    Serial.begin(9600);
    while (!Serial)
    {
        printf("..."); // wait for serial port to connect. Needed for native USB port only
    }
}
// holding the pen up from the paper
void penup()
{
    myservo.write(0);
    delay(100);
}
// putting the pen down to write
void pendown()
{
    myservo.write(28);
    delay(100);
}
/////////////////////////////////////////////////////////////////////////////////////
// 'xone' and 'yone' used only in step's' functions
void xone()
{
    digitalWrite(X_STP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(X_STP, LOW);
    delayMicroseconds(1000);
}
void yone()
{
    digitalWrite(Y_STP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(Y_STP, LOW);
    delayMicroseconds(1000);
}
// moving through xdirection
// set dirx=true>>>move in positive-x ||| set dirx=false>>>move in negative-x
// stepsx in number of steps"distance"
void stepx(boolean dirx, int stepsx)
{
    digitalWrite(X_DIR, dirx);
    delay(50);
    for (int i = 0; i < stepsx; i++)
    {
        xone();
        if (dirx == true)
        {
            countx++;
        }
        if (dirx == false)
        {
            countx--;
        }
    }
    delay(500);
}
// moving through ydirection
// set diry=true>>>move in positive-y ||| set diry=false>>>move in negative-y
// stepsy in number of steps"distance"
void stepy(boolean diry, int stepsy)
{
    digitalWrite(Y_DIR, diry);
    delay(50);
    for (int i = 0; i < stepsy; i++)
    {
        yone();
        if (diry == true)
        {
            county++;
        }
        if (diry == false)
        {
            county--;
        }
    }
    delay(500);
}
// rereturn to zero "not usable yet"!!
void zero()
{
    penup();
    slopexy(false, false, countx, county);
    delay(500);
}

// moving in x-y with (differnt degrees) degree
void slopexy(boolean dirx, boolean diry, int stepsx, int stepsy)
{
    digitalWrite(X_DIR, dirx);
    digitalWrite(Y_DIR, diry);
    delay(50);

    if (stepsx >= stepsy)
    {
        int slope = (stepsx / stepsy + 0.5);
        for (int i = 0; i < stepsy; i++)
        {
            for (int j = 0; j < slope; j++)
            {
                xone();
                if (dirx == true)
                {
                    countx++;
                }
                if (dirx == false)
                {
                    countx--;
                }
            }
            yone();
            if (diry == true)
            {
                county++;
            }
            if (diry == false)
            {
                county--;
            }
        }
    }
    if (stepsy > stepsx)
    {
        int slope = (stepsy / stepsx);
        for (int i = 0; i < stepsx; i++)
        {
            for (int j = 0; j < slope; j++)
            {
                yone();
                if (diry == true)
                {
                    county++;
                }
                if (diry == false)
                {
                    county--;
                }
            }
            xone();
            if (dirx == true)
            {
                countx++;
            }
            if (dirx == false)
            {
                countx--;
            }
        }
    }
    delay(100);
}

// void square(int centerx, int centery, int lengthx, int lengthy) {
//   zero();
//   slopexy (true , true, centerx, centery);
//   stepx(false, lengthx / 2);
//   stepy(false, lengthy / 2);
//   pendown();
//   stepx(true, lengthx);
//   stepy(true, lengthy);
//   stepx(false, lengthx);
//   stepy(false, lengthy);
// }
// void rhumbus(int centerx, int centery, int xdiagonal, int ydiagonal) {
//   zero();
//   slopexy (true , true, centerx, centery);
//   stepx(false, xdiagonal / 2);
//   stepy(false, ydiagonal / 2);
//   slopexy(true, true, xdiagonal / 2, ydiagonal / 2);
//   slopexy(false, true, xdiagonal / 2, ydiagonal / 2);
//   slopexy(false, false, xdiagonal / 2, ydiagonal / 2);
//   slopexy(true, false, xdiagonal / 2, ydiagonal / 2);
// }
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
void drawshape(int xpose[], int ypose[], int state[])
{
    penup();
    for (byte i = 0; i < (sizeof(xpose) / sizeof(xpose[0])); i++)
    {
        // Now we have (x,y) -> (x_values[i],y_values[i])
        // We need to mvoe the pen to that point
        if (state[i] == 0)
        {
            penup();
        }
        else
        {
            pendown();
        }
        if (100 * xpose[i] >= countx)
        {
            xd = true;
            xt = (100 * xpose[i]) - countx;
        }
        else
        {
            xd = false;
            xt = countx - (100 * xpose[i]);
        }
        if (100 * ypose[i] >= county)
        {
            yd = true;
            yt = (100 * ypose[i]) - county;
        }
        else
        {
            yd = false;
            yt = county - (100 * ypose[i]);
        }
        slopexy(xd, yd, xt, yt);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
    penup();
    if (esp.available() >= 0)
    {
        Serial.print("esp is available...");
        Serial.print(esp.read());

        // flag for recieving array sizes
        starting_flag = esp.read();

        if (starting_flag == "GO")
        {
            Serial.print("start recieving data...");
            get_data_from_stream();
            Serial.print("done recieving data...");

            for (byte i = 0; i < (sizeof(x_values) / sizeof(x_values[0])); i++)
            {
                // Now we have (x,y) -> (x_values[i],y_values[i])
                // We need to mvoe the pen to that point
                if (state[i] == 0)
                {
                    penup();
                }
                else
                {
                    pendown();
                }
                if (x_values[i] >= countx)
                {
                    xd = true;
                    xt = x_values[i] - countx;
                }
                else
                {
                    xd = false;
                    xt = countx - x_values[i];
                }
                if (y_values[i] >= county)
                {
                    yd = true;
                    yt = y_values[i] - county;
                }
                else
                {
                    yd = false;
                    yt = county - y_values[i];
                }
                slopexy(xd, yd, xt, yt);
            }
        }
    }
    zero();
    exit(0);
}