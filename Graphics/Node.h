#pragma once

// Includes
#include "Position.h"

class Node
{
private:
	// Fields
	Position location, destination;
	Node* parent;
	double f, g, h;

public:
	// Properties
	Position GetLocation();
	void SetLocation(Position* s);

	Position GetDestination();
	void SetDestination(Position* d);

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
	Node(Position* s, Position* d, Node* p, double g);
	~Node();

	// Methods

	// Operators
	bool operator == (const Node& other) { return location == other.location; }
	bool operator != (const Node& other) { return location != other.location; }
};