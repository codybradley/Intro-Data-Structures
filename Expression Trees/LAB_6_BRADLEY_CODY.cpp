//LAB 6 BRADLEY, CODY TH
//main.cpp

#include"ExTree.h"
#include"Stack.h"
#include<iostream>
#include<string>
#include<fstream>
using namespace std;

void die(string msg);
void clearInput();
int inputValidInt(int minInt, int maxInt, string msg);
void readInData(string exArray[], int& els);
void printExpressions(string exArray[], int els);
string convertInToPost(string inS);
int getPrec(char op);

const int MAXEXPRESSIONS = 100;

int main() {
	string exArray[MAXEXPRESSIONS];
	int els = 0;
	int userChoice = -1;
	readInData(exArray, els);

	do{
		printExpressions(exArray, els);
		userChoice = inputValidInt(0, els,
			"\nEnter the number of an expression to create a tree or "
			"0 to exit the program");
		if (userChoice == 0)//end program
			cout << "\nExiting program\n" << endl;
		else {//create tree from expression numbered by userChoice (index userChoice-1)
			string postfix = convertInToPost(exArray[userChoice - 1]);
			ExTree et(postfix);
			do{
				userChoice = inputValidInt(0, 5,
					"\nEnter one of the following options:\n"
					"0. Exit program\n"
					"1. Choose another expression\n"
					"2. Print preorder traversal of the expression tree\n"
					"3. Print inorder traversal of the expression tree\n"
					"4. Print postorder traversal of the expression tree");
				switch (userChoice) {
				case 0://exit program
					cout << "\nExiting program\n" << endl;
					break;
				case 1://choose another expression
					break;
				case 2://preorder
					cout << "\nPreorder:\n";
					et.Preorder(et.root);
					cout << endl;
					break;
				case 3://inorder
					cout << "\nInorder:\n";
					et.Inorder(et.root);
					cout << endl;
					break;
				case 4://postorder
					cout << "\nPostorder:\n";
					et.Postorder(et.root);
					cout << endl;
					break;
				default:
					clearInput();
					cout << "Invalid choice\n" << endl;
				}
			} while (userChoice != 0 && userChoice != 1);//exits loop when choice is 0 or 1
		}
	} while (userChoice != 0);//exits loop when choice is 0 in either menu
	return 0;
}

//Fnc: outputs message and exits program
//Pre: none
//Post: message has been output and program has stopped execution
void die(string msg) {
	cout << msg << "\nExiting Program" << endl;
	exit(1);
}

//Fnc: clears cin error flags and clears the input buffer
//Pre: none
//Post: input buffer is cleared
void clearInput() {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//Fnc: outputs msg and takes input from user until it is an integer from minInt to maxInt
//Pre: none
//Post: returns an integer between minInt and maxInt (inclusive)
int inputValidInt(int minInt, int maxInt, string msg) {
	int userInt = maxInt + 1;
	if (minInt > maxInt)
		die("In inputValidInt(): minInt cannot be greater than maxInt");
	do {
		cout << msg << endl;
		cin >> userInt;
		if (cin.fail() || userInt < minInt || userInt > maxInt) {
			clearInput();
			cout << "Invalid input" << endl;
			userInt = maxInt + 1;
		}
	} while (userInt<minInt || userInt>maxInt);
	return userInt;
}

//Fnc: inputs expressions from the input file into exArray
//Pre: input file exists and has a new line at the end of the file
//Post: exArray has expressions in it and els contains the total number of expressions
void readInData(string exArray[], int& els) {
	ifstream inFile;
	string tempEx;
	inFile.open("D:\\Intro Data Structure\\Lab 6\\Lab_6_Input.txt");
	if (!(inFile.is_open()))
		die("Input file not found");
	inFile >> tempEx;
	for (els = 0; !inFile.eof() && els < MAXEXPRESSIONS; els++) {
		exArray[els] = tempEx;
		inFile >> tempEx;
	}
	if (!inFile.eof())
		cout << "Too many expressions in the file, first " << els << " read in\n" << endl;
	else
		cout << els << " expressions read in\n" << endl;
	inFile.close();
}

//Fnc: outputs the expressions in exArray and their associated expression number
//Pre: all of the elements of exArray up to index els-1 have values
//Post: all expressions of exArray have been output
void printExpressions(string exArray[], int els) {
	cout << "\n";
	for (int i = 0; i < els; i++)
		cout << i + 1 << ". " << exArray[i] << endl;
}

//Fnc: converts an expression that is infix to postfix
//Pre: inS is a valid infix expression
//Post: returns the postfix expression that is equivalent to the infix expression passed
string convertInToPost(string infix) {
	Stack opStack;
	string postfix;
	for (int i = 0; infix[i] != '\0'; i++) {//loops through each character in string
		etNode currentOp;//holds the operand or operator in infix[i]
		currentOp.op = infix[i];
		if ((infix[i] >= 'A' && infix[i] <= 'Z') || (infix[i] >= 'a' && infix[i] <= 'z'))
			postfix += infix[i];//infix[i] is an operand so add it on to the end of postfix
		else if (infix[i] == '(')//infix[i] is open parens so push it on stack
			opStack.Push(currentOp);
		else if (infix[i] == ')') {//infix[i] is close parens so pop stack and add to postfix until open parens
			while (!(opStack.IsEmpty()) && opStack.Peek().op != '(') {
				//while stack is not empty and top of stack is not open parens
				postfix += opStack.Peek().op;
				opStack.Pop();
			}
			if (opStack.IsEmpty()) {//stopped popping because empty stack, invalid expression
				cout << "The expression " << infix << " has too many close parens before open parens\nExiting program" << endl;
				exit(1);
			}
			else//stopped popping because open parens on stack
				opStack.Pop();//pop open parens
		}
		else {//infix[i] is an operator or an invalid character
			if (getPrec(infix[i]) == -1) {//invalid character
				cout << "The expression " << infix << " contains an invalid character or symbol: " << infix[i]
					<< "\nExiting program" << endl;
				exit(1);
			}
			while (!(opStack.IsEmpty()) && getPrec(infix[i]) <= getPrec(opStack.Peek().op)) {
				//pop and add to postfix everything with equal or higher precedence than current operator
				postfix += opStack.Peek().op;
				opStack.Pop();
			}
			opStack.Push(currentOp);//current operator has higher precedence than top of stack (or stack is empty)
		}
	}//endfor
	while (!(opStack.IsEmpty())) {//pop and add the rest of the operators on the stack to postfix
		postfix += opStack.Peek().op;
		opStack.Pop();
	}
	return postfix;
}

//Fnc: helper function to find the relative precidence of an operator (higher # = higher precedence)
//Pre: none
//Post: returns 1, 2, or 3 depending on the operator, or returns -1 if not a valid operator
int getPrec(char op) {
	int prec = -1;//not a valid operator
	if (op == '+' || op == '-')
		prec = 1;//lowest precedence
	else if (op == '*' || op == '/' || op == '%')
		prec = 2;
	else if (op == '^')
		prec = 3;//highest precedence 
	return prec;
}