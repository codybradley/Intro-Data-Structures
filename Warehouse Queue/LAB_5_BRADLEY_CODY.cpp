//LAB 5 BRADLEY, CODY TH
//main.cpp

#include"Queue.h"
#include<iostream>
#include<string>
#include<iomanip>
using namespace std;

enum Choice { DELIVERY = 1, ORDER, INV, ORDERQUE, END };


void die(string msg);
void clearInput();
int inputValidInt(int minInt, int maxInt, string msg);
double inputValidDoub(double minDouble, double maxDouble, string msg);

void getNewDelivery(Queue& qD, int& totalDeliveries, int& totalWidgetsHeld);
void getNewOrder(Queue& qS, int& totalOrders);

void processOrders(Queue& qD, Queue& qO, int& totalWidgetsShipped, int& totalWidgetsHeld);
void processOrdersGT(Queue& qD, Shipment& tempD, Shipment& tempO, int& widgetsShipped, double& totalCostWarehouse);
void processOrdersLE(Queue& qD, Shipment& tempD, Shipment& tempO, int& widgetsShipped, double& totalCostWarehouse);
void printBreakdown(int qtyShipped, double unitPrice);
void printShipmentStats(Shipment tempO, int widgetsShipped, double totalCostWarehouse, double totalCostCustomer);

void putFront(Queue& qS, Shipment frontShip);
void printQueue(const Queue& qS, Choice shipmentType);

int main() {
	cout << right << fixed << setprecision(2);
	
	int userChoice = 0;
	Queue qD;//delivery queue
	Queue qO;//order queue
	int totalDeliveries = 0;//total deliveries that have existed, to ensure different delivery #'s
	int totalOrders = 0;//total orders that have existed, to ensure different order #'s
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
			getNewDelivery(qD, totalDeliveries, totalWidgetsHeld);
			processOrders(qD, qO, totalWidgetsShipped, totalWidgetsHeld);
			break;
		case ORDER:
			getNewOrder(qO, totalOrders);
			processOrders(qD, qO, totalWidgetsShipped, totalWidgetsHeld);
			break;
		case INV:
			if (!(qD.IsEmpty())) {
				printQueue(qD, INV);
				cout << "\n\tTotal widgets in inventory: " << totalWidgetsHeld << endl;
			}
			else
				cout << "\n\tThe inventory is empty" << endl;
			break;
		case ORDERQUE:
			if (!(qO.IsEmpty()))
				printQueue(qO, ORDERQUE);
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

//Fnc: tells user the error and exits program
//Pre: none
//Post: program ends with termination status EXIT_FAILURE
void die(string msg) {
	cout << "Error: " << msg << "\nExiting Program." << endl;
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
		}
	} while (userInt<minInt || userInt>maxInt);
	return userInt;
}

//Fnc: outputs msg and takes input from user until it is a double from minDoub to maxDoub
//Pre: none
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

//Fnc: adds a new delivery to the rear of the queue
//Pre: none
//Post: There is one more delivery in the queue, totalWidgets is updated
void getNewDelivery(Queue& qD, int& totalDeliveries, int& totalWidgetsHeld) {
	Shipment tempD;
	++totalDeliveries;
	tempD.idNum = totalDeliveries;
	tempD.currentWidgets = inputValidInt(1, 10000, "How many widgets in the delivery? (1-10000)");
	tempD.widgetPrice = inputValidDoub(0.01, 10000.00, "How much per widget? (0.01-10000.00)");
	qD.Enq(tempD);
	totalWidgetsHeld += tempD.currentWidgets;
}

//Fnc: adds a new order to the rear of the queue
//Pre: none
//Post: There is one more order in the queue
void getNewOrder(Queue& qS, int& totalOrders) {
	Shipment tempO;
	++totalOrders;
	tempO.idNum = totalOrders;
	tempO.initialWidgets = inputValidInt(1, 10000, "How many widgets would you like to order? (1-10000)");
	tempO.currentWidgets = tempO.initialWidgets;
	qS.Enq(tempO);
}

//Fnc: processes orders until either no deliveries left or no outstanding orders left
//Pre: # of widgets is positive in both queues being passed
//Post: totalWidgetsShipped and totalWidgetsHeld are updated, Either one or neither of the queues has nodes in it
void processOrders(Queue& qD, Queue& qO, int& totalWidgetsShipped, int& totalWidgetsHeld) {
	while (!(qD.IsEmpty() || qO.IsEmpty())) {//processes one order after another until one or both queues are empty
		Shipment tempD = qD.PeekFront();
		Shipment tempO = qO.PeekFront();
		double totalCostWarehouse = 0, totalCostCustomer = 0;
		int widgetsShipped = 0;
		cout << "\n" << setw(15) << "Qty Shipped" << setw(15) << "Unit Price" << setw(25)
			<< "Cost to the Warehouse" << setw(25) << "Cost to the Customer" << endl;
		while (tempD.currentWidgets < tempO.currentWidgets && !qD.IsEmpty()) {
			//exits loop when delivery widgets >= order widgets, or run out of widgets from deliveries
			processOrdersGT(qD, tempD, tempO, widgetsShipped, totalCostWarehouse);
		}
		if (tempD.currentWidgets >= tempO.currentWidgets) {
			processOrdersLE(qD, tempD, tempO, widgetsShipped, totalCostWarehouse);
		}
		totalWidgetsHeld -= widgetsShipped;
		totalWidgetsShipped += widgetsShipped;
		totalCostCustomer = 1.5 * totalCostWarehouse;

		printShipmentStats(tempO, widgetsShipped, totalCostWarehouse, totalCostCustomer);

		qO.Deq();//updating order queue after processing an order
		if (tempO.currentWidgets > 0)//if order was partially processed, put adjusted order at front
			putFront(qO, tempO);
		if (tempD.currentWidgets > 0)//if delivery was partially used, put adjusted delivery at front
			putFront(qD, tempD);
	}
}

//Fnc: (helper function) process order that has # widgets left that is greater than the current delivery
//Pre: neither queue is empty
//Post: totalCostWarehouse and widgetsShipped are updated, as well as current widgets for tempD and tempO
void processOrdersGT(Queue& qD, Shipment& tempD, Shipment& tempO, int& widgetsShipped, double& totalCostWarehouse) {
	totalCostWarehouse += (tempD.currentWidgets) * (tempD.widgetPrice);
	widgetsShipped += tempD.currentWidgets;
	printBreakdown(tempD.currentWidgets, tempD.widgetPrice);
	tempO.currentWidgets -= tempD.currentWidgets;
	qD.Deq();
	if (!(qD.IsEmpty()))
		tempD = qD.PeekFront();
	else//Delivery stack is empty
		tempD.currentWidgets = 0;//won't go thru other if statement, or put an empty delivery at front of queue
}

//Fnc: (helper function) processes orders that have # widgets left that is less than or equal to the current delivery
//Pre: neither queue is empty
//Post: totalCostWarehouse and widgetsShipped are updated, as well as current widgets for tempD and tempO
void processOrdersLE(Queue& qD, Shipment& tempD, Shipment& tempO, int& widgetsShipped, double& totalCostWarehouse) {
	totalCostWarehouse += (tempO.currentWidgets) * (tempD.widgetPrice);
	widgetsShipped += tempO.currentWidgets;
	printBreakdown(tempO.currentWidgets, tempD.widgetPrice);
	qD.Deq();
	if (tempD.currentWidgets == tempO.currentWidgets)
		tempD.currentWidgets = 0;
	else//tempD.currentWidges > tempO.currentWidgets
		tempD.currentWidgets -= tempO.currentWidgets;
	tempO.currentWidgets = 0;
}

//Fnc: outputs qtyShipped, unitPrice, and calculated costs to warehouse and customer
//Pre: none
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

//Fnc: outputs statistics of the entire shipment
//Pre: none
//Post: the statistics of the shipment are output on the screen
void printShipmentStats(Shipment tempO, int widgetsShipped, double totalCostWarehouse, double totalCostCustomer) {
	cout << "\n\t" << left << setw(30) << "Order Number" << right << setw(8) << tempO.idNum
		<< "\n\t" << left << setw(30) << "Qty Ordered" << right << setw(8) << tempO.initialWidgets
		<< "\n\t" << left << setw(30) << "Qty Shipped this Shipment" << right << setw(8) << widgetsShipped
		<< "\n\t" << left << setw(30) << "Qty to be Shipped" << right << setw(8) << tempO.currentWidgets
		<< "\n\t" << left << setw(30) << "Total Cost to the Warehouse" << right << setw(8) << totalCostWarehouse
		<< "\n\t" << left << setw(30) << "Total Cost to the Customer" << right << setw(8) << totalCostCustomer
		<< "\n\t" << left << setw(30) << "Profit this Shipment" << right << setw(8) << totalCostCustomer - totalCostWarehouse
		<< endl;
}

//Fnc: helper function to put a partially filled delivery or order
//     at the front of a queue without changing the rest of the order
//Pre: the queue is properly constructed
//Post: the partially filled delivery or order is at the front of the queue
void putFront(Queue& qS, Shipment frontShip) {
	Queue tempQ = qS;//copy the queue over to a temp queue
	Shipment tempS;
	qS.MakeEmpty();//clear the queue
	qS.Enq(frontShip);//put the node at the front of the queue (first node in)
	while (!(tempQ.IsEmpty())) {
		tempS = tempQ.Deq();//dequeue the temp queue and store the data
		qS.Enq(tempS);//enqueue that data in the queue
	}
}

//Fnc: if INV, outputs id number, number of widgets, and price per widget for each delivery in the queue
//     if ORDERQUE (or anything else), outputs id number, initial number of widgets, and current number of widgets for each order in the queue
//Pre: if INV, every node has values for id number, current widgets, and price per widget
//     if ORDERQUE (or anything else), every node has values for id number, initial widgets, current widgets
//Post: the deliveries or orders in the queue have been output (in order from front to rear)
void printQueue(const Queue& qS, Choice shipmentType) {
	Shipment tempS;
	Queue tempQ = qS;//makes sure not to touch the original queue
	if(shipmentType==INV)
		cout << "\n" << setw(20) << "Delivery Number" << setw(20) << "Number of Widgets" << setw(20) << "Price per Widget" << endl;
	else//shipmentType==ORDERQUE (or just not INV)
		cout << "\n" << setw(15) << "Order Number" << setw(15) << "Qty Ordered" << setw(20) << "Qty to be Shipped" << endl;
	while (!(tempQ.IsEmpty())) {//continues looping as long as tempQ isn't empty
		tempS = tempQ.Deq();//removes front node and puts its data in tempD
		if(shipmentType==INV)
			cout << setw(20) << tempS.idNum << setw(20) << tempS.currentWidgets << setw(20) << tempS.widgetPrice << endl;
		else//shipmentType==ORDERQUE (or just not INV)
			cout << setw(15) << tempS.idNum << setw(15) << tempS.initialWidgets << setw(20) << tempS.currentWidgets << endl;
	}//by the end of the loop, all nodes have been dequeued and output
}