#ifndef WO_KEY_INCLUDED
#define WO_KEY_INCLUDED

#include "WorldObject.h"

class WO_Key : public WorldObject
{
private:
	char id;
	float radius;		//collision/drawing radius
	bool isHeld;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	WO_Key();
	WO_Key(Coord2D indices, char c);
	~WO_Key();

	//SETTERS
	void setID(char c);

	//GETTERS
	char getID();

	//VIRTUAL
	int getType();

};

#endif