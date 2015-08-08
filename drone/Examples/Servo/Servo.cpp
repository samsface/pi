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

using namespace Navio;

class servo
{
   float _minPWMmS, _maxPWMmS, _currentPWMmS;
public:   
   servo(float minPWMmS, float maxPWMmS) : 
   _minPWMmS(minPWMmS), 
   _maxPWMmS(maxPWMmS),
   _currentPWMmS(_minPWMmS) {}

   //servo() : servo(1.250, 1.340) {}
};

class servoRail
{
   std::string _error;

   servoRail() {
      Pin pin(RPI_GPIO_27);
      if(pin.init()) {
         pin.setMode(Pin::GpioModeOutput);
         pin.write(0);
      
         PCA9685 pwm;
         pwm.initialize();
         pwm.setFrequency(50);
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

   const std::string& error() const { return _error; } 
};

int main() {
   servoRail& sr = servoRail::instance();
   if(!sr.error().empty()) {
      std::cout << sr.error() << std::endl;
      return 0;
   }

   return 0;
}
