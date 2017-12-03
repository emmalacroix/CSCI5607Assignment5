#ifndef WO_START_INCLUDED
#define WO_START_INCLUDED

#include "WorldObject.h"

class WO_Start : public WorldObject
{
private:


public:
	//CONSTRUCTORS AND DESTRUCTORS
	WO_Start();
	WO_Start(Coord2D indices);
	~WO_Start();

	//VIRTUAL
	int getType();
};

#endif