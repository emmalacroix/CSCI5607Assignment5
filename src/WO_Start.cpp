#include "WO_Start.h"

using namespace std;

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
WO_Start::WO_Start()
{
}

WO_Start::WO_Start(Coord2D indices)
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = indices;
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
}


WO_Start::~WO_Start()
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
int WO_Start::getType()
{
	return START_WOBJ;
}
