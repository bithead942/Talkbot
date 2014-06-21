/*********************************************
Talkbot
by Bithead942

Arduino Pins Used:
2 - Sound Module Reset
3 - Sound Module Clock
4 - Sound Module Data
5 - Sound Module Busy
6 - Left Bumper sensor
7 - Right Bumper sensor
8 - Mood Switch
9 - Green Eye LEDs
10 - Red Eye LEDs

Required:
WTV020SD16p sound module for WAV file playback with 7 WAV files loaded
Adafruit Motor Shield with 2 electric motors
2 Bumper Sensors
2 RGB LEDs with common cathode
1 Toggle switch for mood


*************************************************/

#include <Wtv020sd16p.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

int resetPin = 2; 
int clockPin = 3; 
int dataPin = 4; 
int busyPin = 5; 
const int LBumperPin = 6; 
const int RBumperPin = 7; 
const int MoodPin = 8; 
const int GreenEyePin = 9;  
const int RedEyePin = 10;
int iLBumperStateNew = 0;
int iRBumperStateNew = 0;
int iBumperStateOld = 0;
int iMoodState = 0;
byte iPlayVal = 0;

Wtv020sd16p SoundModule(resetPin,clockPin,dataPin,busyPin);
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *RMotor = AFMS.getMotor(1);
Adafruit_DCMotor *LMotor = AFMS.getMotor(2);

void setup() {
  SoundModule.reset();
  SoundModule.unmute();
  SoundModule.asyncPlayVoice(10);

  pinMode(LBumperPin, INPUT);
  pinMode(RBumperPin, INPUT);
  pinMode(MoodPin, INPUT);
  pinMode(GreenEyePin, OUTPUT);
  pinMode(RedEyePin, OUTPUT);

  AFMS.begin();  

}
void loop() {
  iLBumperStateNew = digitalRead(LBumperPin);
  iRBumperStateNew = digitalRead(RBumperPin);
  iMoodState = digitalRead(MoodPin);

  if (iMoodState == HIGH)
  {
    digitalWrite(RedEyePin, LOW);
    digitalWrite(GreenEyePin,HIGH);
  }
  else
  {
    digitalWrite(GreenEyePin, LOW);
    digitalWrite(RedEyePin,HIGH);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
  }

  if (iLBumperStateNew == HIGH or iRBumperStateNew == HIGH)
  {
    if (iMoodState == HIGH)  
    {
      if (iPlayVal >= 0 and iPlayVal <= 2)  iPlayVal = iPlayVal + 3;
    }
    else
    {
      if (iPlayVal >= 3 and iPlayVal <= 5) iPlayVal = iPlayVal - 3;
    }

    if (iLBumperStateNew == HIGH)  ReverseLeft();
    
    if (iRBumperStateNew == HIGH)  ReverseRight();
    
    iPlayVal = iPlayVal + 1;
    if (iPlayVal > 5)  iPlayVal = 3; 
  }
  else
  {
    GoForward();
  }

  delay(100);
}

void GoForward()
{
  LMotor->run(FORWARD);
  RMotor->run(FORWARD);
  LMotor->setSpeed(255);
  RMotor->setSpeed(255);
}

void ReverseLeft()
{
  LMotor->run(BACKWARD);
  RMotor->run(BACKWARD);
  LMotor->setSpeed(100);
  RMotor->setSpeed(255);
  SoundModule.asyncPlayVoice(iPlayVal);
  SoundModule.stopVoice();
  delay(2000);
  LMotor->run(RELEASE);
  RMotor->run(RELEASE);
}

void ReverseRight()
{
  LMotor->run(BACKWARD);
  RMotor->run(BACKWARD);
  LMotor->setSpeed(255);
  RMotor->setSpeed(100);
  SoundModule.asyncPlayVoice(iPlayVal);
  SoundModule.stopVoice();
  delay(2000);
  LMotor->run(RELEASE);
  RMotor->run(RELEASE);
}

