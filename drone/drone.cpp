#include "servo.hpp"
#include "gyro.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include "pid.hpp"
#include <algorithm>
#include <signal.h>

const auto ROLL= 0;
const auto PITCH = 1;
const auto YAW = 2;

const auto NORTH = 0;
const auto SOUTH = 1;
const auto EAST = 2;
const auto WEST = 3;

auto MAX_PWM = 1.48;
const auto MIN_PWM = 1.2;

template <typename Servo, typename Pid>
struct drone 
{
   std::vector<Servo> _servos;
   std::vector<Pid> _pids;
   bool _stop;

   drone(decltype(_servos) servos, decltype(_pids) pids) : _servos(servos), _pids(pids), _stop(false) {}

   void run() {
      _stop = false;
      auto& gy = gyro::instance();

      std::vector<float> corrections = { 0, 0, 0 };
 
      while(true) {
         auto r = gy.rotation();

         if(r[0] > 90 || r[0] < -90 || r[1] > 90 || r[1] < -90) {
            std::cout << "Aborted, roll and pitch must stay in range -90 - 90." << std::endl;
            stop();
         }

         corrections[ROLL] = _pids[ROLL].correct(r[ROLL], 0);
         corrections[PITCH] = _pids[PITCH].correct(r[PITCH], 0);
         corrections[YAW] = _pids[YAW].correct(r[YAW], 0);
 
         if(_stop)
            break;              

         _servos[NORTH].incPower(-1*corrections[PITCH]);
         _servos[SOUTH].incPower(corrections[PITCH]);
         _servos[EAST].incPower(-1*corrections[ROLL]);
         _servos[WEST].incPower(corrections[ROLL]);
      }

      stop();
   }

   void stop() { _stop = true; for(auto& s: _servos) s.setPower(0); }
};

drone<servo, pid>* stupid = nullptr;
void ting(int s) { if(stupid) stupid->stop(); }

int main(int argc, char **argv) {

   if(argc < 5) {
      std::cout << "Invalid number of args." << std::endl;
      return 0;
   }

   auto maxPwm = atof(argv[1]);
   if(maxPwm < 1.34 || maxPwm > 1.7) {
      std::cout << "Max pulse width needs to be in range 1.34-1.7." << std::endl;
      return 0;
   }

   auto pGain = atof(argv[2]);
   if(pGain < 0) {
      std::cout << "Proportional gain needs to be larget than zero." << std::endl;
      return 0;
   }

   auto iGain = atof(argv[3]);
   if(iGain < 0) {
      std::cout << "Integral gain needs to be larget than zero." << std::endl;
      return 0;
   }

   auto dGain = atof(argv[4]);
   if(dGain < 0) {
      std::cout << "Derivative gain needs to be larget than zero." << std::endl;
      return 0;
   }

   drone<servo, pid> d(std::vector<servo>({ 
                         servo(4, 1.2, maxPwm, pins::instance().pwm),
                         servo(5, 1.2, maxPwm, pins::instance().pwm),
                         servo(6, 1.2, maxPwm, pins::instance().pwm),
                         servo(7, 1.2, maxPwm, pins::instance().pwm) }),
                       std::vector<pid>({ 
                         pid(pGain, iGain, dGain),
                         pid(pGain, iGain, dGain),
                         pid(pGain, iGain, dGain) }));
   stupid = &d;

   struct sigaction sigIntHandler;
   sigIntHandler.sa_handler = ting;
   sigemptyset(&sigIntHandler.sa_mask);  
   sigIntHandler.sa_flags = 0;
   sigaction(SIGINT, &sigIntHandler, NULL);
 
   d.run();
}
