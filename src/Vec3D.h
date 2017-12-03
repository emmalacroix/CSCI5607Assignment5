#ifndef VEC3D_INCLUDED
#define VEC3D_INCLUDED


class Vec3D
{
public:
	//CONSTRUCTORS AND DESTRUCTORS
	Vec3D();
	Vec3D(float xx, float yy, float zz);
	~Vec3D();

	//SETTERS
	void setX(float xx);
	void setY(float yy);
	void setZ(float zz);

	//GETTERS
	float getX();
	float getY();
	float getZ();
	float getMagnitude();

	//OPERATORS
	//Component-wise addition of coordinates
	friend Vec3D operator+ (const Vec3D& a, const Vec3D& b);

	//Component-wise subtraction of coordinates
	friend Vec3D operator- (const Vec3D& a, const Vec3D& b);

	//Component-wise multiplication
	friend Vec3D operator* (float f, const Vec3D& a);

	//Component-wise multiplication
	friend Vec3D operator* (const Vec3D& a, const Vec3D& b);

	//Component-wise multiplication (dot product)
	friend float dotProduct(const Vec3D& a, const Vec3D& b);

	//OTHERS
	void normalize();
	friend Vec3D cross(const Vec3D& a, const Vec3D& b);
	void print();

private:
	float x;
	float y;
	float z;
};

#endif