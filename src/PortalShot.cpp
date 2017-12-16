#include "PortalShot.h"


/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
PortalShot::PortalShot()
{
	start_time = 0;
	pos = Vec3D();
	dir = Vec3D();
	portal = nullptr;
	active = false;
}


PortalShot::~PortalShot()
{
	
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void PortalShot::setStartTime(int t)
{
	start_time = t;
}

void PortalShot::setPos(Vec3D p)
{
	pos = p;
}

void PortalShot::setDir(Vec3D d)
{
	dir = d;
}

void PortalShot::setPortal(WorldObject* port)
{
	portal = port;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
int PortalShot::getStartTime()
{
	return start_time;
}

Vec3D PortalShot::getPos()
{
	return pos;
}

Vec3D PortalShot::getDir()
{
	return dir;
}

WorldObject * PortalShot::getPortal()
{
	return portal;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
void PortalShot::beginShot()
{
	active = true;
}

bool PortalShot::shooting()
{
	return active;
}

void PortalShot::ceaseShot()
{
	active = false;
}