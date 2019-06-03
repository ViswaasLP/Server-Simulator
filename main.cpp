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
	while(!globalframeq->){
		Host * source;
		Host* destination;
		transmissiontime = transmissiontime + negexdistime(source->lambda);
		char type;
		Frame  fr(length, source, destination, transmissiontime, type);
		source->frameQueue.enqueue(fr);
	}


}
