#include "WO_Portal.h"

using namespace std;

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
WO_Portal::WO_Portal()
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = Coord2D(0, 0);
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
	exists = false;
}

WO_Portal::WO_Portal(Coord2D indices)
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = indices;
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
	exists = true;
}

WO_Portal::~WO_Portal()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void WO_Portal::setExists(bool b)
{
	exists = b;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
bool WO_Portal::doesExist()
{
	return exists;
}

/*----------------------------*/
// VIRTUALS
/*----------------------------*/
int WO_Portal::getType()
{
	return PORTAL_WOBJ;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
void WO_Portal::moveTo(Coord2D indices)
{
	world_indices = indices;
}