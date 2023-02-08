// Includes
#include "Position.h"

// Properties
double Position::GetRow() { return row; }
void Position::SetRow(double r) { row = r; }

double Position::GetColumn() { return column; }
void Position::SetColumn(double c) { column = c; }

// Constructors & Destructors
Position::Position()
{
	SetRow(-1);
	SetColumn(-1);
}
Position::Position(double r, double c)
{
	SetRow(r);
	SetColumn(c);
}
Position::~Position() {}

// Methods