#ifndef INTER_INCLUDED
#define INTER_INCLUDED

#include "Vec3D.h"
#include "WorldObject.h"

class Intersection
{
private:
	WorldObject* int_obj;
	Vec3D int_pt;
	Vec3D normal;
	Vec3D up;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	Intersection();
	~Intersection();

	//SETTERS
	void setObject(WorldObject* wo);
	void setPoint(Vec3D p);
	void setNormal(Vec3D n);
	void setUp(Vec3D u);

	//GETTERS
	WorldObject* getObject();
	Vec3D getPoint();
	Vec3D getNormal();
	Vec3D getUp();

};

#endif