#ifndef WO_PORTAL_INCLUDED
#define WO_PORTAL_INCLUDED

#include "WorldObject.h"
#include "Intersection.h"
#include <cmath>

class WO_Portal : public WorldObject
{
private:
	bool exists;
	Vec3D normal;
	Vec3D up;
	glm::mat4 model;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	WO_Portal();
	~WO_Portal();

	//SETTERS
	void setExists(bool b);
	void setNorm(Vec3D n);
	void setUp(Vec3D u);

	//GETTERS
	bool doesExist();
	Vec3D getNorm();
	Vec3D getUp();
	glm::mat4 getModelMat();

	//VIRTUAL
	int getType();
	void draw(Camera* cam, GLuint shaderProgram);

	//OTHERS
	void moveTo(Vec3D pos);
	void calcModel();
};
#endif