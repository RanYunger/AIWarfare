// Includes
#include "Node.h"

#include <math.h>
#include "Position.h"

// Properties
Position Node::GetSourcePosition() { return sourcePosition; }
void Node::SetSourcePosition(Position* sp) { sourcePosition = *sp; }

Position Node::GetDestinationPosition() { return destinationPosition; }
void Node::SetDestinationPosition(Position* dp) { destinationPosition = *dp; }

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
Node::Node(Position* sp, Position* dp, Node* p, double g)
{
	SetSourcePosition(sp);
	SetDestinationPosition(dp);
	SetParent(p);
	SetG(g);
	SetH(fabs(sp->GetRow() - dp->GetRow()) + fabs(sp->GetColumn() - dp->GetColumn()));
	SetF(g + h);
}
Node::~Node() {}

// Methods