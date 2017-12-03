#include "WO_Key.h"

using namespace std;

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
WO_Key::WO_Key()
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = Coord2D(0,0);
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
	id = '\0';
}

WO_Key::WO_Key(Coord2D indices, char c)
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = indices;
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
	id = c;
}


WO_Key::~WO_Key()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void WO_Key::setID(char c)
{
	id = c;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
char WO_Key::getID()
{
	return id;
}

/*----------------------------*/
// VIRTUALS
/*----------------------------*/
int WO_Key::getType()
{
	return KEY_WOBJ;
}
