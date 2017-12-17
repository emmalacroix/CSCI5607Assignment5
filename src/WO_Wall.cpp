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

	//check for divide by zeroes
	if (dir_x != 0.0) inv_x = 1.0 / dir_x;
	if (dir_y != 0.0) inv_y = 1.0 / dir_y;
	if (dir_z != 0.0) inv_z = 1.0 / dir_z;

	//get min x,y,z values
	float min_x = world_pos.getX() - 0.5*size.getX();
	float max_x = world_pos.getX() + 0.5*size.getX();

	float min_y = world_pos.getY() - 0.5*size.getY();
	float max_y = world_pos.getY() + 0.5*size.getY();

	float min_z = world_pos.getZ() - 0.5*size.getZ();
	float max_z = world_pos.getZ() + 0.5*size.getZ();

	//test x faces
	float tx1 = (min_x - origin.getX())*inv_x;
	float tx2 = (max_x - origin.getX())*inv_x;

	float tmin = min(tx1, tx2);
	float tmax = max(tx1, tx2);

	cout << "tmin : " << tmin << endl;
	cout << "tmax : " << tmax << endl;

	//test y faces
	float ty1 = (min_y - origin.getY())*inv_y;
	float ty2 = (max_y - origin.getY())*inv_y;

	tmin = max(tmin, min(ty1, ty2));
	tmax = min(tmax, max(ty1, ty2));

	cout << "tmin : " << tmin << endl;
	cout << "tmax : " << tmax << endl;

	if (tmax < tmin) return false; //did not intersect

	//test z faces
	float tz1 = (min_z - origin.getZ())*inv_z;
	float tz2 = (max_z - origin.getZ())*inv_z;

	tmin = max(tmin, min(tz1, tz2));
	tmax = min(tmax, max(tz1, tz2));

	cout << "tmin : " << tmin << endl;
	cout << "tmax : " << tmax << endl;

	//determine minimum t value
	if (tmax >= tmin)
	{
		iSect.setPoint(origin + tmin*dir);
		return true;			//does intersect!
	}

	return false;
}
