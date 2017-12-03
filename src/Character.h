#ifndef CHARACTER_INCLUDED
#define CHARACTER_INCLUDED

#include <vector>

#include "Vec3D.h"
#include "Coord2D.h"
#include "WorldObject.h"
#include "World.h"

class Character
{
private:
	//similar to Camera params
	Vec3D pos_VEC;
	Vec3D dir_VEC;
	Vec3D right_VEC;
	Vec3D up_VEC;

	//inventory
	std::vector<WorldObject*> inventory;
	int cur_item_index;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	Character();
	~Character();

	//SETTERS
	void setPos(Vec3D c);
	void setDir(Vec3D c);
	void setUp(Vec3D c);
	void setRight(Vec3D c);

	//GETTERS
	Vec3D getPos();
	Vec3D getDir();
	Vec3D getUp();
	Vec3D getRight();
	WorldObject* getCurItem();
	void nextItem();

	//OTHERS
	bool isInventoryEmpty();
	void addToInventory(WorldObject* wobj);
	void draw(Camera* cam, GLuint shaderProgram, World* myWorld, GLuint uniTexID);
	bool hasKey(char c);

};

#endif