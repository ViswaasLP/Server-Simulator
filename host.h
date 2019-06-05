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

      Host(){
      	receivedFrame = NULL;
      	ackFrame = NULL;
      	lambda = 0;
      	backoffno = 0;
      }
};

#endif
