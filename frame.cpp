#include "frame.h"


Frame::Frame(int srchost, int desthost, int nlength, double ntime, char ntype) {
  		this->srcHost = srchost;
  		this->destHost = desthost;
  		this->length = nlength;
  		this->time = ntime;
  		this->type = ntype;
}

bool operator < (const Frame &f1, const Frame &f2){
	return f1.getTime() < f2.getTime();
}