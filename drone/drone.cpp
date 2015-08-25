#include "servo.hpp"
#include "gyro.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include "pid.hpp"
#include <algorithm>

const auto ROLL= 0;
const auto PITCH = 1;
const auto YAW = 2;

const auto NORTH = 0;
const auto SOUTH = 1;
const auto EAST = 2;
const auto WEST = 3;

auto MAX_PWM = 1.48;
const auto MIN_PWM = 1.2;

int main(int argc, char **argv) {
   auto& gy = gyro::instance();
   auto& pi = pins::instance();

   auto k = atof(argv[1]);
   if(k < 1.34 || k > 1.7) {
      std::cout << "Max pulse width needs to be in range 1.34-1.7." << std::endl;
      return 0;
   }

   MAX_PWM = k;

   std::vector<servo> servos = {
      servo(4, MIN_PWM, MAX_PWM, pi.pwm),
      servo(5, MIN_PWM, MAX_PWM, pi.pwm),
      servo(6, MIN_PWM, MAX_PWM, pi.pwm),
      servo(7, MIN_PWM, MAX_PWM, pi.pwm)
   };

   std::vector<pid> pids = {
      pid(0.01, 0.00000, 0.00000),
      pid(0.01, 0.00000, 0.00000),
      pid(0.01, 0.00000, 0.00000)
   };

   std::vector<float> corrections = { 0, 0, 0 };
 
   for(int j = 0; j < 5000; j++) {
      auto r = gy.rotation();
      for(int i = 0; i < r.size(); i++)
         corrections[i] = pids[i].compute(r[i], 0);
     
      std::cout << r[0] << " " << r[1] << " " << r[2] << std::endl;
 
      corrections[YAW] = 0;
      float dick = 10;
      servos[NORTH].incPower(-1*corrections[PITCH]);
      servos[SOUTH].incPower(corrections[PITCH]);
     // servos[EAST].incPower(-1*corrections[ROLL] - dick);
     //  servos[WEST].incPower(corrections[ROLL] + dick);
   }

   for(auto& s : servos)
      s.setPower(0);
}
