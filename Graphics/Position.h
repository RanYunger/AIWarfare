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
	bool operator == (const Position& other) { return (row == other.row) && (column == other.column); }
	bool operator != (const Position& other) { return (row != other.row) || (column != other.column); }
	bool operator | (const Position& other)
	{
		double rowDiff = fabs(row - other.row), columnDiff = fabs(column - other.column);

		return rowDiff + columnDiff == 1;
	}
};