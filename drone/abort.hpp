#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>

template <typename Op>
void abortseq(Op o) {
   o();

   struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = nullptr;
   sigemptyset(&sigIntHandler.sa_mask);  
   sigIntHandler.sa_flags = 0;

   sigaction(SIGINT, &sigIntHandler, NULL);
}
