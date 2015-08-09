#include "servo.hpp"
#include "gyro.hpp"
#include "kisslog.hpp"
#include "kissvec.hpp"

int main() {
   auto& sr = servoRail::instance();
   if(!sr.error().empty()) {
      LOG_ERROR(sr.error());
      return 0;
   }

   auto& gy = gyro::instance();

    while(true) {
      auto r = gy.rotation();
      auto m = kissvec::mult({0, 1, 0}, r);
      std::cout << r[0][0] << " " << r[0][1] << " " << r[0][2] << std::endl;
    }   
}
