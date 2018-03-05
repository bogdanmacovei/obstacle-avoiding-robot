#include <IRremote.h>
#include <IRremoteInt.h>
#include <IRremoteTools.h>

// senzor IR
const int recvPin = 2;
IRrecv irrecv(recvPin);
decode_results results;

// senzor distanta
const int trig = 13;
const int echo = 12;
int duration = 0;
int dist; 

// motoare
const int enablePin1 = 3;
const int in1Pin = 10;
const int in2Pin = 9;
const int enablePin2 = 4;
const int in3Pin = 6;
const int in4Pin = 5;

void setup() 
{
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enablePin2, OUTPUT);

  pinMode (trig, OUTPUT);
  pinMode (echo, INPUT);
  
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void setMotor1(int speed, boolean reverse)
{
  analogWrite(enablePin1, speed);
  digitalWrite(in1Pin, !reverse);
  digitalWrite(in2Pin, reverse);
}

void setMotor2(int speed, boolean reverse)
{
  analogWrite(enablePin2, speed);
  digitalWrite(in4Pin, !reverse);
  digitalWrite(in3Pin, reverse);
}

int speed1 = 255;
int speed2 = 255;
int IRdirection1 = 1;
int IRdirection2 = 1;

bool isStarted = false;

void loop() 
{
  if (isStarted)
  {
    setMotor1(speed1, IRdirection1);
    setMotor2(speed2, IRdirection2);
  }

  if (irrecv.decode(&results))
  {
     if (results.value == 0xffe21d)
     {
      isStarted = true;
      speed1 = speed2 = 255;
     }
     else if (results.value == 0xff18e7)
     {
        IRdirection1 = IRdirection2 = 1;
     }
     else if (results.value == 0xff38c7)
     {
        IRdirection1 = IRdirection2 = 0;
     }
     else if (results.value == 0xff5aa5)
     {
        IRdirection1 = 1;
        IRdirection2 = 0;
     }
     else if (results.value == 0xff10ef)
     {
        IRdirection1 = 0;
        IRdirection2 = 1;
     }
     
     irrecv.resume();
     irrecv.enableIRIn();
  }

  digitalWrite (trig, LOW);
  digitalWrite (trig, HIGH);
  delayMicroseconds (10);
  digitalWrite (trig, LOW);

  // determinarea distantei citita de senzor
  duration = pulseIn (echo, HIGH);
  dist = duration / (2 * 29);

  if (dist <= 12 && (IRdirection1 == 1 && IRdirection2 == 1))
  {
    speed1 = speed2 = 0;
    setMotor1 (speed1, 0);
    setMotor2 (speed2, 0);
  }
  else
  {
    speed1 = speed2 = 255;
  }
}

