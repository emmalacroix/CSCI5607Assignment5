#ifndef WO_PORTAL_INCLUDED
#define WO_PORTAL_INCLUDED

#include "WorldObject.h"
class WO_Portal : public WorldObject
{
private:
	bool exists;
	Vec3D dir;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	WO_Portal();
	~WO_Portal();

	//SETTERS
	void setExists(bool b);
	void setDir(Vec3D d);

	//GETTERS
	bool doesExist();
	Vec3D getDir();

	//VIRTUAL
	int getType();
	void draw(Camera* cam, GLuint shaderProgram);

	//OTHERS
	void moveTo(Vec3D pos);
};
#endif