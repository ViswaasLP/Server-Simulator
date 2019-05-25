#ifndef _QUEUE_H
#define _QUEUE_H
#include "Packet.h"
#include "Node.h"

class Queue {
	int size;
	Node *head;
	Node *tail;

public:
	Queue();
	int getSize() { return size; }
	bool isCapped(int cap);
	void enqueue(Packet *p);
	Packet* dequeue();
};

#endif 