#include "GraphType.h"
#include<iostream>
#include<exception>
#include<iomanip>
using namespace std;

template<class VertexType>
GraphType<VertexType>::GraphType()
// Post: Arrays of size 50 are dynamically allocated for  
//       marks and vertices.  numVertices is set to 0; 
//       maxVertices is set to 50.
{
  numVertices = 0;
  maxVertices = 50;
  vertices = new VertexType[50];
  marks = new bool[50];
}

template<class VertexType>
GraphType<VertexType>::GraphType(int maxV)
// Post: Arrays of size maxV are dynamically allocated for  
//       marks and vertices.  
//       numVertices is set to 0; maxVertices is set to maxV.
{
  numVertices = 0;
  maxVertices = maxV;
  vertices = new VertexType[maxV];
  marks = new bool[maxV];
}

template<class VertexType>
// Post: arrays for vertices and marks have been deallocated.
GraphType<VertexType>::~GraphType()
{
  delete [] vertices;
  delete [] marks;
}
const int NULL_EDGE = 0;

template<class VertexType>
void GraphType<VertexType>::AddVertex(VertexType vertex)
// Post: vertex has been stored in vertices.
//       Corresponding row and column of edges has been set 
//       to NULL_EDGE.
//       numVertices has been incremented.
{
  vertices[numVertices] = vertex;

  for (int index = 0; index < numVertices; index++)
  {
    edges[numVertices][index] = NULL_EDGE;
    edges[index][numVertices] = NULL_EDGE;
  }
  numVertices++;
}

template<class VertexType>
int IndexIs(VertexType* vertices, VertexType vertex)
// Post: Returns the index of vertex in vertices.
{
  int index = 0;

  while (!(vertex == vertices[index]))
    index++;  
  return index;
}       

template<class VertexType>
void GraphType<VertexType>::AddEdge(VertexType fromVertex,
     VertexType toVertex, int weight)
// Post: Edge (fromVertex, toVertex) is stored in edges.
{
  int row;
  int col;

  row = IndexIs(vertices, fromVertex);
  col = IndexIs(vertices, toVertex);
  edges[row][col] = weight;
}

template<class VertexType>
int GraphType<VertexType>::WeightIs
     (VertexType fromVertex, VertexType toVertex)
// Post: Returns the weight associated with the edge 
//       (fromVertex, toVertex).
{
  int row;
  int col;

  row = IndexIs(vertices, fromVertex);
  col = IndexIs(vertices, toVertex);
  return edges[row][col];
}

template<class VertexType>
void GraphType<VertexType>::GetToVertices(VertexType vertex, 
     QueType<VertexType>& adjVertices)
// Post: 
{
  int fromIndex;
  int toIndex;

  fromIndex = IndexIs(vertices, vertex);
  for (toIndex = 0; toIndex < numVertices; toIndex++)
    if (edges[fromIndex][toIndex] != NULL_EDGE)
      adjVertices.Enqueue(vertices[toIndex]);
}     

template<class VertexType>
void GraphType<VertexType>::DeleteEdge(VertexType fromVertex, VertexType toVertex)
//Function: "deletes a given edge"
//
//Post: The edge from fromVertex to toVertex is NULL_EDGE
{
	edges[IndexIs(vertices, fromVertex)][IndexIs(vertices, toVertex)] = NULL_EDGE;
}

template<class VertexType>
bool GraphType<VertexType>::EdgeExists(VertexType vertex1,
	VertexType vertex2)
//Function: "determines whether two vertices are connected by an edge"
//Pre: vertex1 and vertex2 are in the vertices array
//Post: Returns true if there is an edge from vertex1
//      to vertex2 or from vertex2 to vertex1.
//      (direction does not matter)
{
	int index1 = IndexIs(vertices, vertex1);
	int index2 = IndexIs(vertices, vertex2);
	return (!(edges[index1][index2] == NULL_EDGE &&
		edges[index2][index1] == NULL_EDGE));
}

template<class VertexType>
void GraphType<VertexType>::PrintEdges(int maxLength)
//Post: Outputs the table of edges with spacing based
//      on the max number of chars in any of the entries
//Note: If the most characters in any entry of the table
//      was 5, you pass 5 to the function
{
	maxLength += 2;
	if (numVertices > 0) {
		cout << endl;
		cout << right << setw(maxLength*2) << vertices[0];
		for (int index = 1; index < numVertices; index++)
			cout << setw(maxLength) << vertices[index];
		cout << "\n" << endl;
		for (int index = 0; index < numVertices; index++) {
			cout << left << setw(maxLength) << vertices[index] << right;
			for (int index2 = 0; index2 < numVertices; index2++)
				cout << setw(maxLength) << edges[index][index2];
			cout << "\n" << endl;
		}
	}
	else
		cout << "\nThere are no vertices." << endl;
}