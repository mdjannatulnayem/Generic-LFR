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

int irValues[7],STOP = 4;

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
  printReading(); /*Print out the readings*/
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
  if(
     //1 - 110011
    (irValues[1] == 1 && irValues[2] == 1 && irValues[3] == 0 && irValues[4] == 0 && irValues[5] == 1 && irValues[6] == 1) 
    ||
     //2 - 100111
    (irValues[1] == 1 && irValues[2] == 0 && irValues[3] == 0 && irValues[4] == 1 && irValues[5] == 1 && irValues[6] == 1) 
    ||
     //3 - 111001
    (irValues[1] == 1 && irValues[2] == 1 && irValues[3] == 1 && irValues[4] == 0 && irValues[5] == 0 && irValues[6] == 1)
    ||
     //4 - 110111
    (irValues[1] == 1 && irValues[2] == 1 && irValues[3] == 0 && irValues[4] == 1 && irValues[5] == 1 && irValues[6] == 1)
    ||
     //5 - 111011
    (irValues[1] == 1 && irValues[2] == 1 && irValues[3] == 1 && irValues[4] == 0 && irValues[5] == 1 && irValues[6] == 1)
    ||
     //6 - 101111
    (irValues[1] == 1 && irValues[2] == 0 && irValues[3] == 1 && irValues[4] == 1 && irValues[5] == 1 && irValues[6] == 1)
    ||
     //7 - 111101
    (irValues[1] == 1 && irValues[2] == 1 && irValues[3] == 1 && irValues[4] == 1 && irValues[5] == 0 && irValues[6] == 1) 
    ){
      // Invert the readings!
     irValues[1] = !irValues[1];
     irValues[2] = !irValues[2];
     irValues[3] = !irValues[3];
     irValues[4] = !irValues[4];
     irValues[5] = !irValues[5];
     irValues[6] = !irValues[6];
  }
  
}

void printReading(){
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
  int lft,rgt,diff,summ;
  
  lft = irValues[1] + irValues[2] + irValues[3];
  rgt = irValues[4] + irValues[5] + irValues[6];
  
  diff = lft - rgt;
  summ = lft + rgt;
  
  if((diff == 1 && irValues[3]) == 1 || (diff == -1 && irValues[4] == 1))
    forward();
  else if(diff > 0) left();
  else if(diff < 0) right();
  else if(irValues[3] && irValues[4] && summ != 6)
    forward();
  else{/*No activity*/}
  if(summ == 6)
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
  // Superloop ends!
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
