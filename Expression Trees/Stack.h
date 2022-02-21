//LAB 6 BRADLEY, CODY TH
//Stack.h
#ifndef STACK_H
#define STACK_H

#include<iostream>
using namespace std;

#ifndef ETNODE
#define ETNODE
struct etNode {
	char op;
	etNode* left;//used for tree
	etNode* right;//used for tree
	etNode* next;//used for stack
};
#endif //ETNODE

typedef etNode NodeType;

class Stack {
public:
	NodeType* top;

	Stack();
	Stack(const Stack& q1);
	~Stack();

	Stack& operator=(const Stack& s1);

	bool IsEmpty();
	int GetLength();
	NodeType Peek();

	void CopyFrom(const Stack& s1);
	void Push(NodeType newNode);
	void Pop();
	void MakeEmpty();
};

#endif //STACK_H