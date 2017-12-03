#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
Camera::Camera()
{
	//Default camera parameters
	pos_VEC = Vec3D(0, 0, 0);
	dir_VEC = Vec3D(0, 0, 1);
	up_VEC = Vec3D(0, 1, 0);
	right_VEC = Vec3D(1, 0, 0);
	half_angle = 45 * M_PI / 180.0;
}

Camera::~Camera()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void Camera::setPos(Vec3D c)
{
	pos_VEC = c;
}

void Camera::setDir(Vec3D c)
{
	c.normalize();
	dir_VEC = c;
}

void Camera::setUp(Vec3D c)
{
	c.normalize();
	up_VEC = c;
}

//Assumes passed in as degrees
void Camera::setHA(float h)
{
	half_angle = h * M_PI / 180.0;
}

void Camera::setRight(Vec3D c)
{
	c.normalize();
	right_VEC = c;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
Vec3D Camera::getPos()
{
	return pos_VEC;
}

Vec3D Camera::getDir()
{
	return dir_VEC;
}

Vec3D Camera::getUp()
{
	return up_VEC;
}

float Camera::getHA()
{
	return half_angle;
}

Vec3D Camera::getRight()
{
	return right_VEC;
}