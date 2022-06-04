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
char x_values[100];
char y_values[100];
char state[100];
char oldLength = 0;
char starting_flag;
int my_length = 0;
char array_length = 0;

Servo myservo;

void setup()
{ // The stepper motor used in the IO pin is set to output
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STP, OUTPUT);
  pinMode(Y_DIR, OUTPUT);
  pinMode(Y_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  myservo.attach(11);
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
void zero()
{
  penup();
  slopexy(false, false, countx, county);
  delay(500);
}
// moving in x-y with (differnt degrees)
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

void get_data_from_stream()
{
  array_length = 0;
  array_length = Serial.read();

  if (array_length == oldLength)
  {
    Serial.print("array_length == oldLength");
    return;
  }

  oldLength = array_length;
  for (char i = 0; i < array_length; i++)
  {
    if (Serial.read() != -1)
    {
      x_values[i] = Serial.read();
      my_length += array_length;
    }
  }

  for (char i = 0; i < array_length; i++)
  {
    if (Serial.read() != -1)
    {
      y_values[i] = Serial.read();
      my_length += array_length;
    }
  }

  for (char i = 0; i < array_length; i++)
  {
    if (Serial.read() != -1)
    {
      state[i] = Serial.read();
      my_length += array_length;
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  penup();
  if (esp.available() >= 0)
  {

    Serial.println("avail");

    // flag for recieving array sizes
    starting_flag = Serial.read();
    if (starting_flag == 255)
    {
      Serial.println("start recieving data...");
      get_data_from_stream();
      Serial.println("done recieving data...");
    }
    if (my_length == (array_length * 3) && (my_length != 0))
    {
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
        if ((2 * x_values[i]) >= countx)
        {
          xd = true;
          xt = (2 * x_values[i]) - countx;
        }
        else
        {
          xd = false;
          xt = countx - (2 * x_values[i]);
        }
        if ((2 * y_values[i]) >= county)
        {
          yd = true;
          yt = (2 * y_values[i]) - county;
        }
        else
        {
          yd = false;
          yt = county - (2 * y_values[i]);
        }
        slopexy(xd, yd, xt, yt);
        //        zero();
        //        exit(0);
        //        delay(10000);
      }
    }
  }
}