#include "WO_Goal.h"

using namespace std;

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
WO_Goal::WO_Goal()
{
}

WO_Goal::WO_Goal(Coord2D indices)
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = indices;
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
}

WO_Goal::~WO_Goal()
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
int WO_Goal::getType()
{
	return GOAL_WOBJ;
}
