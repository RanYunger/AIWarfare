#pragma once

// Includes

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
};