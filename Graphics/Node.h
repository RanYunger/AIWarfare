#pragma once

// Includes
#include "Position.h"

class Node
{
private:
	// Fields
	Position sourcePosition, destinationPosition;
	Node* parent;
	double f, g, h;

public:
	// Properties
	Position GetSourcePosition();
	void SetSourcePosition(Position* sp);

	Position GetDestinationPosition();
	void SetDestinationPosition(Position* dp);

	Node* GetParent();
	void SetParent(Node* p);

	double GetF();
	void SetF(double fValue);

	double GetG();
	void SetG(double gValue);

	double GetH();
	void SetH(double hValue);

	// Constructors & Destructors
	Node();
	Node(Position* sp, Position* dp, Node* p, double g);
	~Node();

	// Methods
	bool operator == (const Node& other) { return sourcePosition == other.sourcePosition; }
};