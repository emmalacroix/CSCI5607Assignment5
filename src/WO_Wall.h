#ifndef WO_WALL_INCLUDED
#define WO_WALL_INCLUDED

#include "WorldObject.h"

class WO_Wall : public WorldObject
{
private:


public:
	//CONSTRUCTORS AND DESTRUCTORS
	WO_Wall();
	WO_Wall(Coord2D indices);
	~WO_Wall();

	//VIRTUAL
	int getType();
};

#endif