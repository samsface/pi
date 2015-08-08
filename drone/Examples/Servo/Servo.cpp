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

#include <Navio/gpio.h>
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
   float _minPWMms, _maxPWMms, _currentPWMms;
   std::string _error;
public:   
   servo(int rcOut, float minPWMms, float maxPWMms) :
   _rcOut(rcOut), 
   _minPWMms(minPWMms), 
   _maxPWMms(maxPWMms),
   _currentPWMms(_minPWMms) {}
 
   void setPower(float p) {
      if(!_error.empty())
         return;
      if(p > 1) p = 1;
      if(p < 0) p = 0;
      _currentPWMms = _minPWMms + (_maxPWMms-_minPWMms)*p;
      _pwm.setPWMmS(_rcOut, _currentPWMms);    
   }

   void rev(float p) {
      setPower(p);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      setPower(0);
   }

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
            _servos.push_back(std::move(servo(s, 1.250, 1.340)));
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

int main() {
   servoRail& sr = servoRail::instance();
   if(!sr.error().empty()) {
      std::cout << sr.error() << std::endl;
      return 0;
   }

   sr[0].rev(0.5);

   return 0;
}
