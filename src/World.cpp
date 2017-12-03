#include "World.h"

using namespace std;


/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
World::World()
{
	width = 0;
	height = 0;
	cell_width = 1;
	open_speed = 0.0005 * cell_width;
}

World::World(int w, int h)
{
	width = w;
	height = h;
	cell_width = 1;
	open_speed = 0.0005 * cell_width;
}

World::~World()
{
	delete floor;

	//delete each row
	for (int i = 0; i < width*height; i++)
	{
		delete objects_array[i];
	}

	//delete column of pointers that pointed to each row
	delete[] objects_array;
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void World::setCellWidth(float w)
{
	cell_width = w;
	open_speed = 0.0005 * cell_width;
}

void World::setCubeIndices(int start, int tris)
{
	CUBE_START = start;
	CUBE_VERTS = tris;
}

void World::setSphereIndices(int start, int tris)
{
	SPHERE_START = start;
	SPHERE_VERTS = tris;
}

void World::setCollisionRadius(float f)
{
	collision_radius = f;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
int World::getWidth()
{
	return width;
}

int World::getHeight()
{
	return height;
}

float World::getCellWidth()
{
	return cell_width;
}

WorldObject* World::getWO(Vec3D v)
{
	float i = ((v.getX() - 0.5*cell_width) / cell_width) + 0.1;
	float j = ((v.getZ() - 0.5*cell_width) / cell_width) + 0.1;
	//printf("getWO indices : (%f , %f)\n", i, j);
	return objects_array[(int)j*width + (int)i];
}

//map lies in xz plane
Vec3D World::getWorldPosition(Coord2D c)
{
	return Vec3D(
		c.getI()* cell_width + 0.5*cell_width,
		0,
		c.getJ()*cell_width + 0.5*cell_width);
}

Vec3D World::getStartWorldPosition()
{
	cout << "Start indices : ";
	start_indices.print();
	return getWorldPosition(start_indices);
}

float World::getCollisionRadius()
{
	return collision_radius;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
//takes ifstream set up by main and parses scenefile
bool World::parseFile(ifstream & input)
{
	//1. capture dimensions
	input >> width >> height;

	//2. dynamically allocate array
	cout << "Allocating flattened 1D of length: " << width*height << endl;
	objects_array = new WorldObject*[width*height];		//2D -> flattened 1D array

	//3. Loop through rest, reading each line
	string line;
	int row_num = 0;
	while (input >> line) //Read first word in the line (i.e., the command type)
	{
		cout << "\nRetrieved line: " << line << endl;

		//check line length against width
		if (line.length() != width)
		{
			return false;
		}

		//go through each char of line
		for (int i = 0; i < width; i++)
		{
			//row_num --> z value
			//i		  --> x value
			switch (line[i])
			{
			case '0':
				cout << "\tEmpty";
				objects_array[row_num*width + i] = new WorldObject(Coord2D(i, row_num));
				break;
			case 'S':
				cout << "\tStart";
				objects_array[row_num*width + i] = buildStart(Coord2D(i, row_num));
				break;
			case 'W':
				cout << "\tWall";
				objects_array[row_num*width + i] = buildWall(Coord2D(i, row_num));
				break;
			case 'G':
				cout << "\tGoal";
				objects_array[row_num*width + i] = buildGoal(Coord2D(i, row_num));
				break;
			default:
				//must be a Key (lower case)
				if (islower(line[i]) && (line[i] - 'a' < 5)) //['a' , 'e']
				{
					cout << "\tKey: " << line[i];
					objects_array[row_num*width + i] = buildKey(Coord2D(i, row_num), line[i]);
				}
				//or a door (upper case)
				else if (isupper(line[i]) && (line[i] - 'A' < 5)) //['A' , 'E']
				{
					cout << "\tDoor: " << line[i];
					objects_array[row_num*width + i] = buildDoor(Coord2D(i, row_num), line[i]);
				}
				else
				{
					cout << "ERROR. Invalid character in scenefile." << endl;
					return false;
				}
				break;
			}//END switch

			printf(" Added at objects_array[%i] : ", row_num*width + i);
			objects_array[row_num*width + i]->getWIndices().print();

		}//END line parsing For

		row_num++;

	}//END parsing While
	cout << "\n\n";

	//initialize floor
	floor = new WorldObject();
	floor->setVertStartIndex(CUBE_START);
	floor->setTotalVertices(CUBE_VERTS);

	Material mat = Material();
	mat.setAmbient(glm::vec3(0.7, 0.7, 0.7));
	mat.setDiffuse(glm::vec3(0.7, 0.7, 0.7));
	mat.setSpecular(glm::vec3(0, 0, 0));

	floor->setMaterial(mat);
	floor->setSize(Vec3D(width*cell_width, 0.01, height*cell_width)); //xz plane

	Vec3D pos = Vec3D(width*cell_width*0.5, -0.5*cell_width, height*cell_width*0.5);

	floor->setWPosition(pos);
	
	return true;
}

//loops through WObj array and draws each
//also draws floor
void World::draw(Camera * cam, GLuint shaderProgram, GLuint uniTexID)
{
	for (int i = 0; i < width*height; i++)
	{
		if (objects_array[i]->getType() != EMPTY_WOBJ)	//only draw non-empty WObjs
		{
			if (objects_array[i]->getType() == DOOR_WOBJ)
			{
				WO_Door* door = (WO_Door*)objects_array[i];
				Vec3D d_pos = door->getWPosition();

				//if the door IS NOT locked
				if (!door->isLocked())
				{
					if (d_pos.getY() < 2*cell_width)
					{
						//printf("Moving door %c up!\n", door->getID());
						d_pos.setY(d_pos.getY() + (open_speed));	//move door up if not all the way up
					}
					else if (d_pos.getY() >= 2*cell_width) //all the way up
					{
						door->lock();
						d_pos.setY(d_pos.getY() - (open_speed));	//move door down if all the way up
					}
				}
				else //if the door IS locked
				{
					if (d_pos.getY() > 0 && d_pos.getY() <= 2*cell_width)
					{
						d_pos.setY(d_pos.getY() - (open_speed));	//move door down if all the way up
					}

					//do nothing if we're locked and at 0
				}

				door->setWPosition(d_pos);
				glUniform1i(uniTexID, 0); //Set texture ID to use (0 = wood texture)
			}//END DOOR_WOBJ if
			else
			{
				glUniform1i(uniTexID, -1); //Set texture ID to use (-1 = no texture)
			}
			objects_array[i]->draw(cam, shaderProgram);
		}
	}//END for loop

	glUniform1i(uniTexID, 1); //Set texture ID to use for floor (1 = brick texture)
	floor->draw(cam, shaderProgram);
}

//check if given pos vector collides with and WObjs in map
WorldObject* World::checkCollision(Vec3D pos)
{
	//cout << "\nChecking for collision : ";
	//pos.print();

	//check if in bounds of the map
	if (pos.getX() >= 0 &&
		pos.getZ() >= 0 &&
		pos.getX() < width*cell_width &&
		pos.getZ() < height*cell_width)
	{
		return getWO(pos);
	}

	return nullptr;
}

void World::removeWO(Vec3D pos)
{
	int i = (int)((pos.getX() - 0.5*cell_width) / cell_width);
	int j = (int)((pos.getZ() - 0.5*cell_width) / cell_width);
	objects_array[j*width + i] = new WorldObject(); //make it empty!
}

/*----------------------------*/
// PRIVATE FUNCTIONS
/*----------------------------*/
//start will start off as a green sphere
WorldObject* World::buildStart(Coord2D c)
{
	start_indices = c;
	
	WorldObject* start = new WO_Start(c);
	start->setVertStartIndex(SPHERE_START);
	start->setTotalVertices(SPHERE_VERTS);

	Material mat = Material();
	mat.setAmbient(glm::vec3(0, 1, 0));
	mat.setDiffuse(glm::vec3(0, 1, 0));
	mat.setSpecular(glm::vec3(0.75, 0.75, 0.75));

	start->setMaterial(mat);
	start->setSize(Vec3D(0.1, 0.1, 0.1));
	Vec3D p = getWorldPosition(c);
	start->setWPosition(p);

	return start;
}

//a wall is a white cube
WorldObject* World::buildWall(Coord2D c)
{
	WorldObject* wall = new WO_Wall(c);
	wall->setVertStartIndex(CUBE_START);
	wall->setTotalVertices(CUBE_VERTS);

	Material mat = Material();
	mat.setAmbient(glm::vec3(1, 1, 1));
	mat.setDiffuse(glm::vec3(1, 1, 1));
	mat.setSpecular(glm::vec3(0.2, 0.2, 0.2));

	wall->setMaterial(mat);
	wall->setSize(Vec3D(cell_width, cell_width, cell_width));
	wall->setWPosition(getWorldPosition(c));

	return wall;
}

//goal is a golden sphere
WorldObject* World::buildGoal(Coord2D c)
{
	WorldObject* goal = new WO_Goal(c);
	goal->setVertStartIndex(SPHERE_START);
	goal->setTotalVertices(SPHERE_VERTS);

	Material mat = Material();
	mat.setAmbient(glm::vec3(1, 0.8, 0));
	mat.setDiffuse(glm::vec3(1, 0.8, 0));
	mat.setSpecular(glm::vec3(1, 1, 1));

	goal->setMaterial(mat);
	goal->setSize(Vec3D(0.25, 0.25, 0.25));
	goal->setWPosition(getWorldPosition(c));

	return goal;
}

//key is a small cube
WorldObject* World::buildKey(Coord2D c, char ch)
{
	WorldObject* key = new WO_Key(c, ch);
	key->setVertStartIndex(CUBE_START);
	key->setTotalVertices(CUBE_VERTS);

	Material mat = Material();
	glm::vec3 color = getLetterColor(ch);
	mat.setAmbient(color);
	mat.setDiffuse(color);
	mat.setSpecular(glm::vec3(0.5, 0.5, 0.5));

	key->setMaterial(mat);
	key->setSize(cell_width*0.05*Vec3D(1,1,1));
	key->setWPosition(getWorldPosition(c));

	return key;
}

//door is a wall-sized cube
WorldObject* World::buildDoor(Coord2D c, char ch)
{
	WorldObject* door = new WO_Door(c, ch);
	door->setVertStartIndex(CUBE_START);
	door->setTotalVertices(CUBE_VERTS);

	Material mat = Material();
	glm::vec3 color = getLetterColor(ch);
	mat.setAmbient(color);
	mat.setDiffuse(color);
	mat.setSpecular(glm::vec3(0.2, 0.2, 0.2));

	door->setMaterial(mat);
	door->setSize(Vec3D(cell_width, cell_width, cell_width));
	door->setWPosition(getWorldPosition(c));

	return door;
}

//switch-case to get which color corresponds to read-in char
//for doors and keys
glm::vec3 World::getLetterColor(char ch)
{
	switch (ch)
	{
	case 'A':
	case 'a':
		return ORANGE;
	case 'B':
	case 'b':
		return BLUE;
	case 'C':
	case 'c':
		return RED;
	case 'D':
	case'd':
		return PINK;
	case 'E':
	case'e':
		return YELLOW;
	default:
		printf("\nERROR. Invalid char entered for key/door ID.\n");
		return glm::vec3(-1, -1, -1);
	}
}
