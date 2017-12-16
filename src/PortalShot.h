#ifndef PORTALSHOT_INCLUDED
#define PORTALSHOT_INCLUDED

#include "Vec3D.h"
#include "WorldObject.h"

class PortalShot
{
private:
	int start_time;
	Vec3D pos;
	Vec3D dir;
	WorldObject* portal;
	bool active;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	PortalShot();
	~PortalShot();

	//SETTERS
	void setStartTime(int t);
	void setPos(Vec3D p);
	void setDir(Vec3D d);
	void setPortal(WorldObject* port);

	//GETTERS
	int getStartTime();
	Vec3D getPos();
	Vec3D getDir();
	WorldObject* getPortal();

	//OTHERS
	void beginShot();
	bool shooting();
	void ceaseShot();

};

#endif