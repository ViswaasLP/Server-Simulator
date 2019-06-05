#include "frame.h"


Frame::Frame(int srchost, int desthost, int nlength, double ntime, char ntype) {
  		this->srcHost = srchost;
  		this->destHost = desthost;
  		this->length = nlength;
  		this->time = ntime;
  		this->type = ntype;
}

bool compareFrameTimes(Frame *a, Frame *b) { 
	return a->time < b->time; 
}