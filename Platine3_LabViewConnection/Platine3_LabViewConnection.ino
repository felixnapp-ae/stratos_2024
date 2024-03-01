#include <SPI.h>


byte a;
unsigned int data;

int Sensor;
 char sendeInhalt = ' ';

const int DRDY = 6;

int M=0;
int s=255;

//SPIClass SPI(VSPI);

void setup() {
Serial.begin(9600);

pinMode(8, OUTPUT);
//digitalWrite(8, LOW);

  //while (!Serial) {}
  //MUX Ch1:
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);

  setMUX(2);

//MUX Ch2:
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);

  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);


  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);

  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);



  pinMode(DRDY, INPUT);
  

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV32);

  //init:
  initCh1();
  initCh2();

  //attachInterrupt(digitalPinToInterrupt(6), isrM(), CHANGE);
}

void loop() {

  Serial.print(Ch1());
  Serial.print(";");
  Serial.println(Ch2());

  if (Serial.available() > 0) 
  {
    digitalWrite(13,HIGH);
    sendeInhalt = Serial.read(); 
 
    if (sendeInhalt == '1') { Sensor=1; }
    if (sendeInhalt == '2') { Sensor=2; }  
    if (sendeInhalt == '3') { Sensor=3; }  
    if (sendeInhalt == '4') { Sensor=4; }  

    setMUX(Sensor);

    Serial.flush(); //seriellen Puffer löschen
  }
}


unsigned int Ch1() {
  while (!dataReadyCh1()) {}
  SPI.transfer(56);
  unsigned int data = SPI.transfer16(0);

  return data;
}

unsigned int Ch2() {
  while (!dataReadyCh2()) {}
  SPI.transfer(57);
  unsigned int data = SPI.transfer16(0);

  return data;
}

bool dataReadyCh1() {
  SPI.transfer(8);
  byte b1 = SPI.transfer(0);
  return (b1 & 0x80) == 0x0;
}

bool dataReadyCh2() {
  SPI.transfer(9);
  byte b1 = SPI.transfer(0);
  return (b1 & 0x80) == 0x0;
}

void initCh1() {
  SPI.transfer(32);
  SPI.transfer(9);

  SPI.transfer(16);
  SPI.transfer(68);

  while (!dataReadyCh1()) {}
}

void initCh2() {
  SPI.transfer(33);
  SPI.transfer(9);

  SPI.transfer(17);
  SPI.transfer(68);

  while (!dataReadyCh2()) {}
}

void setMUX(int c){
switch (c)
  {
  case 1:
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    break;

    case 2:
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
    break;

    case 3:
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    break;

    case 4:
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
    break;
  
  default:
  Serial.print("MUX chanal failed");
    break;
  }

}

void isrM()
{
M=M+1;
}

void irs_timer()
{
  if(M>6)
  {
    s=s-2;
    if (s<0){s=0;}
  }
  if(M<6)
  {
    s=s+2;
    if (s>255){s=255;}   
  }
M=0;
analogWrite(9, s);
}