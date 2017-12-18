#include "Character.h"

using namespace std;

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
Character::Character()
{
	//Default camera parameters
	pos_VEC = Vec3D(0, 0, 0);
	dir_VEC = Vec3D(0, 0, 1);
	up_VEC = Vec3D(0, 1, 0);
	right_VEC = Vec3D(1, 0, 0);
	cur_item_index = -1;
}


Character::~Character()
{
	//loop through inventory and delete items
	for (int i = 0; i < inventory.size(); i++)
	{
		delete inventory.at(i);
	}
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void Character::setPos(Vec3D c)
{
	pos_VEC = c;
}

void Character::setDir(Vec3D c)
{
	c.normalize();
	dir_VEC = c;
}

void Character::setUp(Vec3D c)
{
	c.normalize();
	up_VEC = c;
}

void Character::setRight(Vec3D c)
{
	c.normalize();
	right_VEC = c;
}

void Character::setVelocity(Vec3D v)
{
	vel_VEC = v;
}

void Character::setJumpStart(int s)
{
	jump_start = s;
}

void Character::setTraveling(bool b)
{
	portal_traveling = b;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
Vec3D Character::getPos()
{
	return pos_VEC;
}

Vec3D Character::getDir()
{
	return dir_VEC;
}

Vec3D Character::getUp()
{
	return up_VEC;
}

Vec3D Character::getRight()
{
	return right_VEC;
}

WorldObject * Character::getCurItem()
{
	//just to be safe check that we have some stuff
	if (cur_item_index != -1)
	{
		return inventory.at(cur_item_index);
	}

	return nullptr;
}

void Character::nextItem()
{
	//just to be safe check that we have some stuff
	if (cur_item_index != -1)
	{
		cur_item_index++;

		if (cur_item_index >= inventory.size())
		{
			cur_item_index = 0;
		}
	}
}

Vec3D Character::getVelocity()
{
	return vel_VEC;
}

int Character::getJumpStart()
{
	return jump_start;
}

bool Character::isTraveling()
{
	return portal_traveling;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
bool Character::isInventoryEmpty()
{
	return (cur_item_index == -1);
}

void Character::addToInventory(WorldObject * wobj)
{
	inventory.push_back(wobj);
	cur_item_index++;
}

void Character::draw(Camera* cam, GLuint shaderProgram, World* myWorld, GLuint uniTexID)
{
	//draw current item in front
	WorldObject* curItem = getCurItem();
	
	if (curItem != nullptr)
	{
		curItem->setWPosition(pos_VEC + myWorld->getCollisionRadius() * dir_VEC);
		glUniform1i(uniTexID, -1); //Set texture ID to use (-1 = no texture)
		curItem->draw(cam, shaderProgram);
	}

}

//check if current item is the right key
bool Character::hasKey(char c)
{
	//just to be safe check that we have some stuff
	if (cur_item_index != -1)
	{
		if (inventory.at(cur_item_index)->getType() == KEY_WOBJ)
		{
			WO_Key* key = (WO_Key*)inventory.at(cur_item_index);
			char k_id = key->getID();
			cout << "Currently holding key " << k_id << endl;

			if (k_id == tolower(c))
			{
				return true;
			}
		}
	}
	return false;
}

void Character::enterPortal()
{
	portal_traveling = true;
}

void Character::exitPortal()
{
	portal_traveling = false;
}
