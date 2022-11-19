#define leftmP 11
#define leftmN 10
#define rightmP 12
#define rightmN 13
#define leftmS A0
#define rightmS A1
#define ir1 7
#define ir2 6
#define ir3 5
#define ir4 4
#define ir5 3
#define ir6 2

int irValues[7],STOP = 5;

void setup() 
{
  Serial.begin(9600);
  pinMode(leftmS,OUTPUT);
  pinMode(rightmS,OUTPUT);
  pinMode(leftmP,OUTPUT);
  pinMode(leftmN,OUTPUT);
  pinMode(rightmP,OUTPUT);
  pinMode(rightmN,OUTPUT);
  pinMode(ir1,INPUT);
  pinMode(ir2,INPUT);
  pinMode(ir3,INPUT);
  pinMode(ir4,INPUT);
  pinMode(ir5,INPUT);
  pinMode(ir6,INPUT);
}

void loop()
{
  readSensor(); /*Reads from IR sensors*/
  printreading(); /*Print out the readings*/
  actuateMotor(); /*Run motor*/
  Serial.println(); /*Newline*/
}

void readSensor(){
  irValues[1] = digitalRead(ir1);
  irValues[2] = digitalRead(ir2);
  irValues[3] = digitalRead(ir3);
  irValues[4] = digitalRead(ir4);
  irValues[5] = digitalRead(ir5);
  irValues[6] = digitalRead(ir6);
}

void printreading(){
  Serial.print(
    String(irValues[1])
    +String(irValues[2])
    +String(irValues[3])
    +String(irValues[4])
    +String(irValues[5])
    +String(irValues[6])
    +">>"
    +String(STOP)
    +">>"
  ); //Prints out the readings!
}

void actuateMotor()
{
  //Group of 4
  if(irValues[1] == 1 && irValues[2] == 1 && irValues[3] == 1 && irValues[4] == 1 && irValues[5] == 0 && irValues[6] == 0) //1 - 111100
    left();         //1
  if(irValues[1] == 0 && irValues[2] == 0 && irValues[3] == 1 && irValues[4] == 1 && irValues[5] == 1 && irValues[6] == 1) //2 - 001111
    right();        //2
  //Group of 3
  if(irValues[1] == 0 && irValues[2] == 1 && irValues[3] == 1 && irValues[4] == 1 && irValues[5] == 0 && irValues[6] == 0) //3 - 011100
    left();         //3
  if(irValues[1] == 0 && irValues[2] == 0 && irValues[3] == 1 && irValues[4] == 1 && irValues[5] == 1 && irValues[6] == 0) //4 - 001110
    right();        //4
  if(irValues[1] == 1 && irValues[2] == 1 && irValues[3] == 1 && irValues[4] == 0 && irValues[5] == 0 && irValues[6] == 0) //5 - 111000
    left();         //5
  if(irValues[1] == 0 && irValues[2] == 0 && irValues[3] == 0 && irValues[4] == 1 && irValues[5] == 1 && irValues[6] == 1) //6 - 000111
    right();        //6
  //Group of 2
  if(irValues[1] == 0 && irValues[2] == 1 && irValues[3] == 1 && irValues[4] == 0 && irValues[5] == 0 && irValues[6] == 0) //7 - 011000
    left();         //7
  if(irValues[1] == 1 && irValues[2] == 1 && irValues[3] == 0 && irValues[4] == 0 && irValues[5] == 0 && irValues[6] == 0) //8 - 110000
    left();         //8
  if(irValues[1] == 0 && irValues[2] == 0 && irValues[3] == 0 && irValues[4] == 1 && irValues[5] == 1 && irValues[6] == 0) //9 - 000110
    right();        //9
  if(irValues[1] == 0 && irValues[2] == 0 && irValues[3] == 0 && irValues[4] == 0 && irValues[5] == 1 && irValues[6] == 1) //10 - 000011
    right();        //10
  if(irValues[1] == 0 && irValues[2] == 0 && irValues[3] == 1 && irValues[4] == 1 && irValues[5] == 0 && irValues[6] == 0) //11 - 001100
    forward();      //11
  //Group of 1
  if(irValues[1] == 0 && irValues[2] == 0 && irValues[3] == 1 && irValues[4] == 0 && irValues[5] == 0 && irValues[6] == 0) //12 - 001000
    forward();      //12
  if(irValues[1] == 0 && irValues[2] == 1 && irValues[3] == 0 && irValues[4] == 0 && irValues[5] == 0 && irValues[6] == 0) //13 - 010000 
    left();         //13
  if(irValues[1] == 1 && irValues[2] == 0 && irValues[3] == 0 && irValues[4] == 0 && irValues[5] == 0 && irValues[6] == 0) //14 - 100000
    left();         //14
  if(irValues[1] == 0 && irValues[2] == 0 && irValues[3] == 0 && irValues[4] == 1 && irValues[5] == 0 && irValues[6] == 0) //15 - 000100
    forward();      //15
  if(irValues[1] == 0 && irValues[2] == 0 && irValues[3] == 0 && irValues[4] == 0 && irValues[5] == 1 && irValues[6] == 0) //16 - 000010
    right();        //1
  if(irValues[1] == 0 && irValues[2] == 0 && irValues[3] == 0 && irValues[4] == 0 && irValues[5] == 0 && irValues[6] == 1) //17 - 000001
    right();        //17
  //All of 1
  if(irValues[1] == 1 && irValues[2] == 1 && irValues[3] == 1 && irValues[4] == 1 && irValues[5] == 1 && irValues[6] == 1) //18 - 111111
  {
    if(STOP > 0){
      forward();
      delayMicroseconds(100000);
      readSensor();
      if(irValues[1] == 0 || irValues[2] == 0 || irValues[3] == 0 || irValues[4] == 0 || irValues[5] == 0 || irValues[6] == 0) {
        STOP -= 1;
      }
    }
    else{
      fullstop();
    }
  }
  else{Serial.print("LOST-TRAIL!");}
}

void forward(){
  Serial.print("FORWARD");
  analogWrite(leftmS,180);
  analogWrite(rightmS,180); 
  digitalWrite(leftmP,HIGH);
  digitalWrite(leftmN,LOW);
  digitalWrite(rightmP,HIGH);
  digitalWrite(rightmN,LOW);  
}

void left(){
  Serial.print("LEFT");
  analogWrite(leftmS,180);
  analogWrite(rightmS,180);
  digitalWrite(leftmP,LOW);
  digitalWrite(leftmN,LOW);
  digitalWrite(rightmP,HIGH);
  digitalWrite(rightmN,LOW);  
}

void right(){
  Serial.print("RIGHT");
  analogWrite(leftmS,180);
  analogWrite(rightmS,180);  
  digitalWrite(leftmP,HIGH);
  digitalWrite(leftmN,LOW);
  digitalWrite(rightmP,LOW);
  digitalWrite(rightmN,LOW);
}

void fullstop(){
  Serial.print("STOP");
  analogWrite(leftmS,0);
  analogWrite(rightmS,0);  
  digitalWrite(leftmP,LOW);
  digitalWrite(leftmN,LOW);
  digitalWrite(rightmP,LOW);
  digitalWrite(rightmN,LOW);
}
