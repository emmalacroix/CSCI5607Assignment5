#include "Intersection.h"


/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
Intersection::Intersection()
{
	int_obj = nullptr;
	int_pt = Vec3D();
}


Intersection::~Intersection()
{
	
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void Intersection::setObject(WorldObject * wo)
{
	int_obj = wo;
}

void Intersection::setPoint(Vec3D p)
{
	int_pt = p;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
WorldObject * Intersection::getObject()
{
	return int_obj;
}

Vec3D Intersection::getPoint()
{
	return int_pt;
}
