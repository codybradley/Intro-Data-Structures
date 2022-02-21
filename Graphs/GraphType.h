#ifndef GRAPHTYPE_H
#define GRAPHTYPE_H

#include "QueType.h"
#include<iostream>

template<class VertexType>
// Assumption: VertexType is a type for which the "=", 
// "==", and "<<" operators are defined
class GraphType
{
public:
	GraphType();                // Default of 50 vertices
	GraphType(int maxV);          // maxV <= 50
	~GraphType();
//  void MakeEmpty();
//  bool IsEmpty() const;
//  bool IsFull() const;
	void AddVertex(VertexType vertex);
	void AddEdge(VertexType fromVertex, VertexType toVertex, int weight);
	int WeightIs(VertexType fromVertex, VertexType toVertex);
	void GetToVertices(VertexType, QueType<VertexType>&);
//  void ClearMarks();
//  void MarkVertex(VertexType);
//  bool IsMarked(VertexType);
	void DeleteEdge(VertexType fromVertex, VertexType toVertex);
	bool EdgeExists(VertexType vertex1, VertexType vertex2);
	void PrintEdges(int);
private:
	int numVertices;
	int maxVertices;
	VertexType* vertices;
	int edges[50][50];
	bool* marks;	// marks[i] is mark for vertices[i].
};

#endif //GRAPHTYPE_H