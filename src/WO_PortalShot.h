#ifndef PORTALSHOT_INCLUDED
#define PORTALSHOT_INCLUDED

#include "WorldObject.h"

class WO_PortalShot : public WorldObject
{
private:
	int start_time;
	Vec3D start_pos;
	Vec3D dir;
	WorldObject* portal;
	bool active;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	WO_PortalShot();
	~WO_PortalShot();

	//SETTERS
	void setStartTime(int t);
	void setStartPos(Vec3D p);
	void setDir(Vec3D d);
	void setPortal(WorldObject* port);

	//GETTERS
	int getStartTime();
	Vec3D getStartPos();
	Vec3D getDir();
	WorldObject* getPortal();

	//VIRTUAL
	int getType();

	//OTHERS
	void beginShot();
	bool shooting();
	void ceaseShot();

};

#endif