#ifndef HOST_H
#define HOST_H
#include <vector>
#include "frame.h"

using namespace std;

class Host {
    public:
      vector<Frame*> frameQueue;
      Frame *receivedFrame;
      Frame *ackFrame;
      double lambda;
      int backoffno;

};

#endif
