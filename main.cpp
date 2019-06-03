#include <iostream>
#include "queue.h"
#include "host.h"
#include "frame.h"

using namespace std;

double negexdistime(double rate){
	double u;
	u = drand48();
	return ((-1/rate)*log(1-u));
}

int main(int argc, char *argv[])
{
	double lambda = atoi(argv[1]);
	int numframes = 0;
	Queue * globalframeq = new Queue;
	while(globalframeq->getSize()!=0){
		Host * source;
		Host* destination;
		transmissiontime = transmissiontime + negexdistime(source->lambda);
		char type;
		Frame  fr(length, source, destination, transmissiontime, type);
		source->frameQueue.enqueue(fr);
	if(globalframeq->getSize()==0){
		
	}
	else if(globalframeq->getSize()!=0){
		source->backoffno = source->backoffno - 1;
		if(source->backoff==0){
			globalframeq.enqueue(fr);
		}
	}
	}


}
