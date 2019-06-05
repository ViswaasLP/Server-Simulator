#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <functional>
#include <math.h>
#include <time.h>
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
	u = (rand() % mod);

	while(u == exclude)
		u = (rand() % mod);
	
	return u;
}

int genRandomLength(double rate){
	int u;
	u = negexdistime(rate);

	return u * 1544; // default for now
}

int genBackoffNumber(int T){
	int u;
	u = drand48();

	return u * T; // default for now
}

double getFrameTransTime(double length){

	return 1; // default for now
}

int main(int argc, char *argv[])
{
	double lambda = atof(argv[1]);
	double SIFS = 0.00005, DIFS = 0.0001;
	int numhosts = atoi(argv[2]);
	int maxframes = atoi(argv[3]);
	int T = atoi(argv[4]);
	double currentTime = 0;
	Host hosts[50];
	vector<Frame*> frameOrder;
	vector<Frame*> globalFrameQ;
	srand48(time(NULL));

	// Step 1: All hosts generate all frames for simulation
	for(int i = 0; i < numhosts; i++){
		int frameGenTime = 0;

		for(int j = 0; j < maxframes; j++){
			double transTime = frameGenTime + negexdistime(lambda);
			Frame *newFrame = new Frame(i, genRandomHost(i, numhosts), genRandomLength(lambda), transTime);
			frameOrder.push_back(newFrame);
			frameGenTime = newFrame->time;
			printf("Time for frame %d is %f\n",newFrame->srcHost,newFrame->time);
		}
	}
		sort(frameOrder.rbegin(), frameOrder.rend(), compareFrameTimes);

   // Step 2: Determine what to do w/ each Frame starting with earliest generated
		while(!frameOrder.empty()){
				Frame *currentFrame = frameOrder.back();
				frameOrder.pop_back();
				currentTime = currentFrame->time;
				int sourceHost = currentFrame->srcHost;
				int destinationHost = currentFrame->destHost;
				printf("Host %d's frame wants to send to host %d at time %f\n", sourceHost, destinationHost, currentTime);
				
				// "channel is idle" - transmit frame after DIFS delay
				if(globalFrameQ.empty()){
					printf("Channel is idle\n");
					currentFrame->time += DIFS + getFrameTransTime(currentFrame->length);
					hosts[destinationHost].receivedFrame = currentFrame;
					//timeChannelFree = currentFrame->time;
					globalFrameQ.push_back(currentFrame);
					printf("Host %d will need to send ACK at time %f\n", destinationHost, currentFrame->time);;
				}
				// "channel" is busy -  assign backoff number and put frame in queue
				else if(!globalFrameQ.empty()){
					printf("Channel is busy\n");
					hosts[sourceHost].backoffno = genBackoffNumber(T);
					hosts[sourceHost].frameQueue.push_back(currentFrame);
					currentTime = currentFrame->time;
				}
		}
				// Step 3: Check if we can send any acknowledgement frames
	/*			Frame *peekNextFrame = frameOrder.back();

				for(int i = 0; i < numhosts; i++){
					if(hosts[i].receivedFrame != NULL){
						if(peekNextFrame->time > timeChannelFree){
							currentTime = timeChannelFree;
							Frame *newACKFrame = new Frame(destinationHost, sourceHost, 64, currentTime + SIFS);
							hosts[sourceHost].ackFrame = newACKFrame; 
							printf("Host %d has sent ACK at time %f\n", destinationHost, newACKFrame->time);
						}
					}
				}
				*/
}
