#include "LedControl.h"
#include <Wire.h>

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to LOAD(CS)
 pin 10 is connected to the CLK 
 We have only a single MAX72XX.
 */
LedControl lc = LedControl(12, 10, 11, 1);

int FrontLight = 0;
int SteeringAngle = 0;
int Speed = 0;

int FrontLightPin = 2;
int BackLightPin = 7;
int LeftTurnLightPin = 9;
int RightTurnLightPin = 8;

unsigned long delaytime1 = 50;
unsigned long delaytime2 = 200;

void receiveEvent(int FrontLight)
{
  while (1 < Wire.available())
  {
    FrontLight = Wire.read();
    SteeringAngle = Wire.read();
  }

  Speed = Wire.read();

  digitalWrite(FrontLightPin, FrontLight);
  digitalWrite(BackLightPin, FrontLight);
}

void setup()
{
  /*
    Der MAX72XX ist im Power-Save-Modus beim Start,
    Wir müssen einen "Weckruf" machen
   */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 8);
  /* and clear the display */
  lc.clearDisplay(0);
  pinMode(FrontLightPin, OUTPUT);
  pinMode(BackLightPin, OUTPUT);
  pinMode(LeftTurnLightPin, OUTPUT);
  pinMode(RightTurnLightPin, OUTPUT);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
}

/*
 Diese Methode zeigt die Zeichen für das
  Wort "Arduino" nacheinander auf der Matrix.
  (Du brauchst mindestens 5x7 LEDs, um die ganzen Zeichen zu sehen)
 */
void writeArduinoOnMatrix()
{
  /*Hier sind die Daten für die Zeichen */

    byte a0[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
    byte b0[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
    byte c0[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
    byte d0[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
    byte e0[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};

    byte a1[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
    byte b1[8] = {B00000000, B00000000, B00000000, B00011000, B00011000, B00000000, B00000000, B00000000};
    byte c1[8] = {B00000000, B00000000, B00111100, B00111100, B00111100, B00111100, B00000000, B00000000};
    byte d1[8] = {B00000000, B01111110, B01111110, B01111110, B01111110, B01111110, B01111110, B00000000};
    byte e1[8] = {B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111};

    byte* a;
    byte* b;
    byte* c;
    byte* d;
    byte* e;


  if (Speed < 80)
  {
    a=a1;
    b=b1;
    c=c1;
    d=d1;
    e=e1;
  }
  else
  {
    a=a0;
    b=b0;
    c=c0;
    d=d0;
    e=e0;
  }

  digitalWrite(LeftTurnLightPin, LOW);
  digitalWrite(RightTurnLightPin, LOW);

  /* Zeigen sie nun eins nach dem anderen mit einer kleinen Verzögerung an */
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, a[i]);
  };
  delay(delaytime1);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, b[i]);
  };
  delay(delaytime1);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, c[i]);
  };
  delay(delaytime1);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, d[i]);
  };
  delay(delaytime1);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, e[i]);
  };

  if (SteeringAngle < 80)
    digitalWrite(LeftTurnLightPin, HIGH);
  if (SteeringAngle > 100)
    digitalWrite(RightTurnLightPin, HIGH);

  delay(delaytime1);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, d[i]);
  };
  delay(delaytime1);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, c[i]);
  };
  delay(delaytime1);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, b[i]);
  };
  delay(delaytime1);
}

void loop()
{

  writeArduinoOnMatrix();
}
