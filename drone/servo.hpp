#include "Navio/gpio.h"
#include "Navio/PCA9685.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace Navio;

class servo
{
   PCA9685 _pwm;
   int _rcOut;
   float _minPWMms, _maxPWMms, _power;
public:   
   servo(int rcOut, float minPWMms, float maxPWMms, const PCA9685& pwm) : 
   _rcOut(rcOut+2),
   _minPWMms(minPWMms), 
   _maxPWMms(maxPWMms),
   _pwm(pwm),
   _power(0) {}
 
   void setPower(float p) {
      if(p > 1) p = 1;
      if(p < 0) p = 0;
      _power = p;
      auto newPWMms = _minPWMms + (_maxPWMms-_minPWMms)*p;
      _pwm.setPWMmS(_rcOut, newPWMms);    
   }

   void  incPower(float p) { setPower(_power + p); };
   void  decPower(float p) { setPower(_power - p); };

   float power() { return _power; }
};

class pins {
   pins() {
      Pin pin(RPI_GPIO_27);
      if(pin.init()) {
         pin.setMode(Pin::GpioModeOutput);
         pin.write(0);
   
         pwm.initialize();
         pwm.setFrequency(50);
      }   
   }
public:
   PCA9685 pwm;

   static pins& instance() {
      static pins i;
      return i;
   }
};
