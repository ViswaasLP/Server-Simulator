#include "queue.h"

Queue::Queue(){
	size = 0;
	head = nullptr;
	tail = nullptr;
}

bool Queue::isCapped(int cap){
	if (size == cap){
		return true;
	}

	return false;
}

void Queue::enqueue(Packet *p){
	Node *n;
	n = new Node;
	n->item = p;

	if (size == 0){// first element
		head = n;
		tail = head;
	}
	else { // not first element
		n->prev = tail;
		tail->next = n;
		tail = tail->next;
	}

	size++;
}

Packet* Queue::dequeue(){
	Node *n;
	n = head;

	head = head->next;

	if (size == 1) {
		tail = nullptr;
	}

	size--;
	return n->item;
}