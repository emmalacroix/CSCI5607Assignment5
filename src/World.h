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

//WorldObject classes
#include "WorldObject.h"
#include "WO_Door.h"
#include "WO_Goal.h"
#include "WO_Key.h"
#include "WO_Start.h"
#include "WO_Wall.h"

class World{
private:
	WorldObject** objects_array;
	int width;
	int height;
	Coord2D start_indices;
	float cell_width;				//width of each cell/square of map
	WorldObject* floor;
	float collision_radius;
	float open_speed;

	//modelData indices
	int CUBE_START = 0;
	int CUBE_VERTS = 0;
	int SPHERE_START = 0;
	int SPHERE_VERTS = 0;

	//5 possible door and key colors
	glm::vec3 ORANGE = glm::vec3(1.0, 0.5, 0.0);
	glm::vec3 BLUE = glm::vec3(0, 0, 1);
	glm::vec3 RED = glm::vec3(1, 0, 0);
	glm::vec3 PINK = glm::vec3(0.5, 0, 0);
	glm::vec3 YELLOW = glm::vec3(0.9, 0.7, 0.1);

	//PRIVATE FUNCTIONS
	WorldObject* buildStart(Coord2D c);
	WorldObject* buildWall(Coord2D c);
	WorldObject* buildGoal(Coord2D c);
	WorldObject* buildKey(Coord2D c, char ch);
	WorldObject* buildDoor(Coord2D c, char ch);
	glm::vec3 getLetterColor(char ch);

public:
	//CONSTRUCTORS AND DESTRUCTORS
	World();
	World(int w, int h);
	~World();

	//SETTERS
	void setCellWidth(float w);
	void setCubeIndices(int start, int tris);
	void setSphereIndices(int start, int tris);
	void setCollisionRadius(float f);

	//GETTERS
	int getWidth();
	int getHeight();
	float getCellWidth();
	WorldObject* getWO(Vec3D v);
	Vec3D getWorldPosition(Coord2D c);
	Vec3D getStartWorldPosition();
	float getCollisionRadius();

	//OTHERS
	bool parseFile(std::ifstream& input);
	void draw(Camera * cam, GLuint shaderProgram, GLuint uniTexID);
	WorldObject* checkCollision(Vec3D pos);
	void removeWO(Vec3D pos);

};

#endif
