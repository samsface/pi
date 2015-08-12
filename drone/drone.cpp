#include "servo.hpp"
#include "gyro.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include "pid.hpp"
#include <algorithm>

const int ROLL= 0;
const int PITCH = 1;
const int YAW = 2;

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;

int main() {
   auto& gy = gyro::instance();
   auto& pi = pins::instance();

   std::vector<servo> servos = {
      servo(4, 1.2, 1.5, pi.pwm),
      servo(5, 1.2, 1.5, pi.pwm),
      servo(6, 1.2, 1.5, pi.pwm),
      servo(7, 1.2, 1.5, pi.pwm)
   };

   std::vector<pid> pids = {
      pid(1, 1, 1),
      pid(1, 1, 1),
      pid(1, 1, 1)
   };

   std::vector<float> corrections = { 0, 0, 0 };
 
   while(true) {
      auto r = gy.rotation();
      for(int i = 0; i < r.size(); i++)
         corrections[i] = pids[i].compute(r[i], 0);
      
      servos[NORTH].incPower(corrections[PITCH] - corrections[YAW]);
      servos[SOUTH].decPower(corrections[PITCH] - corrections[YAW]);
      servos[WEST].incPower(corrections[PITCH] + corrections[YAW]);
      servos[EAST].decPower(corrections[PITCH] + corrections[YAW]);
   }

   for(auto& s : servos)
      s.setPower(0);
}
