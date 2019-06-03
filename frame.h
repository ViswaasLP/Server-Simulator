#ifndef FRAME_H
#define FRAME_H


class Frame {
  public:
    int srcHost;
    int destHost;
    int length;
    double time;
    char type;
  	Frame(int srchost, int desthost, int nlength, double ntime, char ntype);
  	double getTime() const {return time;}
};

#endif

bool operator<(const Frame &f1, const Frame &f2);