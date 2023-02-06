#pragma once

// Includes
#include "Node.h"

class NodesComparator
{
public:
	// Constructors & Destructors
	NodesComparator();
	~NodesComparator();

	// Methods
	bool operator() (Node* node1, Node* node2);
};