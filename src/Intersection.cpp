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

void Intersection::setNormal(Vec3D n)
{
	normal = n;
}

void Intersection::setUp(Vec3D u)
{
	up = u;
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

Vec3D Intersection::getNormal()
{
	return normal;
}

Vec3D Intersection::getUp()
{
	return up;
}
