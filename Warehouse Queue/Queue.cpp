//LAB 5 BRADLEY, CODY TH
//Queue.cpp
#include "Queue.h"

//Fnc: sets front and rear to be nullptr
//Pre: none
//Post: front and rear are equal to nullptr
Queue::Queue() {
	front = nullptr;
	rear = nullptr;
}

//Fnc: does a deep copy from the queue passed to the calling queue
//Pre: last node in the queue passed points to nullptr
//Post: queue that called the function is a deep copy of the queue passed
Queue::Queue(const Queue& q1) {
	front = nullptr;
	rear = nullptr;
	CopyFrom(q1);
}

//Fnc: dequeues all nodes in the queue
//Pre: last node in the queue points to nullptr
//Post: there are no nodes in the queue, front and rear point to nullptr
Queue::~Queue() {
	MakeEmpty();
}

//Fnc: does a deep copy from the queue passed to the calling queue
//Pre: last node in the queue passed points to nullptr
//Post: queue that called the function is a deep copy of the queue passed
Queue& Queue::operator=(const Queue& q1) {
	CopyFrom(q1);
	return *this;
}

//Fnc: checks if the queue is empty
//Pre: none
//Post: returns 1 if queue is empty, 0 if queue is not empty
bool Queue::IsEmpty() {
	return (front == nullptr);
}

//Fnc: gets the length of the queue (how many nodes)
//Pre: none
//Post: returns the number of nodes in the queue
int Queue::GetLength() {
	int count = 1;
	NodeType* tempPtr = front;
	if (!IsEmpty())//queue has at least 1 element
		while (tempPtr != rear) {
			tempPtr = tempPtr->next;
			++count;
		}
	else//queue is empty
		count = 0;
	return count;
}

//Fnc: get the data of the front node without dequeueing
//Pre: queue must have at least one node
//Post: returns a copy of the data of the front node
NodeType Queue::PeekFront() {
	if (IsEmpty()) {
		cout << "Tried peeking an empty stack\nExiting Program\n" << endl;
		exit(1);
	}
	return *front;
}

//Fnc: does a deep copy from the queue passed to the calling queue
//Pre: last node in the queue passed points to nullptr, queue is empty
//Post: queue that called the function is a deep copy of the queue passed
void Queue::CopyFrom(const Queue& q1) {
	if (this != &q1) {//if they are the same exact object of class queue, this fnc should do nothing
		MakeEmpty();
		NodeType* current = q1.front;
		while (current != nullptr) {
			NodeType tempNode = *current;//data that current points to is copied to where tempPtr points to
			Enq(tempNode);//the data field 'next' is set to nullptr in Enq(), so it is no longer connected to q1
			current = current->next;//points current to the next node, or nullptr if it was the last node
		}//after processing last node, current points to nullptr
	}
}

//Fnc: puts a node with the same data as newNode at the end of the queue
//Pre: none
//Post: node with the same data as newNode is at the end of the queue
void Queue::Enq(NodeType newNode) {
	NodeType* ptrNewNode = new (nothrow) NodeType;
	if (ptrNewNode == nullptr) {
		cout << "Failed to allocate memory in Enq()\nExiting program\n" << endl;
		exit(1);
	}
	*ptrNewNode = newNode;
	if (IsEmpty()) {//0 nodes in queue
		front = ptrNewNode;//both front and rear must point to the node
		rear = ptrNewNode;
	}
	else {//there's at least 1 node in the queue
		rear->next = ptrNewNode;//last node points to new node
		rear = ptrNewNode;//rear points to new node
	}
	ptrNewNode->next = nullptr;
}

//Fnc: removes the front node and returns a copy of its data
//     or exits program if queue is empty (main() should check IsEmpty())
//Pre: last node in the queue points to nullptr
//Post: front node has been 'deleted' and a copy of its data has been returned,
//      if queue is empty, error message is output and program stops execution
NodeType Queue::Deq() {
	if (IsEmpty()) {
		cout << "\nTried to dequeue an empty queue. Exiting program" << endl;
		exit(1);
	}
	NodeType* tempPtr = front;//point something to node being deleted, so front can be moved
	NodeType tempData = *front;//copy data from front node into tempData
	front = front->next;//point front to next node
	if (front == nullptr)//node being dequeued is the only node
		rear = nullptr;
	delete tempPtr;
	tempPtr = nullptr;
	return tempData;
}

//Fnc: dequeues all nodes in the queue
//Pre: last node in the queue points to nullptr
//Post: front and rear point to nullptr, there are no nodes in the queue
void Queue::MakeEmpty() {
	while (!IsEmpty())
		Deq();
}