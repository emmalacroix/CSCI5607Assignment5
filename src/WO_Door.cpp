#include "WO_Door.h"

using namespace std;

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
WO_Door::WO_Door()
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = Coord2D(0, 0);
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
	id = '\0';
	locked = true;
}

WO_Door::WO_Door(Coord2D indices, char c)
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = indices;
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
	id = c;
	locked = true;
}


WO_Door::~WO_Door()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void WO_Door::setID(char c)
{
	id = c;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
char WO_Door::getID()
{
	return id;
}

bool WO_Door::isLocked()
{
	return locked;
}

/*----------------------------*/
// VIRTUALS
/*----------------------------*/
int WO_Door::getType()
{
	return DOOR_WOBJ;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
void WO_Door::lock()
{
	locked = true;
}

void WO_Door::unlock()
{
	locked = false;
}
