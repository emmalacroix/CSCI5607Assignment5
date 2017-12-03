#include "Vec3D.h"

#include <cmath>
#include <iostream>

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
Vec3D::Vec3D()
{
	x = 0, y = 0, z = 0;
}

Vec3D::Vec3D(float xx, float yy, float zz)
{
	x = xx, y = yy, z = zz;
}


Vec3D::~Vec3D()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void Vec3D::setX(float xx)
{
	x = xx;
}

void Vec3D::setY(float yy)
{
	y = yy;
}

void Vec3D::setZ(float zz)
{
	z = zz;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
float Vec3D::getX()
{
	return x;
}

float Vec3D::getY()
{
	return y;
}

float Vec3D::getZ()
{
	return z;
}

float Vec3D::getMagnitude()
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

/*----------------------------*/
// OPERATORS
/*----------------------------*/
Vec3D operator+ (const Vec3D& a, const Vec3D& b)
{
	return Vec3D(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3D operator- (const Vec3D& a, const Vec3D& b)
{
	return Vec3D(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3D operator* (float f, const Vec3D& a)
{
	return Vec3D(a.x*f, a.y*f, a.z*f);
}

Vec3D operator*(const Vec3D & a, const Vec3D & b)
{
	Vec3D result;

	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	
	return result;
}

float dotProduct(const Vec3D & a, const Vec3D & b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
//Normalize current Vec3D in place
void Vec3D::normalize()
{
	float mag = this->getMagnitude();

	x /= mag;
	y /= mag;
	z /= mag;
}

Vec3D cross(const Vec3D& a, const Vec3D& b)
{
	return Vec3D(a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x);
}

void Vec3D::print()
{
	printf("(%f , %f , %f)\n", x, y, z);
}