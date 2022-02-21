//LAB 6 BRADLEY, CODY TH
//Stack.cpp
#include "Stack.h"

//Fnc: sets top to be nullptr
//Pre: none
//Post: top is equal to nullptr
Stack::Stack() {
	top = nullptr;
}

//Fnc: does a deep copy from the stack passed to the calling stack
//Pre: bottom node on the stack passed points to nullptr
//Post: stack that called the function is a deep copy of the stack passed
Stack::Stack(const Stack& s1) {
	top = nullptr;
	CopyFrom(s1);
}

//Fnc: pops all nodes on the stack
//Pre: bottom node on the stack points to nullptr
//Post: there are no nodes on the stack, top points to nullptr
Stack::~Stack() {
	MakeEmpty();
}

//Fnc: does a deep copy from the stack passed to the calling stack
//Pre: bottom node on the stack passed points to nullptr
//Post: stack that called the function is a deep copy of the stack passed
Stack& Stack::operator=(const Stack& s1) {
	CopyFrom(s1);
	return *this;
}

//Fnc: checks if the stack is empty
//Pre: none
//Post: returns 1 if stack is empty, 0 if stack is not empty
bool Stack::IsEmpty() {
	return (top == nullptr);
}

//Fnc: gets the length of the stack (how many nodes)
//Pre: none
//Post: returns the number of nodes in the stack
int Stack::GetLength() {
	int count = 0;
	NodeType* tempPtr = top;
	while (tempPtr != nullptr) {
		tempPtr = tempPtr->next;
		++count;
	}
	return count;
}

//Fnc: get the data of the top node without popping
//Pre: stack must have at least one node
//Post: returns a copy of the data of the top node
NodeType Stack::Peek() {
	if (IsEmpty()) {
		cout << "Tried peeking an empty stack\nExiting Program\n" << endl;
		exit(1);
	}
	return *top;
}

//Fnc: does a deep copy from the stack passed to the calling stack
//Pre: bottom node in the stack passed points to nullptr
//Post: stack that called the function is a deep copy of the stack passed
void Stack::CopyFrom(const Stack& s1) {
	if (this != &s1) {//if they are the same exact object of class stack, this fnc should do nothing
		MakeEmpty();
		Stack s2;
		NodeType* current = s1.top;
		while (current != nullptr) {//deep copies s1 into s2 in reverse order
			NodeType tempNode = *current;//data that current points to is copied into tempNode
			s2.Push(tempNode);//bottom node of s2 will point to nullptr
			current = current->next;//points current to the next node, or nullptr if it was the last node
		}//after processing last node, current points to nullptr
		//s2 is reverse of s1
		current = s2.top;
		while (current != nullptr) {//deep copies s2 into calling stack in reverse order
			NodeType tempNode = *current;
			Push(tempNode);
			current = current->next;
		}
		s2.MakeEmpty();
		//calling stack has the same data in the same order as s1
	}
}

//Fnc: puts a node with the same data as newNode on the top of the stack
//Pre: none
//Post: node with the same data as newNode is at the top of the stack
void Stack::Push(NodeType newNode) {
	NodeType* ptrNewNode = new (nothrow) NodeType;
	if (ptrNewNode == nullptr) {
		cout << "Failed to allocate memory in Push()\nExiting program\n" << endl;
		exit(1);
	}
	*ptrNewNode = newNode;//copy data of newNode into ptrNewNode
	NodeType* tempPtr = top;//pointer to previous top of stack (or nullptr if stack was empty)
	top = ptrNewNode;//top now points to new node
	top->next = tempPtr;//new node points to previous top (or nullptr if stack was empty)
}

//Fnc: removes top node from stack, or if stack is empty, exits program
//Pre: bottom node points to nullptr
//Post: memory used by top node has been freed, top now points to next node
//      if stack is empty, error message is output and program stops execution
void Stack::Pop() {
	if (IsEmpty()) {
		cout << "\nTried to pop an empty stack\nExiting program" << endl;
		exit(1);
	}
	NodeType* tempPtr = top;//point something to node being deleted, so top can be moved
	top = top->next;//point top to next node, or it will point to nullptr if it was the last node
	delete tempPtr;
	tempPtr = nullptr;
}

//Fnc: pops all nodes on the stack
//Pre: bottom node on the stack points to nullptr
//Post: top points to nullptr, there are no nodes on the stack
void Stack::MakeEmpty() {
	while (!IsEmpty())
		Pop();
}