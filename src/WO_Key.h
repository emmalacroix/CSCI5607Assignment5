#ifndef WO_KEY_INCLUDED
#define WO_KEY_INCLUDED

#include "WorldObject.h"

class WO_Key : public WorldObject
{
private:
	int id;
	float radius;		//collision/drawing radius
	bool isHeld;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	WO_Key();
	WO_Key(Coord2D indices, int i);
	~WO_Key();

	//SETTERS
	void setID(int i);

	//GETTERS
	char getID();

	//VIRTUAL
	int getType();

};

#endif