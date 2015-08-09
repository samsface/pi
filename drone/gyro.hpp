/*
To achieve stable loop you need to run this application with a high priority
on a linux kernel with real-time patch. Raspbian distribution with real-time
kernel is available at emlid.com and priority can be set with chrt command:
chrt -f -p 99 PID
*/

#include "Navio/MPU9250.h"
#include "Navio/AHRS.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

class gyro 
{
   MPU9250 _imu;
   AHRS _ahrs;
   std::vector<float> _accel;
   std::vector<float> _gyro;
   std::vector<float> _mag;
   std::chrono::high_resolution_clock::time_point _t0, _t1;

   gyro() : _accel(3), _gyro(3), _mag(3) {
      _imu.initialize();

      std::vector<float> offset(3);
      for(int i = 0; i< 100; i++) {
         _imu.getMotion6(&_accel[0], &_accel[1], &_accel[2], 
                         &_gyro[0],  &_gyro[1],  &_gyro[2]);
	 offset[0] -= _gyro[0]*0.0175;
         offset[1] -= _gyro[1]*0.0175;
         offset[2] -= _gyro[2]*0.0175;
         std::this_thread::sleep_for(std::chrono::microseconds(10000));
      }
      
      _ahrs.setGyroOffset(offset[0]/100.0, offset[1]/100.0, offset[2]/100.0);
   }

public:
   static gyro& instance() {
      static gyro i;
      return i;
   }

   std::vector<std::vector<float>> rotation() {  
      _t0 = _t1;
      _t1 = std::chrono::high_resolution_clock::now();
      auto dt = std::chrono::duration_cast<std::chrono::microseconds>(_t1-_t0).count()/1000000.0;
      
      _imu.getMotion9(&_accel[0], &_accel[1], &_accel[2],
                      &_gyro[0],  &_gyro[1],  &_gyro[2],
                      &_mag[0],   &_mag[1],   &_mag[2]);

      _mag[0] = (_mag[0]-17.0317)/100; // Gauss, IMU output = micro Tesla
      _mag[1] = (_mag[1]-25.7831)/100; // Gauss
      _mag[2] = (_mag[2]+12.5051)/100; // Gauss

      _ahrs.update(_accel[0],       _accel[1],       _accel[2], 
                   _gyro[0]*0.0175, _gyro[1]*0.0175, _gyro[2]*0.0175,
                   _mag[0],         _mag[1],         _mag[2]*-1,        
                   dt);

      float x = _ahrs.getX();
      float y = _ahrs.getY();
      float z = _ahrs.getZ();
      float w = _ahrs.getW();
      #define scf(x) static_cast<float>(x)
      return {{ scf(1.0-2.0*(y*y+z*z)),  scf(2.0*(x*y+w*z)),      scf(2.0*(x*z-w*y))     },
              { scf(2.0*(x*y-w*z)),      scf(1.0-2.0*(x*x+z*z)),  scf(2.0*(y*z+w*x))     },
              { scf(2.0*(x*z+w*y)),      scf(2.0*(y*z-w*x)),      scf(1.0-2.0*(x*x+y*y)) }};
      #undef scf
   }
};