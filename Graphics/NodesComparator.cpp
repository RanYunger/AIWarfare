// Includes
#include "NodesComparator.h"

#include "Node.h"

// Constructors & Destructors
NodesComparator::NodesComparator() {}
NodesComparator::~NodesComparator() {}

// Methods
bool NodesComparator::operator() (Node* node1, Node* node2)
{
	return node1->GetF() > node2->GetF();
}