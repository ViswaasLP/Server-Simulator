#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <functional>
#include <math.h>
#include "host.h"
#include "frame.h"

using namespace std;

double negexdistime(double rate){
	double u;
	u = drand48();

	return ((-1/rate)*log(1-u));
}

int genRandomHost(int exclude, int mod){
	int u;
	u = (rand() % mod) + 1;

	if(u == exclude)
		return u/2;
	else
		return u;
}

int genRandomLength(){
	int u;

	// insert code for generating random frame length here
	return 10; // default for now
}

int genBackoffNumber(int T){

	return 10; // default for now
}

double getFrameTransTime(double length){

	return 1; // default for now
}

int main(int argc, char *argv[])
{
	double lambda = atof(argv[1]);
	double SIFS = 0.0005, DIFS = 0.001;
	int numhosts = atoi(argv[2]);
	int maxframes = atoi(argv[3]);
	int T = atoi(argv[4]);
	int numframes = 0;
	double currentTime = 0;
	Host hosts[50];
	vector<Frame*> frameOrder;
	vector<Frame*> globalFrameQ;

	while(numframes != maxframes){
		double timeChannelFree = 0;
		// Step 1: All hosts generate one Frame at rate Lambda
		for(int i = 0; i < numhosts; i++){
			double transTime = currentTime + negexdistime(lambda);
			Frame *newFrame = new Frame(i, genRandomHost(i, numhosts), genRandomLength(), transTime, 'r');
			frameOrder.push_back(newFrame);
		}

		sort(frameOrder.begin(), frameOrder.end());
		vector<Frame*>::iterator p;
		for(p = frameOrder.begin(); p != frameOrder.end(); ++p){
			Frame *printFrame = *p;
    	printf("Time for frame %d is %f\n",printFrame->srcHost,printFrame->time);
		}

		// Step 2: Determine what to do w/ each Frame starting with earliest generated
			for(int i = 0; i < numhosts; i++){
				Frame *currentFrame = frameOrder.back();
				frameOrder.pop_back();
				currentTime = currentFrame->time;
				int sourceHost = currentFrame->srcHost;
				int destinationHost = currentFrame->destHost;

				// "channel is idle" - transmit frame after DIFS delay
				if(globalFrameQ.empty()){
					currentFrame->time += DIFS + getFrameTransTime(currentFrame->length);
					hosts[destinationHost].receivedFrame = currentFrame;
					timeChannelFree = currentFrame->time;
					globalFrameQ.push_back(currentFrame);
					currentTime = timeChannelFree;
				}
				// "channel" is busy -  assign backoff number and put frame in queue
				else if(!globalFrameQ.empty()){
					hosts[sourceHost].backoffno = genBackoffNumber(T);
					hosts[sourceHost].frameQueue.push_back(currentFrame);
					currentTime = currentFrame->time;
				}
			}


			numframes++;
		}

}
