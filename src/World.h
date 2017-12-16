#ifndef WORLD_INCLUDED
#define WORLD_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

#include "Vec3D.h"
#include "Coord2D.h"
#include "Camera.h"
#include "Util.h"
#include "Intersection.h"
#include "PortalShot.h"

//WorldObject classes
#include "WorldObject.h"
#include "WO_Door.h"
#include "WO_Goal.h"
#include "WO_Key.h"
#include "WO_Start.h"
#include "WO_Wall.h"
#include "WO_Portal.h"

class World{
private:
	WorldObject*** levels_array;
	int width;
	int height;
	int num_levels;
	Coord2D start_indices;
	int start_level;
	float cell_width;				//width of each cell/square of map
	WorldObject* floor;
	WO_Portal* portal1;
	WO_Portal* portal2;
	PortalShot* shot;
	float collision_radius;
	float open_speed;

	//modelData indices
	int CUBE_START = 0;
	int CUBE_VERTS = 0;
	int SPHERE_START = 0;
	int SPHERE_VERTS = 0;
	int OVAL_START = 0;
	int OVAL_VERTS = 0;

	//5 possible door and key colors
	glm::vec3 ORANGE = glm::vec3(1.0, 0.5, 0.0);
	glm::vec3 BLUE = glm::vec3(0, 0, 1);
	glm::vec3 RED = glm::vec3(1, 0, 0);
	glm::vec3 PINK = glm::vec3(0.5, 0, 0);
	glm::vec3 YELLOW = glm::vec3(0.9, 0.7, 0.1);

	//PRIVATE FUNCTIONS
	WorldObject* buildStart(Coord2D c, int level);
	WorldObject* buildWall(Coord2D c, int level);
	WorldObject* buildGoal(Coord2D c, int level);
	WorldObject* buildKey(Coord2D c, int level, int i);
	WorldObject* buildDoor(Coord2D c, int level, int i);
	glm::vec3 getLetterColor(int i);

public:
	//CONSTRUCTORS AND DESTRUCTORS
	World();
	World(int w, int h);
	~World();

	//SETTERS
	void setCellWidth(float w);
	void setCubeIndices(int start, int tris);
	void setSphereIndices(int start, int tris);
	void setOvalIndices(int start, int tris);
	void setCollisionRadius(float f);

	//GETTERS
	int getWidth();
	int getHeight();
	float getCellWidth();
	WorldObject* getWO(Vec3D v);
	Vec3D getWorldPosition(Coord2D c, int level);
	Vec3D getStartWorldPosition();
	float getCollisionRadius();

	//OTHERS
	bool parseFile(std::ifstream& input);
	void draw(Camera * cam, GLuint shaderProgram, GLuint uniTexID);
	Intersection checkCollision(Vec3D pos);
	void removeWO(Vec3D pos);
	void movePortal1(Vec3D pos);
	void movePortal2(Vec3D pos);
	void shootPortal(Vec3D pos, Vec3D dir, int time, WO_Portal* portal);

};

#endif
