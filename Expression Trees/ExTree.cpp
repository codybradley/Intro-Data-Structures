//LAB 6 BRADLEY, CODY TH
//ExTree.cpp
#include "ExTree.h"
#include "Stack.h"

//Fnc: sets root to nullptr
//Pre: none
//Post: root is nullptr
ExTree::ExTree() {
	root = nullptr;
}

//Fnc: creates an expression tree based on a postfix expression
//Pre: the postfix expression is valid and contains only valid characters
//Post: expression tree has been created based on the postfix expression given
ExTree::ExTree(string postfix) {
	ConvertPostToTree(postfix);
}

//Fnc: frees memory used by the tree
//Pre: root points to a correctly constructed tree or nullptr
//Post: the memory used by the tree has been freed
ExTree::~ExTree() {
	MakeEmpty(root);
}

//Fnc: checks if the tree is empty
//Pre: root points to the correct place
//Post: returns 1 if the tree is empty, 0 if it is not
bool ExTree::IsEmpty() {
	return root == nullptr;
}

//Fnc: traverses and prints the tree in preorder
//Pre: ptrNode points to a correctly constructed tree
//Post: the tree has been output in preorder
void ExTree::Preorder(NodeType* ptrNode) {
	if (ptrNode != nullptr) {
		cout << ptrNode->op << " ";
		Preorder(ptrNode->left);
		Preorder(ptrNode->right);
	}
}

//Fnc: traverses and prints the tree in inorder
//Pre: ptrNode points to a correctly constructed tree
//Post: the tree has been output in inorder with parens around each subtree
void ExTree::Inorder(NodeType* ptrNode) {
	if (ptrNode != nullptr) {
		cout << "(";
		Inorder(ptrNode->left);
		cout << ptrNode->op;
		Inorder(ptrNode->right);
		cout << ")";
	}
}

//Fnc: traverses and prints the tree in postorder
//Pre: ptrNode points to a correctly constructed tree
//Post: the tree has been output in postorder
void ExTree::Postorder(NodeType* ptrNode) {
	if (ptrNode != nullptr) {
		Postorder(ptrNode->left);
		Postorder(ptrNode->right);
		cout << ptrNode->op << " ";
	}
}

//Fnc: creates an expression tree based on a postfix expression
//Pre: the postfix expression is valid and contains only valid characters
//     the tree calling this function is empty
//Post: expression tree has been created based on the postfix expression given
void ExTree::ConvertPostToTree(string postfix) {
	Stack treeStack;//contains the root nodes of subtrees that will make up the final tree
	NodeType et;//operator that will have etl and etr as children
	NodeType *etl, *etr;//left and right subtrees to be added to et
	for (int i = 0; postfix[i] != '\0'; i++) {
		if ((postfix[i] >= 'A' && postfix[i] <= 'Z') || (postfix[i] >= 'a' && postfix[i] <= 'z')) {
			//current character is an operand, push it onto stack as node of tree with no left or right children
			et.op = postfix[i];
			et.left = nullptr;
			et.right = nullptr;
			treeStack.Push(et);
		}
		else {//current character is an operator, make the top two subtrees of the stack left and right 
			et.op = postfix[i];//create new subtree with the root node being the current operator

			etr = AllocNode();
			*etr = treeStack.Peek();
			et.right = etr;//right points to the subtree on the top of the stack
			treeStack.Pop();

			etl = AllocNode();
			*etl = treeStack.Peek();
			et.left = etl;//left points to the next subtree on the stack
			treeStack.Pop();

			treeStack.Push(et);//push the new subtree onto the stack
		}
	}//if the expression is valid, only one tree should remain on the stack
	et = treeStack.Peek();
	treeStack.Pop();
	root = AllocNode();
	*root = et;
}

//Fnc: helper function to dynamically allocate memory for an expression tree node
//Pre: none
//Post: returns a pointer to the allocated memory
NodeType* ExTree::AllocNode() {
	NodeType* tempPtr = new (nothrow) NodeType;
	if (tempPtr == nullptr) {
		cout << "Failed to allocate memory in ConvertPostToTree()\nExiting program" << endl;
		exit(1);
	}
	return tempPtr;
}

//Fnc: frees memory used by the tree pointed to by ptrNode
//Pre: root points to a correctly constructed tree or nullptr
//Post: the memory used by the tree has been freed
void ExTree::MakeEmpty(NodeType*& ptrNode) {
	if (ptrNode != nullptr) {//delete in postorder
		MakeEmpty(ptrNode->left);
		MakeEmpty(ptrNode->right);
		delete ptrNode;
		ptrNode = nullptr;
	}
}