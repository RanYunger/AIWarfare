// Includes
#include "Node.h"

#include <math.h>
#include "Position.h"

// Properties
Position Node::GetLocation() { return location; }
void Node::SetLocation(Position* s) { location = *s; }

Position Node::GetDestination() { return destination; }
void Node::SetDestination(Position* d) { destination = *d; }

Node* Node::GetParent() { return parent; }
void Node::SetParent(Node* p) { parent = p; }

double Node::GetF() { return f; }
void Node::SetF(double fValue) { f = fValue; }

double Node::GetG() { return g; }
void Node::SetG(double gValue) { g = gValue; }

double Node::GetH() { return h; }
void Node::SetH(double hValue) { h = hValue; }

// Constructors & Destructors
Node::Node()
{
	SetG(0);
	SetParent(nullptr);
}
Node::Node(Position* s, Position* d, Node* p, double g)
{
	SetLocation(s);
	SetDestination(d);
	SetParent(p);
	SetG(g);
	SetH(fabs(s->GetRow() - d->GetRow()) + fabs(s->GetColumn() - d->GetColumn()));
	SetF(g + h);
}
Node::~Node() {}

// Methods