#ifndef WO_WALL_INCLUDED
#define WO_WALL_INCLUDED

#include "WorldObject.h"
#include "Intersection.h"

class WO_Wall : public WorldObject
{
private:
	float min_x, max_x;
	float min_y, max_y;
	float min_z, max_z;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	WO_Wall();
	WO_Wall(Coord2D indices);
	~WO_Wall();

	//SETTERS
	void setFaceValues();

	//VIRTUAL
	int getType();

	//OTHERS
	bool getIntersection(Vec3D origin, Vec3D dir, Intersection& iSect);
	Vec3D setPortalDirection(Vec3D pos);
};

#endif