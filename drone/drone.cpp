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

const auto MAX_PWM = 1.34;
const auto MIN_PWM = 1.2;

int main() {
   auto& gy = gyro::instance();
   auto& pi = pins::instance();

   std::vector<servo> servos = {
      servo(4, MIN_PWM, MAX_PWM, pi.pwm),
      servo(5, MIN_PWM, MAX_PWM, pi.pwm),
      servo(6, MIN_PWM, MAX_PWM, pi.pwm),
      servo(7, MIN_PWM, MAX_PWM, pi.pwm)
   };

   std::vector<pid> pids = {
      pid(1, 0, 0),
      pid(1, 0, 0),
      pid(1, 0, 0)
   };

   std::vector<float> corrections = { 0, 0, 0 };
 
   for(int j = 0; j < 9000; j++) {
      auto r = gy.rotation();
      for(int i = 0; i < r.size(); i++)
         corrections[i] = pids[i].compute(r[i], 0);
      
      std::cout << corrections[ROLL] << " " << corrections[PITCH] << " " << corrections[YAW] << " " << std::endl;
      corrections[YAW] = 0;

      servos[NORTH].incPower(corrections[PITCH] - corrections[YAW]);
      servos[SOUTH].decPower(corrections[PITCH] - corrections[YAW]);
      servos[WEST].incPower(corrections[ROLL] + corrections[YAW]);
      servos[EAST].decPower(corrections[ROLL] + corrections[YAW]);
   }

   for(auto& s : servos)
      s.setPower(0);
}
