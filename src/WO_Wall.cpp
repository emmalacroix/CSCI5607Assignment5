#include "WO_Wall.h"

using namespace std;

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
WO_Wall::WO_Wall()
{
}

WO_Wall::WO_Wall(Coord2D indices)
{
	//printf("\nWO_Wall constructed with indices : ");
	//indices.print();

	world_pos = Vec3D(0, 0, 0);
	world_indices = indices;
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
}


WO_Wall::~WO_Wall()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void WO_Wall::setFaceValues()
{
	min_x = world_pos.getX() - 0.5*size.getX();
	max_x = world_pos.getX() + 0.5*size.getX();

	min_y = world_pos.getY() - 0.5*size.getY();
	max_y = world_pos.getY() + 0.5*size.getY();

	min_z = world_pos.getZ() - 0.5*size.getZ();
	max_z = world_pos.getZ() + 0.5*size.getZ();
}

/*----------------------------*/
// GETTERS
/*----------------------------*/


/*----------------------------*/
// VIRTUALS
/*----------------------------*/
int WO_Wall::getType()
{
	return WALL_WOBJ;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
bool WO_Wall::getIntersection(Vec3D origin, Vec3D dir, Intersection& iSect)
{
	//code is largely inspired by ray-box intersection from this page:
	//https://tavianator.com/fast-branchless-raybounding-box-intersections/

	float dir_x = dir.getX();
	float dir_y = dir.getY();
	float dir_z = dir.getZ();

	float inv_x = INFINITY, inv_y = INFINITY, inv_z = INFINITY;

	Vec3D temp_norm = Vec3D();

	//check for divide by zeroes
	if (dir_x != 0.0) inv_x = 1.0 / dir_x;
	if (dir_y != 0.0) inv_y = 1.0 / dir_y;
	if (dir_z != 0.0) inv_z = 1.0 / dir_z;

	//test x faces
	float tx1 = (min_x - origin.getX())*inv_x;
	float tx2 = (max_x - origin.getX())*inv_x;

	float tmin = min(tx1, tx2);
	float tmax = max(tx1, tx2);

	(tmin == tx1) ? temp_norm = Vec3D(-1,0,0) : temp_norm = Vec3D(1, 0, 0);

	//test y faces
	float ty1 = (min_y - origin.getY())*inv_y;
	float ty2 = (max_y - origin.getY())*inv_y;

	tmin = max(tmin, min(ty1, ty2));
	tmax = min(tmax, max(ty1, ty2));

	if (tmax < tmin) return false; //did not intersect

	if (tmin == ty1) temp_norm = Vec3D(0, -1, 0);
	else if (tmin == ty2) temp_norm = Vec3D(0, 1, 0);

	//test z faces
	float tz1 = (min_z - origin.getZ())*inv_z;
	float tz2 = (max_z - origin.getZ())*inv_z;

	tmin = max(tmin, min(tz1, tz2));
	tmax = min(tmax, max(tz1, tz2));

	//determine minimum t value
	if (tmax >= tmin)
	{
		if (tmin == tz1) temp_norm = Vec3D(0, 0, -1);
		else if (tmin == tz2) temp_norm = Vec3D(0, 0, 1);

		iSect.setNormal(temp_norm);
		iSect.setPoint(origin + tmin*dir);
		return true; //does intersect!
	}

	return false;
}

Vec3D WO_Wall::setPortalDirection(Vec3D pos)
{
	Vec3D vec = pos - world_pos;
	Vec3D reflect = Vec3D(vec.getX(), vec.getY(), -1 * vec.getZ());
	if (dotProduct(vec, reflect) >= 0)
		//portal in yz plane
	{
		if (pos.getX() > world_pos.getX()) return Vec3D(1, 0, 0);
		else return Vec3D(-1, 0, 0);
	}
	else {
		//portal in xy plane
		if (pos.getZ() > world_pos.getZ()) return Vec3D(0, 0, 1);
		else return Vec3D(0, 0, -1);
	}
}