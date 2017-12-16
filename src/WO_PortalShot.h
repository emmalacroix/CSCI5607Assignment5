#ifndef PORTALSHOT_INCLUDED
#define PORTALSHOT_INCLUDED

#include "WorldObject.h"
#include "WO_Portal.h"

class WO_PortalShot : public WorldObject
{
private:
	int start_time;
	Vec3D start_pos;
	Vec3D dir;
	WO_Portal* portal;
	bool active;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	WO_PortalShot();
	~WO_PortalShot();

	//SETTERS
	void setStartTime(int t);
	void setStartPos(Vec3D p);
	void setDir(Vec3D d);
	void setPortal(WO_Portal* port);

	//GETTERS
	int getStartTime();
	Vec3D getStartPos();
	Vec3D getDir();
	WO_Portal* getPortal();

	//VIRTUAL
	int getType();

	//OTHERS
	void beginShot();
	bool shooting();
	void ceaseShot();

};

#endif