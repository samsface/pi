#include "servo.hpp"
#include "gyro.hpp"
#include "kisslog.hpp"
#include "kissvec.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
   auto& sr = servoRail::instance();
   if(!sr.error().empty()) {
      LOG_ERROR(sr.error());
      return 0;
   }

   int x = 0;
   auto& gy = gyro::instance();


   int sockfd;
   struct sockaddr_in servaddr = {0};
   char sendline[512];
   sockfd = socket(AF_INET,SOCK_DGRAM,0);
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr("192.168.0.3");
   servaddr.sin_port = htons(7000);
   
   while(true) {
      auto r = gy.rotation();
      auto a = kissvec::mult({ -1.0, -1.0, 0.0 }, r);
      auto b = kissvec::mult({ -1.0,  1.0, 0.0 }, r);
      auto c = kissvec::mult({  1.0,  1.0, 0.0 }, r);
      auto d = kissvec::mult({  1.0, -1.0, 0.0 }, r);      
        
      sr[0].setPower(sr[0].power()+(a[0] < 0 ? 0.001 : -0.001 ));

      if(x++ > 100) {
         sprintf(sendline, "%f %f %f %f %f %f %f %f %f %f %f %f\n",
         a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2], d[0], d[1], d[2]);
         sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)); 
         x = 0;
      }
   }
}
