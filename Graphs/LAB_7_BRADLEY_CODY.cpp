//LAB 7 BRADLEY, CODY TH
//main.cpp

#include "GraphType.cpp"
#include<iostream>
#include<string>

using namespace std;

template<class VertexType>
void printConnection(GraphType<VertexType>& graphObj, VertexType vertex1, VertexType vertex2);

int main() {
	string stateArr[10] = { "Alabama", "California",
		"Delaware", "Florida", "Georgia", "Hawaii",
		"Idaho", "Kansas", "Louisiana", "Maine" };

	GraphType<string> stateGraph;
	for (int index = 0; index < 10; index++) {
		stateGraph.AddVertex(stateArr[index]);
	}

	cout << "\nAdded 5 edges:" << endl;
	stateGraph.AddEdge("Hawaii", "Alabama", 4275);
	stateGraph.AddEdge("Florida", "Idaho", 2675);
	stateGraph.AddEdge("Idaho", "Kansas", 1200);
	stateGraph.AddEdge("Kansas", "Idaho", 1175);
	stateGraph.AddEdge("California", "California", 50);
	stateGraph.PrintEdges(10);//expecting max 10 char edge or vertex

	printConnection<string>(stateGraph, "Florida", "Idaho");
	printConnection<string>(stateGraph, "Idaho", "Florida");
	printConnection<string>(stateGraph, "Kansas", "California");
	printConnection<string>(stateGraph, "Kansas", "Idaho");
	printConnection<string>(stateGraph, "California", "California");

	cout << "\nDelete edge from Idaho to Kansas: " << endl;
	stateGraph.DeleteEdge("Idaho", "Kansas");
	stateGraph.PrintEdges(10);
	printConnection<string>(stateGraph, "Idaho", "Kansas");

	cout << "\nDelete edge from Kansas to Idaho: " << endl;
	stateGraph.DeleteEdge("Kansas", "Idaho");
	stateGraph.PrintEdges(10);
	printConnection<string>(stateGraph, "Idaho", "Kansas");
}

template<class VertexType>
void printConnection(GraphType<VertexType>& graphObj, VertexType vertex1, VertexType vertex2) {
	cout << "There is ";
	if (!(graphObj.EdgeExists(vertex1, vertex2)))
		cout << "not ";
	cout << "a connection between " << vertex1 << " and " << vertex2 << "." << endl;
}

/*Test Data:
Added 5 edges:

				 Alabama  California    Delaware     Florida     Georgia      Hawaii       Idaho      Kansas   Louisiana       Maine

Alabama                0           0           0           0           0           0           0           0           0           0

California             0          50           0           0           0           0           0           0           0           0

Delaware               0           0           0           0           0           0           0           0           0           0

Florida                0           0           0           0           0           0        2675           0           0           0

Georgia                0           0           0           0           0           0           0           0           0           0

Hawaii              4275           0           0           0           0           0           0           0           0           0

Idaho                  0           0           0           0           0           0           0        1200           0           0

Kansas                 0           0           0           0           0           0        1175           0           0           0

Louisiana              0           0           0           0           0           0           0           0           0           0

Maine                  0           0           0           0           0           0           0           0           0           0

There is a connection between Florida and Idaho.
There is a connection between Idaho and Florida.
There is not a connection between Kansas and California.
There is a connection between Kansas and Idaho.
There is a connection between California and California.

Delete edge from Idaho to Kansas:

				 Alabama  California    Delaware     Florida     Georgia      Hawaii       Idaho      Kansas   Louisiana       Maine

Alabama                0           0           0           0           0           0           0           0           0           0

California             0          50           0           0           0           0           0           0           0           0

Delaware               0           0           0           0           0           0           0           0           0           0

Florida                0           0           0           0           0           0        2675           0           0           0

Georgia                0           0           0           0           0           0           0           0           0           0

Hawaii              4275           0           0           0           0           0           0           0           0           0

Idaho                  0           0           0           0           0           0           0           0           0           0

Kansas                 0           0           0           0           0           0        1175           0           0           0

Louisiana              0           0           0           0           0           0           0           0           0           0

Maine                  0           0           0           0           0           0           0           0           0           0

There is a connection between Idaho and Kansas.

Delete edge from Kansas to Idaho:

				 Alabama  California    Delaware     Florida     Georgia      Hawaii       Idaho      Kansas   Louisiana       Maine

Alabama                0           0           0           0           0           0           0           0           0           0

California             0          50           0           0           0           0           0           0           0           0

Delaware               0           0           0           0           0           0           0           0           0           0

Florida                0           0           0           0           0           0        2675           0           0           0

Georgia                0           0           0           0           0           0           0           0           0           0

Hawaii              4275           0           0           0           0           0           0           0           0           0

Idaho                  0           0           0           0           0           0           0           0           0           0

Kansas                 0           0           0           0           0           0           0           0           0           0

Louisiana              0           0           0           0           0           0           0           0           0           0

Maine                  0           0           0           0           0           0           0           0           0           0

There is not a connection between Idaho and Kansas.*/