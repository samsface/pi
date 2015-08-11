/*
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Control servos connected to PCA9685 driver onboard of Navio shield for Raspberry Pi.

Connect servo to Navio's rc output and watch it work.
Output 1 on board is connected to PCA9685 channel 3, output 2 to channel 4 and so on.
To use full range of your servo correct SERVO_MIN and SERVO_MAX according to it's specification.

To run this example navigate to the directory containing it and run following commands:
make
./Servo
*/

#include "Navio/gpio.h"
#include "Navio/PCA9685.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "kisslog.hpp"

using namespace Navio;

class servo
{
   PCA9685 _pwm;
   int _rcOut;
   float _minPWMms, _maxPWMms, _power;
   std::string _error;
public:   
   servo(int rcOut, float minPWMms, float maxPWMms, const PCA9685& pwm) :
   _rcOut(rcOut), 
   _minPWMms(minPWMms), 
   _maxPWMms(maxPWMms),
   _pwm(pwm),
   _power(0) {}
 
   void setPower(float p) {
      if(!_error.empty()) return;
      if(p == _power) return;
      if(p > 1) p = 1;
      if(p < 0) p = 0;
      _power = p;
      auto newPWMms = _minPWMms + (_maxPWMms-_minPWMms)*p;
      _pwm.setPWMmS(_rcOut, newPWMms);    
   }

   void rev(float p) {
      setPower(p);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      setPower(0);
   }

   float power() { return _power; }

   const std::string& error() const { return _error; } 
};

class servoRail
{
   std::vector<servo> _servos;
   std::string _error;

   servoRail() {
      Pin pin(RPI_GPIO_27);
      if(pin.init()) {
         pin.setMode(Pin::GpioModeOutput);
         pin.write(0);
   
         PCA9685 pwm;   
         pwm.initialize();
         pwm.setFrequency(50);

         auto rcOutputs = { 4, 5, 6, 7};
         for(auto s : rcOutputs) 
            _servos.push_back(std::move(servo(s+2, 1.250, 1.46, pwm)));
      }
      else {
         _error = "Could not init pin driver. Are you root?";
      }
   }

public:
   static servoRail& instance() {
      static servoRail i;
      return i;
   }

   servo& operator[](int i) { return _servos.at(i); }

   const std::string& error() const { return _error; } 
};
