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
