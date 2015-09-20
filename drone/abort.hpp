#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>

template <typename Op>
struct abortseqhack {
   Op _o;

   static abortseqhack& instance() {
      static abortseqhack i;
      return i;
   }
   
   void my_handler(int s) {
      _o();
   }
};

template <typename Op>
void abortseq(Op o) {
   o();

   struct sigaction sigIntHandler;

//   sigIntHandler.sa_handler = &(abortseqhack<Op>::my_handler);
   sigemptyset(&sigIntHandler.sa_mask);  
   sigIntHandler.sa_flags = 0;

   sigaction(SIGINT, &sigIntHandler, NULL);
}
