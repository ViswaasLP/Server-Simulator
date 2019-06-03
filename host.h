#ifndef HOST_H
#define HOST_H
#include <vector>
class Host {
    public:
      vector<Frame*> frameQueue;
      Frame *receivedFrame;
      double lambda;
      int backoffno;

};

#endif
