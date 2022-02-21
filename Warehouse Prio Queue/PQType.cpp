#include "PQType.h"

//Fnc: dynamically allocates an array of 63 nodes
//Pre: none
//Post: the priority queue has been initialized
PQType::PQType() {
	maxNodes = 63;//default max of 6 layers
	numNodes = 0;
	heap = allocHeap();
}

//Fnc: dynamically allocates an array of 2^maxLevels-1 nodes
//Pre: maxLevels>0
//Post: the priority queue has been initialized
PQType::PQType(int maxLevels) {
	maxNodes = pow(2, maxLevels) - 1;
	numNodes = 0;
	heap = allocHeap();
}

//Fnc: initializes the calling priority queue as a deep copy of pq
//Pre: pq is correctly constructed
//Post: the calling priority queue is a deep copy of pq
PQType::PQType(const PQType& pq) {
	maxNodes = pq.maxNodes;
	numNodes = pq.numNodes;
	heap = allocHeap();
	copyFrom(pq);
}

//Fnc: frees the memory used by the pointer heap
//Pre: none
//Post: the memory used by the priority queue has been freed
PQType::~PQType() {
	if (heap != nullptr)
		delete[] heap;
}

//Fnc: checks if the queue is empty
//Pre: none
//Post: returns true if there are zero nodes in the queue
//      otherwise returns false
bool PQType::isEmpty() {
	return (numNodes == 0);
}

//Fnc: checks if the queue is full
//Pre: none
//Post: returns true if the nodes in the array equals the max number of nodes
//      otherwise returns false
bool PQType::isFull() {
	return (numNodes == maxNodes);
}

//Fnc: gets the data in the top of the heap (front of priority queue)
//Pre: the queue is correctly constructed
//Post: if queue is empty, outputs error message and exits program
//      otherwise returns the data in the top node of the heap (front of queue)
NodeType PQType::top() {
	if (isEmpty()) {
		cout << "\nTried peeking an empty queue\nExiting program" << endl;
		exit(1);
	}
	return heap[0];
}

//Fnc: does a deep copy from pq to the calling priority queue
//Pre: pq is correctly constructed
//Post: the calling priority queue is a deep copy of pq
PQType& PQType::operator=(const PQType& pq) {
	delete[] heap;
	copyFrom(pq);
	return *this;
}

//Fnc: does a deep copy from pq to the calling priority queue
//Pre: pq is correctly constructed
//Post: the calling priority queue is a deep copy of pq
PQType& PQType::copyFrom(const PQType& pq) {
	maxNodes = pq.maxNodes;
	heap = allocHeap();
	for (int index = 0; index < numNodes; index++)
		heap[index] = pq.heap[index];
	return *this;
}

//Fnc: helper function used to allocate memory for the heap array
//Pre: maxNodes is positive
//Post: returns a pointer to an array with number of nodes equal to maxNodes
NodeType* PQType::allocHeap() {
	NodeType* tempPtr = new (nothrow) NodeType[maxNodes];
	if (tempPtr == nullptr) {
		cout << "\nFailed to allocate memory for the heap\nExiting program" << endl;
		exit(1);
	}
	return tempPtr;
}

//Fnc: swaps the data in index1 and index2 of the heap
//Pre: index1 and index2 are the indexes of nodes with valid data in the heap array
//Post: index1 and index2 of the heap have their data swapped
void PQType::swapNodes(int index1, int index2) {
	NodeType tempNode = heap[index1];
	heap[index1] = heap[index2];
	heap[index2] = tempNode;
}

//Fnc: figures out which index contains data with the higher priority
//Pre: both indexes contain valid data
//Post: returns the index with higher priority data
int PQType::getHigher(int index1, int index2) {
	if (heap[index1] > heap[index2])
		return index1;
	else
		return index2;

}

//Fnc: given the index of an out of place node, swaps it
//     with its parent until it is in the correct place (max heap)
//Pre: the rest of the heap is correctly constructed aside from the target node,
//     '>' operator is defined for NodeType
//Post: the heap is correctly constructed including the target node
void PQType::reheapUp(int index) {
	int parentIndex;
	if (index > 0) {//index>0 means current node is not the root node
		if (index & 1)//index is odd (left child)
			parentIndex = (index - 1) / 2;
		else//index is even (right child)
			parentIndex = (index - 2) / 2;
		if (heap[index] > heap[parentIndex]) {
			swapNodes(index, parentIndex);
			reheapUp(parentIndex);
		}
	}
	//else current node is root node, cannot reheap up anymore
}

//Fnc: given the index of an out of place node, swaps it
//     with its children until in the correct place (max heap)
//Pre: the rest of the heap is correctly constructed aside from the target node,
//     '>' operator is defined for NodeType
//Post: the heap is correctly constructed including the target node
void PQType::reheapDown(int index) {
	int leftIndex, rightIndex, highPriority;
	leftIndex = index * 2 + 1;
	rightIndex = index * 2 + 2;
	if (rightIndex < numNodes) {//left and right children exist
		highPriority = getHigher(getHigher(index, leftIndex), rightIndex);
		if (highPriority != index) { //highest of the 3 is one of the children
			swapNodes(index, highPriority);
			reheapDown(highPriority);
		}
	}
	else if (leftIndex < numNodes)//only left child exists
		if (heap[leftIndex] > heap[index]) {//left child is larger than current node
			swapNodes(index, leftIndex);
			reheapDown(leftIndex);
		}
	//else no children (done reheaping down)
}

//Fnc: puts newNode into the queue in the proper place
//Pre: the calling priority queue is correctly constructed
//Post: newNode is in the proper place in the priority queue
void PQType::Enq(NodeType newNode) {
	if (!isFull()) {
		numNodes += 1;
		heap[numNodes - 1] = newNode;
		reheapUp(numNodes - 1);
	}
	else {
		cout << "\nTried enqueueing a full queue\nExiting program" << endl;
		exit(1);
	}
}

//Fnc: removes the top node in the heap (front of priority queue)
//     and gets a copy of its data
//Pre: the queue is correctly constructed
//Post: the data in the top of the heap is returned,
//      the top node is removed,
//      the max heap's properties are maintained
NodeType PQType::Deq() {
	NodeType tempNode;
	if (!isEmpty()) {
		numNodes -= 1;
		tempNode = heap[0];
		heap[0] = heap[numNodes];
		reheapDown(0);
	}
	else {
		cout << "\nTried dequeueing an empty queue\nExiting program" << endl;
		exit(1);
	}
	return tempNode;
}