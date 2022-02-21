//LAB 4 BRADLEY, CODY TH

#include<iostream>
#include<iomanip>
#include<string>
using namespace std;

struct Delivery {
	int deliveryNum;
	int currentWidgets;//# of widgets 
	double widgetPrice;//cost per widget (should not change)
};

struct Order {
	int orderNum;//identification number of the order (should not change)
	int initialWidgets;//# of widgets when order came in (should not change)
	int currentWidgets;//# of widgets still need to be shipped
};

void die(string msg);
void clearInput();
int inputValidInt(int minInt, int maxInt, string msg);
double inputValidDoub(double minDouble, double maxDouble, string msg);
void getNewDelivery(Delivery stackD[], int& topD, int& totalDeliveries, int& totalWidgetsHeld);
void getNewOrder(Order stackO[], int& topO, int& totalOrders);
void processOrders(Delivery stackD[], int& topD, Order stackO[], int& topO, int& totalWidgetsShipped, int& totalWidgetsHeld);
void processOrdersGT(Delivery stackD[], int& topD, Delivery& tempD, Order& tempO, int& widgetsShipped, double& totalCostWarehouse);
void processOrdersLE(Delivery stackD[], int& topD, Delivery& tempD, Order& tempO, int& widgetsShipped, double& totalCostWarehouse);
void printBreakdown(int qtyShipped, double unitPrice);
void printShipmentStats(Order tempO, int widgetsShipped, double totalCostWarehouse, double totalCostCustomer);

bool isFull(int top);
bool isEmpty(int top);
Delivery peekD(Delivery stackD[], int top);
Order peekO(Order stackO[], int top);
void pushStackD(Delivery stackD[], int& top, Delivery newD);
void pushStackO(Order stackO[], int& top, Order newO);
void popStack(int& top);
void makeStackEmpty(int& top);
void printStackD(Delivery stackD[], int& top);
void printStackO(Order stackO[], int& top);

const int MAXSTACK = 100;//max # of deliveries or orders

int main() {
	cout << right << fixed << setprecision(2);

	enum Choice {DELIVERY = 1, ORDER, INV, ORDERQUE, END};
	int userChoice = 0;
	Delivery stackD[MAXSTACK];
	Order stackO[MAXSTACK];
	int topD = -1, topO = -1;//topD and topO are the top indexes of stackD and stackO respectively
	int totalOrders = 0;//total orders that have existed, to ensure different order #'s
	int totalDeliveries = 0;//to ensure different delivery #'s
	int totalWidgetsShipped = 0, totalWidgetsHeld = 0;

	while (userChoice != END) {
		userChoice = inputValidInt(1, END,//1 will always be first choice, END will always be last choice
			"\nEnter one of the following options:\n"
			"1. Get a new delivery\n"
			"2. Get a new order\n"
			"3. View inventory on hand\n"
			"4. View outstanding orders\n"
			"5. End the program");
		switch (userChoice) {
		case DELIVERY:
			getNewDelivery(stackD, topD, totalDeliveries, totalWidgetsHeld);
			processOrders(stackD, topD, stackO, topO, totalWidgetsShipped, totalWidgetsHeld);
			break;
		case ORDER:
			getNewOrder(stackO, topO, totalOrders);
			processOrders(stackD, topD, stackO, topO, totalWidgetsShipped, totalWidgetsHeld);
			break;
		case INV:
			if (!isEmpty(topD)) {
				printStackD(stackD, topD);
				cout << "\n\tTotal widgets in inventory: " << totalWidgetsHeld << endl;
			}
			else
				cout << "\n\tThe inventory is empty" << endl;
			break;
		case ORDERQUE:
			if (!isEmpty(topO))
				printStackO(stackO, topO);
			else
				cout << "\n\tThere are no outstanding orders" << endl;
			cout << "\n\tTotal widgets shipped: " << totalWidgetsShipped << endl;
			break;
		case END:
			cout << "Ending Program" << endl;
			break;
		default:
			clearInput();
			cout << "Invalid choice" << endl;
		}
	}
}

//Pre: none
//Fnc: tells user the error and exits program
//Post: program ends with termination status EXIT_FAILURE
void die(string msg) {
	cout << "Error: " << msg << "\nExiting Program." << endl;
	exit(1);
}

//Pre: none
//Fnc: clears cin error flags and clears the input buffer
//Post: input buffer is cleared
void clearInput() {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//Pre: none
//Fnc: outputs msg and takes input from user until it is an integer from minInt to maxInt
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
		}
	} while (userInt<minInt || userInt>maxInt);
	return userInt;
}

//Pre: none
//Fnc: outputs msg and takes input from user until it is a double from minDoub to maxDoub
//Post: returns a double between minDoub and maxDoub (inclusive)
double inputValidDoub(double minDoub, double maxDoub, string msg) {
	double userDoub = maxDoub + 1.0;
	if (minDoub > maxDoub)
		die("In inputValidDoub(): minDoub cannot be greater than maxDoub");
	do {
		cout << msg << endl;
		cin >> userDoub;
		if (cin.fail() || userDoub<minDoub || userDoub>maxDoub) {
			clearInput();
			cout << "Invalid input" << endl;
		}
	} while (userDoub<minDoub || userDoub>maxDoub);
	return userDoub;
}

//Pre: topD is the index of the top of stackD
//Fnc: adds a new delivery to stackD, does nothing if stack is full
//Post: There is one more element on stackD than before (or none if it's full)
//      totalWidgets is updated
void getNewDelivery(Delivery stackD[], int& topD, int& totalDeliveries, int& totalWidgetsHeld) {
	if (!isFull(topD)) {
		Delivery tempD;
		++totalDeliveries;
		tempD.deliveryNum = totalDeliveries;
		tempD.currentWidgets = inputValidInt(1, 10000, "How many widgets in the delivery? (1-10000)");
		tempD.widgetPrice = inputValidDoub(0.01, 10000.00, "How much per widget? (0.01-10000.00)");
		pushStackD(stackD, topD, tempD);
		totalWidgetsHeld += (peekD(stackD, topD)).currentWidgets;
	}
	else
		cout << "\nStack for deliveries is full, cannot accept a delivery" << endl;
}

//Pre: topO is the index of the top of stackO
//Fnc: adds a new order to stackO, does nothing if stack is full
//Post: There is one more element on stackO than before (or none if it's full)
void getNewOrder(Order stackO[], int& topO, int& totalOrders) {
	if (!isFull(topO)) {
		Order tempO;
		++totalOrders;
		tempO.orderNum = totalOrders;
		tempO.initialWidgets = inputValidInt(1, 10000, "How many widgets would you like to order? (1-10000)");
		tempO.currentWidgets = tempO.initialWidgets;
		pushStackO(stackO, topO, tempO);
	}
	else
		cout << "\nStack for orders is full, cannot accept an order" << endl;
}

//Pre: # of widgets is positive in both stacks being passed,
//		topD and topO are the indexes of the top of stackD and stackO, respectively
//Fnc: processes orders until either no deliveries left or no outstanding orders left
//Post: totalWidgetsShipped and totalWidgetsHeld are updated, Either one or neither of the stacks has elements in it
void processOrders(Delivery stackD[], int& topD, Order stackO[], int& topO, int& totalWidgetsShipped, int& totalWidgetsHeld) {
	while (!(isEmpty(topD) || isEmpty(topO))) {//processes one order after another until one or both stacks are empty
		Delivery tempD = peekD(stackD, topD);
		Order tempO = peekO(stackO, topO);
		double totalCostWarehouse = 0, totalCostCustomer = 0;
		int widgetsShipped = 0;
		cout << "\n" << setw(15) << "Qty Shipped" << setw(15) << "Unit Price" << setw(25)
			<< "Cost to the Warehouse" << setw(25) << "Cost to the Customer" << endl;
		while (tempD.currentWidgets < tempO.currentWidgets && !isEmpty(topD)) {
			//exits loop when delivery widgets >= order widgets, or run out of widgets from deliveries
			processOrdersGT(stackD, topD, tempD, tempO, widgetsShipped, totalCostWarehouse);
		}
		if (tempD.currentWidgets >= tempO.currentWidgets) {
			processOrdersLE(stackD, topD, tempD, tempO, widgetsShipped, totalCostWarehouse);
		}
		totalWidgetsHeld -= widgetsShipped;
		totalWidgetsShipped += widgetsShipped;
		totalCostCustomer = 1.5 * totalCostWarehouse;

		printShipmentStats(tempO, widgetsShipped, totalCostWarehouse, totalCostCustomer);

		popStack(topO);//updating order stack after processing an order
		if (tempO.currentWidgets > 0)//if order was partially processed, push adjusted order
			pushStackO(stackO, topO, tempO);
		if (tempD.currentWidgets > 0)//if delivery was partially used, push adjusted delivery
			pushStackD(stackD, topD, tempD);
	}
}

//Pre: neither stack is empty, topD is the index of the top of stackD
//Fnc: process order that has # widgets left that is greater than the current delivery
//Post: totalCostWarehouse and widgetsShipped are updated, as well as tempD and tempO widget counts
void processOrdersGT(Delivery stackD[], int& topD, Delivery& tempD, Order& tempO, int& widgetsShipped, double& totalCostWarehouse) {
	totalCostWarehouse += (tempD.currentWidgets) * (tempD.widgetPrice);
	widgetsShipped += tempD.currentWidgets;
	printBreakdown(tempD.currentWidgets, tempD.widgetPrice);
	tempO.currentWidgets -= tempD.currentWidgets;
	popStack(topD);
	if (!isEmpty(topD))
		tempD = peekD(stackD, topD);
	else//Delivery stack is empty
		tempD.currentWidgets = 0;//won't go thru other if statement, or push an empty delivery on stack
}

//Pre: neither stack is empty, topD is the index of the top of stackD
//Fnc: processes orders that have # widgets left that is less than or equal to the current delivery
//Post: totalCostWarehouse and widgetsShipped are updated, as well as tempD and tempO widget counts
void processOrdersLE(Delivery stackD[], int& topD, Delivery& tempD, Order& tempO, int& widgetsShipped, double& totalCostWarehouse) {
	totalCostWarehouse += (tempO.currentWidgets) * (tempD.widgetPrice);
	widgetsShipped += tempO.currentWidgets;
	printBreakdown(tempO.currentWidgets, tempD.widgetPrice);
	popStack(topD);
	if (tempD.currentWidgets == tempO.currentWidgets)
		tempD.currentWidgets = 0;
	else//tempD.currentWidges > tempO.currentWidgets
		tempD.currentWidgets -= tempO.currentWidgets;
	tempO.currentWidgets = 0;
}

//Pre: none
//Fnc: outputs qtyShipped, unitPrice, and calculated costs to warehouse and customer
//Post: qtyShipped, unitPrice, and calculated costs to warehouse and customer have been output
void printBreakdown(int qtyShipped, double unitPrice) {
	/*heading for the table created by calling printBreakdown multiple times:
	cout << "\n" << setw(15) << "Qty Shipped" << setw(15) << "Unit Price" << setw(25)
			<< "Cost to the Warehouse" << setw(25) << "Cost to the Customer" << endl;*/
	double costWarehouse = qtyShipped * unitPrice;
	double costCustomer = qtyShipped * unitPrice * 1.5;
	cout << setw(15) << qtyShipped << setw(15) << unitPrice << setw(25)
		<< costWarehouse << setw(25) << costCustomer << endl;
}

//Pre: none
//Fnc: outputs statistics of the entire shipment
//Post: the statistics of the shipment are output on the screen
void printShipmentStats(Order tempO, int widgetsShipped, double totalCostWarehouse, double totalCostCustomer) {
	cout << "\n\t" << left << setw(30) << "Order Number" << right << setw(8) << tempO.orderNum
		<< "\n\t" << left << setw(30) << "Qty Ordered" << right << setw(8) << tempO.initialWidgets
		<< "\n\t" << left << setw(30) << "Qty Shipped this Shipment" << right << setw(8) << widgetsShipped
		<< "\n\t" << left << setw(30) << "Qty to be Shipped" << right << setw(8) << tempO.currentWidgets
		<< "\n\t" << left << setw(30) << "Total Cost to the Warehouse" << right << setw(8) << totalCostWarehouse
		<< "\n\t" << left << setw(30) << "Total Cost to the Customer" << right << setw(8) << totalCostCustomer
		<< "\n\t" << left << setw(30) << "Profit this Shipment" << right << setw(8) << totalCostCustomer - totalCostWarehouse
		<< endl;
}

//Pre: 'top' is the index of the top of a stack that exists
//Fnc: checks if the stack is full
//Post: returns 0 if stack is not full, 1 if stack is full
bool isFull(int top) {
	return (top == MAXSTACK-1);
}

//Pre: 'top' is the index of the top of a stack that exists
//Fnc: checks if the stack is empty
//Post: returns 0 if stack is not empty, 1 if stack is empty
bool isEmpty(int top) {
	return (top == -1);
}

//Pre: 'top' is the index of the top of stackD
//Fnc: peeks the top element of the stack if stack is not empty, ends program if stack is empty
//Post: returns top element of the stack of deliveries
Delivery peekD(Delivery stackD[], int top) {
	if (isEmpty(top))
		die("Tried peeking an empty stack");
	return stackD[top];
}

//Pre: 'top' is the index of the top of stackO
//Fnc: peeks the top element of the stack if stack is not empty, ends program if stack is empty
//Post: returns top element of the stack of orders
Order peekO(Order stackO[], int top) {
	if (isEmpty(top))
		die("Tried peeking an empty stack");
	return stackO[top];
}

//Pre: 'top' is the index of the top of stackD
//Fnc: pushes new delivery on to the stack if stack is not full, ends program if stack is full
//Post: new delivery is the new top of the stack
void pushStackD(Delivery stackD[], int& top, Delivery newD) {
	if (isFull(top))
		die("Tried pushing a full stack");
	++top;
	stackD[top] = newD;
}

//Pre: 'top' is the index of the top of stackO
//Fnc: pushes new order on to the stack if stack is not full, ends program if stack is full
//Post: new order is the new top of the stack
void pushStackO(Order stackO[], int& top, Order newO) {
	if (isFull(top))
		die("Tried pushing a full stack");
	++top;
	stackO[top] = newO;
}

//Pre: 'top' is the index of the top of a stack that exists
//Fnc: pops top element off the stack if stack is not empty, ends program if stack is empty
//Post: top element has been popped off the stack
void popStack(int& top) {
	if (isEmpty(top))
		die("Tried popping an empty stack");
	--top;
}

//Pre: 'top' is the index of the top of a stack that exists
//Fnc: pops all elements off the stack
//Post: stack is empty
void makeStackEmpty(int& top) {
	while (!isEmpty(top))
		popStack(top);
}

//Pre: 'top' is the index of the top of stackD
//Fnc: outputs number of widgets and price per widget for each delivery on the stack, from top to bottom
//Post: stackD is the same as when the function was called
void printStackD(Delivery stackD[], int& top) {
	Delivery tempD;
	Delivery tempStackD[MAXSTACK];
	int tempTop = -1;
	cout << "\n" << setw(20) << "Delivery Number" << setw(20) << "Number of Widgets" << setw(20) << "Price per Widget" << endl;
	while (!isEmpty(top)) {
		tempD = peekD(stackD, top);
		cout << setw(20) << tempD.deliveryNum << setw(20) << tempD.currentWidgets << setw(20) << tempD.widgetPrice << endl;
		pushStackD(tempStackD, tempTop, tempD);
		popStack(top);
	}//stackD is empty, tempStackD has reverse of what stackD had
	while (!isEmpty(tempTop)) {
		tempD = peekD(tempStackD, tempTop);
		pushStackD(stackD, top, tempD);
		popStack(tempTop);
	}//stackD is back to what it was originally
}

//Pre: 'top' is the index of the top of stackO
//Fnc: outputs order #, Qty ordered, and Qty to be shipped for each order on the stack, from top to bottom
//Post: stackO is the same as when the function was called
void printStackO(Order stackO[], int& top) {
	Order tempO;
	Order tempStackO[MAXSTACK];
	int tempTop = -1;
	cout << "\n" << setw(15) << "Order Number" << setw(15) << "Qty Ordered" << setw(20) << "Qty to be Shipped" <<endl;
	while (!isEmpty(top)) {
		tempO = peekO(stackO, top);
		cout << setw(15) << tempO.orderNum << setw(15) << tempO.initialWidgets << setw(20) << tempO.currentWidgets << endl;
		pushStackO(tempStackO, tempTop, tempO);
		popStack(top);
	}//stackO is empty, tempStackO has reverse of what stackO had
	while (!isEmpty(tempTop)) {
		tempO = peekO(tempStackO, tempTop);
		pushStackO(stackO, top, tempO);
		popStack(tempTop);
	}//stackO is back to what it was originally
}

/*1 5 2.50 2 2 2 6 2 12 2 2 1 8 1.00 1 12 2.00 1 5 1.5 2 9 1 1 3.50 1 2 2.50 2 2*/
/*1 5 2.50 | 2 2 | 2 6 | 2 12 | 2 2 | 1 8 1.00 | 1 12 2.00 | 1 5 1.5 | 2 9 | 1 1 3.50 | 1 2 2.50 | 2 2*/

/*Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
1
How many widgets in the delivery? (1-10000)
5
How much per widget? (0.01-10000.00)
2.5

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
2
How many widgets would you like to order? (1-10000)
2

    Qty Shipped     Unit Price    Cost to the Warehouse     Cost to the Customer
              2           2.50                     5.00                     7.50

        Order Number                         1
        Qty Ordered                          2
        Qty Shipped this Shipment            2
        Qty to be Shipped                    0
        Total Cost to the Warehouse       5.00
        Total Cost to the Customer        7.50
        Profit this Shipment              2.50

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
3

     Delivery Number   Number of Widgets    Price per Widget
                   1                   3                2.50

        Total widgets in inventory: 3

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
4

        There are no outstanding orders

        Total widgets shipped: 2

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
2
How many widgets would you like to order? (1-10000)
6

    Qty Shipped     Unit Price    Cost to the Warehouse     Cost to the Customer
              3           2.50                     7.50                    11.25

        Order Number                         2
        Qty Ordered                          6
        Qty Shipped this Shipment            3
        Qty to be Shipped                    3
        Total Cost to the Warehouse       7.50
        Total Cost to the Customer       11.25
        Profit this Shipment              3.75

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
2
How many widgets would you like to order? (1-10000)
12

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
2
How many widgets would you like to order? (1-10000)
2

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
3

        The inventory is empty

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
4

   Order Number    Qty Ordered   Qty to be Shipped
              4              2                   2
              3             12                  12
              2              6                   3

        Total widgets shipped: 5

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
1
How many widgets in the delivery? (1-10000)
8
How much per widget? (0.01-10000.00)
1

    Qty Shipped     Unit Price    Cost to the Warehouse     Cost to the Customer
              2           1.00                     2.00                     3.00

        Order Number                         4
        Qty Ordered                          2
        Qty Shipped this Shipment            2
        Qty to be Shipped                    0
        Total Cost to the Warehouse       2.00
        Total Cost to the Customer        3.00
        Profit this Shipment              1.00

    Qty Shipped     Unit Price    Cost to the Warehouse     Cost to the Customer
              6           1.00                     6.00                     9.00

        Order Number                         3
        Qty Ordered                         12
        Qty Shipped this Shipment            6
        Qty to be Shipped                    6
        Total Cost to the Warehouse       6.00
        Total Cost to the Customer        9.00
        Profit this Shipment              3.00

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
1
How many widgets in the delivery? (1-10000)
12
How much per widget? (0.01-10000.00)
2

    Qty Shipped     Unit Price    Cost to the Warehouse     Cost to the Customer
              6           2.00                    12.00                    18.00

        Order Number                         3
        Qty Ordered                         12
        Qty Shipped this Shipment            6
        Qty to be Shipped                    0
        Total Cost to the Warehouse      12.00
        Total Cost to the Customer       18.00
        Profit this Shipment              6.00

    Qty Shipped     Unit Price    Cost to the Warehouse     Cost to the Customer
              3           2.00                     6.00                     9.00

        Order Number                         2
        Qty Ordered                          6
        Qty Shipped this Shipment            3
        Qty to be Shipped                    0
        Total Cost to the Warehouse       6.00
        Total Cost to the Customer        9.00
        Profit this Shipment              3.00

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
3

     Delivery Number   Number of Widgets    Price per Widget
                   3                   3                2.00

        Total widgets in inventory: 3

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
4

        There are no outstanding orders

        Total widgets shipped: 22

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
1
How many widgets in the delivery? (1-10000)
5
How much per widget? (0.01-10000.00)
1.5

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
2
How many widgets would you like to order? (1-10000)
9

    Qty Shipped     Unit Price    Cost to the Warehouse     Cost to the Customer
              5           1.50                     7.50                    11.25
              3           2.00                     6.00                     9.00

        Order Number                         5
        Qty Ordered                          9
        Qty Shipped this Shipment            8
        Qty to be Shipped                    1
        Total Cost to the Warehouse      13.50
        Total Cost to the Customer       20.25
        Profit this Shipment              6.75

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
3

        The inventory is empty

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
4

   Order Number    Qty Ordered   Qty to be Shipped
              5              9                   1

        Total widgets shipped: 30

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
1
How many widgets in the delivery? (1-10000)
1
How much per widget? (0.01-10000.00)
3.5

    Qty Shipped     Unit Price    Cost to the Warehouse     Cost to the Customer
              1           3.50                     3.50                     5.25

        Order Number                         5
        Qty Ordered                          9
        Qty Shipped this Shipment            1
        Qty to be Shipped                    0
        Total Cost to the Warehouse       3.50
        Total Cost to the Customer        5.25
        Profit this Shipment              1.75

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
3

        The inventory is empty

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
4

        There are no outstanding orders

        Total widgets shipped: 31

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
1
How many widgets in the delivery? (1-10000)
2
How much per widget? (0.01-10000.00)
2.5

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
2
How many widgets would you like to order? (1-10000)
2

    Qty Shipped     Unit Price    Cost to the Warehouse     Cost to the Customer
              2           2.50                     5.00                     7.50

        Order Number                         6
        Qty Ordered                          2
        Qty Shipped this Shipment            2
        Qty to be Shipped                    0
        Total Cost to the Warehouse       5.00
        Total Cost to the Customer        7.50
        Profit this Shipment              2.50

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
3

        The inventory is empty

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
4

        There are no outstanding orders

        Total widgets shipped: 33

Enter one of the following options:
1. Get a new delivery
2. Get a new order
3. View inventory on hand
4. View outstanding orders
5. End the program
5
Ending Program*/