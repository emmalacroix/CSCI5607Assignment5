#ifndef WO_DOOR_INCLUDED
#define WO_DOOR_INCLUDED

#include "WorldObject.h"
class WO_Door :	public WorldObject
{
private:
	char id;
	bool locked;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	WO_Door();
	WO_Door(Coord2D indices, char c);
	~WO_Door();

	//SETTERS
	void setID(char c);

	//GETTERS
	char getID();
	bool isLocked();

	//VIRTUAL
	int getType();

	//OTHERS
	void lock();
	void unlock();

};

#endif