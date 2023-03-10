#pragma once

// Includes
#include <math.h>

class Position
{
private:
	// Fields
	double row, column;

public:
	// Properties
	double GetRow();
	void SetRow(double r);

	double GetColumn();
	void SetColumn(double c);

	// Constructors & Destructors
	Position();
	Position(double r, double c);
	~Position();

	// Methods

	// Operators
	bool operator == (const Position& other) { return ((int)row == (int)other.row) && ((int)column == (int)other.column); }
	bool operator != (const Position& other) { return ((int)row != (int)other.row) || ((int)column != (int)other.column); }
};