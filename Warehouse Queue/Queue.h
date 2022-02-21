//LAB 5 BRADLEY, CODY TH
//Queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include<iostream>
using namespace std;

struct Shipment {
	int idNum;//identification number of the order or delivery, should not change after creation
	int initialWidgets;//# of widgets when order came in, only used in orders, should not change after creation
	int currentWidgets;//# of widgets left in delivery or # of widgets still need to be shipped in order
	double widgetPrice;//cost per widget, only used in deliveries, should not change after creation
	Shipment* next;//should not be directly changed by main
};

typedef Shipment NodeType;

class Queue {
public:
	NodeType* front;
	NodeType* rear;//last node should always point to nullptr

	Queue();
	Queue(const Queue& q1);
	~Queue();

	Queue& operator=(const Queue& q1);

	bool IsEmpty();
	int GetLength();
	NodeType PeekFront();

	void CopyFrom(const Queue& q1);
	void Enq(NodeType newNode);
	NodeType Deq();
	void MakeEmpty();
};

#endif //QUEUE_H