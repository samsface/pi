#include "servo.hpp"
#include "gyro.hpp"
#include "kisslog.hpp"
#include "kissvec.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

int main() {
   auto& sr = servoRail::instance();
   if(!sr.error().empty()) {
      LOG_ERROR(sr.error());
      return 0;
   }

   int x = 0;
   auto& gy = gyro::instance();
  
   std::cout << "Zeroing gyroscope, stay still." << std::endl; 
   auto r = gy.rotation();
   while(r[1] > 0.1 ||  r[1] < -0.1) {
      r = gy.rotation();   
      std::cout << r[1] << std::endl;
   }

   int sockfd;
   struct sockaddr_in servaddr = {0};
   char sendline[512];
   sockfd = socket(AF_INET,SOCK_DGRAM,0);
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr("192.168.0.3");
   servaddr.sin_port = htons(7000);  
 
   for(int i = 0; i < 1500; i++) {
      auto r = gy.rotation();
      float ramp = 1;
      //std::cout << r[0] << " " << r[1] << " " << r[2] << std::endl;
      sr[0].setPower(sr[0].power()+(r[0] < 0 || r[1] > 0 ? ramp : -ramp )); 
      sr[1].setPower(sr[1].power()+(r[0] > 0 || r[1] < 0 ? ramp : -ramp )); 
      sr[2].setPower(sr[2].power()+(r[0] > 0 || r[1] > 0 ? ramp : -ramp )); 
      sr[3].setPower(sr[3].power()+(r[0] < 0 || r[1] < 0 ? ramp : -ramp )); 
 
      //for(int i = 2; i < 3; i++) 
        // sr[i].setPower(sr[i].power()+(rotations[i][1] < 0 ? 0.01 : -0.01 ));
      /*
      if(x++ > 100) {
         sprintf(sendline, "%f %f %f %f %f %f %f %f %f %f %f %f\n",
         a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2], d[0], d[1], d[2]);
         sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)); 
         x = 0;
      }
      */
   }

   sr[0].setPower(0);
   sr[1].setPower(0);
   sr[2].setPower(0);
   sr[3].setPower(0);

}
