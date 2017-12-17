#include "WO_PortalShot.h"


/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
WO_PortalShot::WO_PortalShot()
{
	start_time = 0;
	start_pos = Vec3D();
	dir = Vec3D();
	portal = nullptr;
	active = false;
}


WO_PortalShot::~WO_PortalShot()
{

}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void WO_PortalShot::setStartTime(int t)
{
	start_time = t;
}

void WO_PortalShot::setStartPos(Vec3D p)
{
	start_pos = p;
}

void WO_PortalShot::setDir(Vec3D d)
{
	dir = d;
}

void WO_PortalShot::setPortal(WO_Portal * port)
{
	portal = port;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
int WO_PortalShot::getStartTime()
{
	return start_time;
}

Vec3D WO_PortalShot::getStartPos()
{
	return start_pos;
}

Vec3D WO_PortalShot::getDir()
{
	return dir;
}

WO_Portal * WO_PortalShot::getPortal()
{
	return portal;
}

/*----------------------------*/
// VIRTUALS
/*----------------------------*/
int WO_PortalShot::getType()
{
	return SHOT_WOBJ;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
void WO_PortalShot::beginShot()
{
	active = true;
}

bool WO_PortalShot::shooting()
{
	return active;
}

void WO_PortalShot::ceaseShot()
{
	active = false;
}