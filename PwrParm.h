#ifndef PwrParm_h
#define PwrParm_h
 
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif

class PwrParm {
public:
	PwrParm();
	
  int acOffset=0;
  float currentRmsOffset=0.0;
	  void initPfCal(const int voltagePin,const int currentPin);
    double calPf(void);
    int interruptState();
    void stop(const int voltagePin,const int currentPin);
    float rmsCurrent(const int sensorPin);
    float rmsVolatge(const int sensorPin);
};
#endif