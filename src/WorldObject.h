#ifndef WORLDOBJ_INCLUDED
#define WORLDOBJ_INCLUDED

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cstdio>
#include <iostream>

#include "Coord2D.h"
#include "Util.h"
#include "Material.h"
#include "Camera.h"
#include "Vec3D.h"

enum WOBJ_type
{
	EMPTY_WOBJ,
	WALL_WOBJ,
	DOOR_WOBJ,
	KEY_WOBJ,
	GOAL_WOBJ,
	START_WOBJ,
	PORTAL_WOBJ,
	SHOT_WOBJ
};

class WorldObject
{
protected:
	Vec3D world_pos;
	Coord2D world_indices;	//indices into World's object_array
	int world_level;
	Vec3D size;
	int start_vertex_index;	//index where vertices start in modelData array
	int total_vertices;	//total num of vertices within modelData array
	Material mat;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	WorldObject();
	WorldObject(Coord2D indices);
	~WorldObject();

	//SETTERS
	void setWPosition(Vec3D p);
	void setWIndices(Coord2D i);
	void setVertStartIndex(int i);
	void setTotalVertices(int i);
	void setMaterial(Material m);
	void setSize(Vec3D s);
	void setLevel(int l);

	//GETTERS
	Vec3D getWPosition();
	Coord2D getWIndices();
	int getStartIndex();
	int getTotalVertices();
	Material getMaterial();
	Vec3D getSize();
	int getLevel();

	//VIRTUAL
	virtual int getType();

	//OTHER
	void draw(Camera* cam, GLuint shaderProgram); //shared draw function among WObjs; WO_Portal has its own implementation

};

#endif