#include <AFMotor.h>

AF_DCMotor M_left(3);
AF_DCMotor M_right(1);

int irValues[7];
int l = 0,r = 0,diff = l - r,prevDiff = 0;
int LMS = 160,RMS = 160;

//PID properties
//~ Kp = 22 ,Kd = 38, Ki = 1.7
float P = 0,I = 0,D = 0,kp = 0,ki = 0,kd = 0;
float prevI = 0,PIDtotal;

void Forward();
void Back();
void softLeft();
void softRight();
void hardLeft();
void hardRight();
void Stop();
void Search();
void readSensor();
void CalcPid();
void pidAction();
void actuateM();

void setup() {
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
}

//*SUPERLOOP
void loop() {
  readSensor(); //read from iR sensors
  CalcPid(); //generate pid parameters
  pidAction(); //set motor speed (pwm)
  actuateM(); //actuate motors accordingly
}

void readSensor(){
  irValues[1] = digitalRead(A3);
  irValues[2] = digitalRead(A4);
  irValues[3] = digitalRead(A2);
  irValues[4] = digitalRead(A5);
  irValues[5] = digitalRead(A1);
  irValues[6] = digitalRead(A6);
  l = irValues[1] + irValues[3] + irValues[5];
  r = irValues[2] + irValues[4] + irValues[6];
  diff = l - r;
}

void CalcPid(){
  P = diff;
  I += prevI;
  D = diff - prevDiff;
  PIDtotal = kp * P;
  PIDtotal += ki * I;
  PIDtotal += kd * D;
  prevDiff = diff;
  prevI = I;
}

void pidAction(){
  LMS -= PIDtotal;
  RMS += PIDtotal;
  constrain(LMS,0,180);
  constrain(RMS,0,180);
  M_left.setSpeed(LMS);
  M_right.setSpeed(RMS);
}

void actuateM()
{
  if(diff == 0)
    Forward();
  else if(diff > 0)
    softRight();
  else if(diff < 0)
    softLeft();
  else if(diff == 3)
    hardRight();
  else if(diff == -3)
    hardLeft();
  else
    Stop();    
}

void Forward()
{
  M_left.setSpeed(LMS);
  M_right.setSpeed(RMS);
  M_left.run(FORWARD);
  M_right.run(FORWARD);
}

void Back()
{
  M_left.setSpeed(LMS);
  M_right.setSpeed(RMS);
  M_left.run(BACKWARD);
  M_right.run(BACKWARD);
}

void softLeft()
{
  M_left.setSpeed(0);
  M_right.setSpeed(RMS);
  M_left.run(RELEASE);
  M_right.run(FORWARD);
}

void softRight()
{
  M_left.setSpeed(LMS);
  M_right.setSpeed(0);
  M_left.run(FORWARD);
  M_right.run(RELEASE);
}

void hardLeft()
{
  M_left.setSpeed(LMS);
  M_right.setSpeed(RMS);
  M_left.run(BACKWARD);
  M_right.run(FORWARD);
}

void hardRight()
{
  M_left.setSpeed(LMS);
  M_right.setSpeed(RMS);
  M_left.run(FORWARD);
  M_right.run(BACKWARD);
}

void Stop()
{
  M_left.setSpeed(0);
  M_right.setSpeed(0);
  M_left.run(RELEASE);
  M_right.run(RELEASE);
}

