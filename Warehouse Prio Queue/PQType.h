#ifndef PQTYPE_H
#define PQTYPE_H

#include<iostream>
#include<cmath>
using namespace std;

enum RushStatus { STANDARD = 1, EXPEDITE, EXTREME };//the higher value, the higher priority

struct Shipment {
	int idNum;//identification number of the order or delivery, should not change after creation
	int initialWidgets;//# of widgets when order came in, only used in orders, should not change after creation
	int currentWidgets;//# of widgets left in delivery or # of widgets still need to be shipped in order
	double widgetPrice;//cost per widget, only used in deliveries, should not change after creation
	int priority;
	bool operator>(const Shipment& s1) {
		if (priority != s1.priority)
			return (priority > s1.priority);
		return (idNum < s1.idNum);//lower idNum was added earlier (higher priority)
	}
};

typedef Shipment NodeType;

class PQType {//implemented as max heap array
private:
	NodeType* heap;
	int maxNodes;//max nodes the heap can hold
	int numNodes;//current nodes in the heap
public:
	PQType();
	PQType(int maxLevels);
	PQType(const PQType& pq);
	~PQType();

	bool isEmpty();
	bool isFull();
	NodeType top();

	PQType& operator=(const PQType& pq);
	PQType& copyFrom(const PQType& pq);
	NodeType* allocHeap();
	void swapNodes(int index1, int index2);
	int getHigher(int index1, int index2);
	void reheapUp(int index);
	void reheapDown(int index);
	void Enq(NodeType newNode);
	NodeType Deq();
};

#endif //PQTYPE_H