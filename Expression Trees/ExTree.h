//LAB 6 BRADLEY, CODY TH
//ExTree.h
#ifndef EXTREE_H
#define EXTREE_H

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

class ExTree {
public:
	NodeType* root;

	ExTree();
	ExTree(string postfix);
	~ExTree();

	bool IsEmpty();
	void Preorder(NodeType* ptrNode);
	void Inorder(NodeType* ptrNode);
	void Postorder(NodeType* ptrNode);

	void ConvertPostToTree(string postfix);
	NodeType* AllocNode();
	void MakeEmpty(NodeType*& ptrNode);
};

#endif //EXTREE_H