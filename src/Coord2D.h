#ifndef COORD2D_INCLUDED
#define COORD2D_INCLUDED

#include <iostream>

class Coord2D
{
private:
	int i;
	int j;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	Coord2D();
	Coord2D(int a, int b);
	~Coord2D();

	//SETTERS
	void setI(int a);
	void setJ(int b);

	//GETTERS
	int getI();
	int getJ();

	//OTHERS
	void print();

};

#endif