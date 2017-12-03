#ifndef WO_GOAL_INCLUDED
#define WO_GOAL_INCLUDED

#include "WorldObject.h"
class WO_Goal :	public WorldObject
{
private:


public:
	//CONSTRUCTORS AND DESTRUCTORS
	WO_Goal();
	WO_Goal(Coord2D indices);
	~WO_Goal();

	//VIRTUAL
	int getType();

};

#endif