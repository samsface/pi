/*
This code is provided under the BSD license.
Copyright (c) 2014, Emlid Limited. All rights reserved.
Written by Igor Vereninov and Mikhail Avkhimenia
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Application: Mahory AHRS algorithm supplied with data from MPU9250.
Outputs roll, pitch and yaw in the console and sends quaternion
over the network - it can be used with 3D IMU visualizer located in
Navio/Applications/3D IMU visualizer.

To run this app navigate to the directory containing it and run following commands:
make
sudo ./AHRS

If you want to visualize IMU data on another machine pass it's address and port
sudo ./AHRS ipaddress portnumber

To achieve stable loop you need to run this application with a high priority
on a linux kernel with real-time patch. Raspbian distribution with real-time
kernel is available at emlid.com and priority can be set with chrt command:
chrt -f -p 99 PID
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include "Navio/MPU9250.h"
#include "AHRS.hpp"
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
                         &_gyro[0], &_gyro[1], &_gyro[2]);
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

   void imuLoop() {  
      _t0 = _t1;
      _t1 = std::chrono::high_resolution_clock::now();
      auto dt = std::chrono::duration_cast<std::chrono::microseconds>(_t1-_t0).count();
      if(dt < 1/1300.0) 
         return;

      _imu.getMotion6(&_accel[0], &_accel[1], &_accel[2], 
                      &_gyro[0], &_gyro[1], &_gyro[2]);
      _ahrs.updateIMU(_accel[0], _accel[1], _accel[2], 
                      _gyro[0]*0.0175, _gyro[1]*0.0175, _gyro[2]*0.0175, dt);

      // Soft and hard iron calibration required for proper function.
      // imu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
      // ahrs.update(ax, ay, az, gx*0.0175, gy*0.0175, gz*0.0175, my, mx, -mz, dt);
   }
};

//=============================================================================

int main(int argc, char *argv[]) {
    gyro& g = gyro::instance();
}
