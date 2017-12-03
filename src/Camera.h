#ifndef CAMERA_INCLUDED
#define CAMERA_INCLUDED

#include "Vec3D.h"

class Camera
{
public:
	//CONSTRUCTORS AND DESTRUCTORS
	Camera();
	~Camera();

	//SETTERS
	void setPos(Vec3D c);
	void setDir(Vec3D c);
	void setUp(Vec3D c);
	void setHA(float h);
	void setRight(Vec3D c);

	//GETTERS
	Vec3D getPos();
	Vec3D getDir();
	Vec3D getUp();
	float getHA();
	Vec3D getRight();

private:
	Vec3D pos_VEC;
	Vec3D dir_VEC;
	Vec3D up_VEC;
	Vec3D right_VEC;
	float half_angle;

};

#endif