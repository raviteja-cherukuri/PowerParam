#include <math.h>
#include "PwrParm.h"
volatile int timeLapse=0;
  volatile int state=0;

PwrParm::PwrParm()
{
}

void startTimer(void)
{
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0s
  TCCR1B |= (1 << CS11);//8 as prescalar
}


float stopTimer(void)
{ 
  int rawCount=TCNT1;
  float time;
  time=rawCount*0.000001;//0.000001=2*8/16MHz
  return time;
}

void startInterrupt(const int zcPin, void handleInterrupt())
 {
  pinMode(zcPin,INPUT);
  attachInterrupt(digitalPinToInterrupt(zcPin), handleInterrupt, RISING);
 }

void handleCurrentInterrupt(void)
{
   //getTime();
   //setState(2);
   timeLapse=TCNT1;
   state=2;
}

void handleVoltageInterrupt(const int zcPin,void handleCurrentInterrupt(void))
{    
   TCNT1=0;
   startInterrupt(zcPin, handleCurrentInterrupt);
   //setState(1);
   state=1;
}


void stopInterrupt(const int zcPin)
{
detachInterrupt(zcPin);
}

double powerFactor(float timeGap)
{ 
  if(timeGap>0.01){
     timeGap=0.02-timeGap;
  }
  return cos(timeGap*PI/0.01);
}

void PwrParm::stop(const int voltagePin,const int currentPin)
{
  stopInterrupt(voltagePin);
  stopInterrupt(currentPin);
}



void PwrParm::initPfCal(const int voltagePin,const int currentPin)
{
  state=0;
  startTimer();
  startInterrupt(voltagePin,handleVoltageInterrupt);
  handleVoltageInterrupt(currentPin,handleCurrentInterrupt);
}

double PwrParm::calPf(void)
{
  if(state==2){
  	float timeGap=timeLapse*0.000001;// 0.000001=2*8/16MHz
    return powerFactor(timeGap);
  }else{
  	return 0;
  }
}

int PwrParm::interruptState(){
    return state;
}

void setState(volatile int i){
 //state=i;
}

void getTime(){
  //timeLapse=TCNT1;
}

float PwrParm::rmsCurrent(const int sensorPin)
 {
  float total=0.0;
  float current=0.0;
  float currentRms=0.0;
  int sensorValue=0,i=0;
  unsigned long period=millis();
  while((millis()-period)<21){
    sensorValue = analogRead(sensorPin);
    current=(sensorValue-acOffset)*1000.0*5.0/1024.0/185.0;
    total=total+(current*current);
    i+=1  ;
    }
  currentRms=sqrt(total/i)-currentRmsOffset;
  return currentRms;
 }

 float PwrParm::rmsVolatge(const int sensorPin)
 {
  float total=0.0;
  float voltage=0.0;
  float voltageRms=0.0;
  int sensorValue=0,i=0;
  unsigned long period=millis();
  while((millis()-period)<21){
    sensorValue = analogRead(sensorPin);
    voltage=sensorValue*5.0/1024.0;
    total+=voltage*voltage;
    i+=1  ;
    }
  voltageRms=sqrt(total/i);
  //equation to be found
  return voltageRms;
 }