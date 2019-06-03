#ifndef HOST_H
#define HOST_H
#include "queue.h"

class host {
    public:
      Queue* frameQueue;
      double lambda;
       int backoffno;

};

#endif
