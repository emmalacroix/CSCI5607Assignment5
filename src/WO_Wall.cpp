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
Vec3D WO_Wall::setPortalDirection(Vec3D pos)
{
	Vec3D vec = pos - world_pos;
	Vec3D reflect = Vec3D(vec.getX(), vec.getY(), -1*vec.getZ());
	if (dotProduct(vec, reflect) >= 0)
	//portal in yz plane
	{
		if (pos.getX() > world_pos.getX()) return Vec3D(1, 0, 0);
		else return Vec3D(-1, 0, 0);
	} else {
	//portal in xy plane
		if (pos.getZ() > world_pos.getZ()) return Vec3D(0, 0, 1);
		else return Vec3D(0, 0, -1);
	}
}
