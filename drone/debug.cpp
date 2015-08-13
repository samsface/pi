#include "servo.hpp"
#include "gyro.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include "pid.hpp"
#include <algorithm>

const auto MAX_PWM = 1.5;
const auto MIN_PWM = 1.25;

int main(int argc, char **argv) {
   if(argc < 2) {
      std::cerr << "Type a pin number." << std::endl;
      return 0;
   }
   int pin = atoi(argv[1]);
   if(pin+2 < 3 || pin+2 > 15) {
      std::cerr << "Pin value must be between 1-13." << std::endl;
      return 0;
   }

   auto& pi = pins::instance();
   servo s(pin, MIN_PWM, MAX_PWM, pi.pwm);

   s.setPower(0.0);
   s.setPower(1);
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   s.setPower(0);
   
   return 0; 
}
