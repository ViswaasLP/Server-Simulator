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
	double u;
	u = negexdistime(rate);

	return u * 1544; // default for now
}

int genBackoffNumber(int T){
	double u;
	u = drand48();

	return u * T; // default for now
}

double getFrameTransTime(double length){
	return (length * 8) / (11 * 106) / 1000; // default for now
}

int main(int argc, char *argv[])
{
	//srand48(time(NULL));
	double lambda = atof(argv[1]);
	double SIFS = 0.00005, DIFS = 0.0001;
	int numhosts = atoi(argv[2]);
	int maxframes = atoi(argv[3]);
	int T = atoi(argv[4]);
	double currentTime = 0, throughput = 0, delay = 0;
	int failed = 0;
	Host hosts[50];
	vector<Frame*> frameOrder;
	vector<Frame*> globalFrameQ;

	// Step 1: All hosts generate all frames for simulation
	for(int i = 0; i < numhosts; i++){
		int frameGenTime = 0;

		for(int j = 0; j < maxframes; j++){
			double transTime = frameGenTime + negexdistime(lambda);
			Frame *newFrame = new Frame(i, genRandomHost(i, numhosts), genRandomLength(lambda), transTime);
			frameOrder.push_back(newFrame);
			hosts[i].frameQueue.push_back(newFrame);
			frameGenTime = newFrame->time;
		}
	}
		sort(frameOrder.rbegin(), frameOrder.rend(), compareFrameTimes);

   // Step 2: Determine what to do w/ each Frame starting with earliest generated
	double timeChannelFree = 0;
	//int limit = 0;
	while(!frameOrder.empty() /*&& limit < 2*/){
		Frame *currentFrame = frameOrder.back();
		frameOrder.pop_back();
		currentTime = currentFrame->time;
		int sourceHost = currentFrame->srcHost;
		int destinationHost = currentFrame->destHost;
		
		if(hosts[sourceHost].sentFrame != NULL){
			if(hosts[sourceHost].ackFrame == NULL){
				hosts[sourceHost].frameQueue.pop_back();
				Frame *resendFrame = hosts[sourceHost].sentFrame;
				failed++;
				hosts[sourceHost].backoffno = genBackoffNumber(failed * T);
				resendFrame->time = DIFS + getFrameTransTime(resendFrame->length) + currentTime;
				hosts[sourceHost].frameQueue.push_back(currentFrame);
				hosts[sourceHost].frameQueue.push_back(resendFrame);
			}
			else{
				frameOrder.push_back(currentFrame);
				hosts[sourceHost].ackFrame = NULL;
				continue;
			}
		}
		// "channel is idle" - transmit frame after DIFS delay
		else if(globalFrameQ.empty()){
			currentFrame->time += DIFS + getFrameTransTime(currentFrame->length);
			hosts[destinationHost].receivedFrame = currentFrame;
			timeChannelFree = currentFrame->time;

			hosts[sourceHost].sentFrame = currentFrame;
			globalFrameQ.push_back(currentFrame);
		}
		// "channel" is busy -  assign backoff number and put frame in queue
		else if(!globalFrameQ.empty()){
			hosts[sourceHost].backoffno = genBackoffNumber(T);
			hosts[sourceHost].frameQueue.push_back(currentFrame);
			currentTime = currentFrame->time;
		}

		// Step 3: Check for hosts that need to send an ACK packet
		for(int i = 0; i < numhosts; i++){
			Frame *peekNextFrame = NULL;

			if(!frameOrder.empty())
				peekNextFrame = frameOrder.back();

			if(hosts[i].receivedFrame != NULL){
				Frame *received = hosts[i].receivedFrame;
				if(peekNextFrame != NULL){
					if(received->time < peekNextFrame->time){
						Frame *newACKFrame = new Frame(destinationHost, sourceHost, 64, received->time + SIFS);
						hosts[sourceHost].ackFrame = newACKFrame; 
						hosts[i].receivedFrame = NULL;
						globalFrameQ.pop_back();
						currentTime = newACKFrame->time;
						throughput += received->length;
						break;
					}
				}
				else{
					Frame *newACKFrame = new Frame(destinationHost, sourceHost, 64, received->time + SIFS);
					hosts[sourceHost].ackFrame = newACKFrame; 
					currentTime = newACKFrame->time;
					throughput += received->length;
					break;
				}
			}
		}

		//Step 4: Deal with backoff numbers 
		int hostWithLowestNum = -1;
		int lowestNum = 50000;
		// Step 4a: find host with smallest backoff number 
		for(int i = 0; i < numhosts; i++){		

			if(hosts[i].backoffno != 0){ // has a backoff number
				if(lowestNum > hosts[i].backoffno){
					lowestNum = hosts[i].backoffno;
					hostWithLowestNum = i;
				}
			}
		}

			// Step 4b: decrement numbers
		Frame *waitingFrame;
		Frame *peekNextFrame = NULL;
		double freeTime = lowestNum * 0.00001;

		if(hostWithLowestNum != -1){
			waitingFrame = hosts[hostWithLowestNum].frameQueue.back();
		}
		else {// No backoff numbers to decrement
			continue;
		}

		if(!frameOrder.empty()){
			peekNextFrame = frameOrder.back();
			if(peekNextFrame->time < timeChannelFree + freeTime){
				freeTime = peekNextFrame->time - timeChannelFree;
			}
		}

		for(int i = 0; i < numhosts; i++)
			for(double t = 0; t < freeTime; t += 0.00001)
				if(hosts[i].backoffno != 0)
					hosts[i].backoffno--;

		if(freeTime == lowestNum * 0.00001) { // Put backed off frame in channel at front
			waitingFrame->time = timeChannelFree + freeTime;
			frameOrder.push_back(waitingFrame);
			globalFrameQ.pop_back();
		}
	
		//limit++;
	}
	printf("Throughput: %f bytes/s \n", throughput / currentTime);
	printf("Total collisions: %d\n", failed);
}
