#include "World.h"
#include "WO_Portal.h"

using namespace std;


/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
World::World()
{
	width = 0;
	height = 0;
	num_levels = 1;
	cell_width = 1;
	open_speed = 0.0005 * cell_width;
}

World::World(int w, int h)
{
	width = w;
	height = h;
	num_levels = 1;
	cell_width = 1;
	open_speed = 0.0005 * cell_width;
}

World::~World()
{
	delete floor;
	delete portal1;
	delete portal2;

	//delete each row of each level
	for (int i = 0; i < num_levels; i++)
	{
		for (int j = 0; j < width*height; j++)
		{
			delete levels_array[i][j];
		}
		delete[] levels_array[i]; //delete pointer to level i
	}

	//delete column of pointers that pointed to each level
	delete[] levels_array;
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

void World::setOvalIndices(int start, int tris)
{
	OVAL_START = start;
	OVAL_VERTS = tris;
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
	float i = ((v.getX() - 0.5*cell_width) / cell_width) + 0.1;		//col #
	float j = ((v.getZ() - 0.5*cell_width) / cell_width) + 0.1;		//row #
	float k = ((v.getY() - 0.5*cell_width) / cell_width) + 0.1;		//level #
	printf("getWO level %f indices : (%f , %f)\n", k, i, j);
	return levels_array[(int)k][(int)j*width + (int)i];
}

//map lies in xz plane
Vec3D World::getWorldPosition(Coord2D c, int level)
{
	return Vec3D(
		c.getI()* cell_width + 0.5*cell_width,
		level*cell_width,
		c.getJ()*cell_width + 0.5*cell_width);
}

Vec3D World::getStartWorldPosition()
{
	cout << "Start indices : ";
	start_indices.print();
	return getWorldPosition(start_indices, start_level);
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
	//1. dynamically allocate array of levels (each level is an array of WOBJ*)
	

	//2. Loop through rest, reading each line
	string line;
	int row_num = 0;
	int level_num = 0;
	vector<string> split_line = vector<string>();
	bool dim_set = false;

	while (input >> line) //Read first word in the line
	{
		cout << "\nRetrieved : " << line << endl;

		//check for "#" - comments
		if (line[0] == '#')
		{
			getline(input, line); //skip rest of line
			cout << "Skipping comment: " << line << endl;
			continue;
		}

		//split line along commas
		split_line = util::commaSplit(line);

		//check line length against width
		if (split_line.size() != width)
		{
			if (split_line.size() == 3 && !dim_set) //dimensions!
			{
				width = stoi(split_line.at(0));
				height = stoi(split_line.at(1));
				num_levels = stoi(split_line.at(2));

				printf("Dimensions set w : %i h : %i num_levels : %i\n", width, height, num_levels);

				printf("Allocating array of double ptrs -> levels_array : %i\n", num_levels);
				levels_array = new WorldObject**[num_levels];

				dim_set = true;
				continue;
			}
			else return false;
		}

		//dynamically allocate flattened array of objects (one single level)
		if (row_num == 0)
		{
			cout << "Allocating level " << level_num << " : " << width*height << endl;
			levels_array[level_num] = new WorldObject*[width*height];	//2D -> flattened 1D array
		}

		//go through each string in split_line
		for (int i = 0; i < width; i++)
		{
			//row_num --> z value
			//i		  --> x value
			if (split_line.at(i) == "0")
			{
				cout << "\tEmpty";
				levels_array[level_num][row_num*width + i] = new WorldObject(Coord2D(i, row_num));
				levels_array[level_num][row_num*width + i]->setLevel(level_num);
			}
			else if (split_line.at(i) == "S")
			{
				cout << "\tStart";
				levels_array[level_num][row_num*width + i] = buildStart(Coord2D(i, row_num), level_num);
			}
			else if (split_line.at(i)[0] == 'D')
			{
				cout << "\tDoor " << split_line.at(i)[1] << " : ";
				levels_array[level_num][row_num*width + i] = buildDoor(Coord2D(i, row_num), level_num, split_line.at(i)[1] - '0');
			}
			else if (split_line.at(i)[0] == 'K')
			{
				cout << "\tKey " << split_line.at(i)[1] << " : ";
				levels_array[level_num][row_num*width + i] = buildKey(Coord2D(i, row_num), level_num, split_line.at(i)[1] - '0');
			}
			else if (split_line.at(i) == "W")
			{
				cout << "\tWall";
				levels_array[level_num][row_num*width + i] = buildWall(Coord2D(i, row_num), level_num);
			}
			else if (split_line.at(i) == "G")
			{
				cout << "\tGoal";
				levels_array[level_num][row_num*width + i] = buildGoal(Coord2D(i, row_num), level_num);
			}
			else
			{
				cout << "ERROR. Invalid character in scenefile." << endl;
				return false;
			}

			printf(" Added at levels_array[%i][%i] : ", level_num, row_num*width + i);
			levels_array[level_num][row_num*width + i]->getWIndices().print();

		}//END line parsing For

		//plug filled in level into levels array if done reading all rows in a level
		if (row_num == height - 1)
		{
			//levels_array[level_num] = level;
			level_num++;
			row_num = 0;
		}
		else row_num++;

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

	//initialize portals

	//portal #1
	portal1 = new WO_Portal();
	portal1->setVertStartIndex(OVAL_START);
	portal1->setTotalVertices(OVAL_VERTS);

	Material mat1 = Material();
	mat1.setAmbient(glm::vec3(1, 0, 0));
	mat1.setDiffuse(glm::vec3(1, 0, 0));
	mat1.setSpecular(glm::vec3(0, 0, 0));

	portal1->setMaterial(mat1);
	portal1->setSize(Vec3D(cell_width, 2*cell_width, 0.01)); //xy plane

	//portal #2
	portal2 = new WO_Portal();
	portal2->setVertStartIndex(OVAL_START);
	portal2->setTotalVertices(OVAL_VERTS);

	Material mat2 = Material();
	mat2.setAmbient(glm::vec3(0, 0, 1));
	mat2.setDiffuse(glm::vec3(0, 0, 1));
	mat2.setSpecular(glm::vec3(0, 0, 0));

	portal2->setMaterial(mat2);
	portal2->setSize(Vec3D(cell_width, 2*cell_width, 0.01)); //xy plane
	
	return true;
}

//loops through WObj array and draws each
//also draws floor
void World::draw(Camera * cam, GLuint shaderProgram, GLuint uniTexID)
{
	for (int lev = 0; lev < num_levels; lev++)
	{
		for (int i = 0; i < width*height; i++)
		{
			if (levels_array[lev][i]->getType() != EMPTY_WOBJ)	//only draw non-empty WObjs
			{
				if (levels_array[lev][i]->getType() == DOOR_WOBJ)
				{
					WO_Door* door = (WO_Door*)levels_array[lev][i];
					Vec3D d_pos = door->getWPosition();

					//if the door IS NOT locked
					if (!door->isLocked())
					{
						if (d_pos.getY() < 2 * cell_width)
						{
							//printf("Moving door %c up!\n", door->getID());
							d_pos.setY(d_pos.getY() + (open_speed));	//move door up if not all the way up
						}
						else if (d_pos.getY() >= 2 * cell_width) //all the way up
						{
							door->lock();
							d_pos.setY(d_pos.getY() - (open_speed));	//move door down if all the way up
						}
					}
					else //if the door IS locked
					{
						if (d_pos.getY() > 0 && d_pos.getY() <= 2 * cell_width)
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
				levels_array[lev][i]->draw(cam, shaderProgram);
			}
		}//END row/col for loop
	}//END levels for loop

	glUniform1i(uniTexID, 1); //Set texture ID to use for floor (1 = brick texture)
	floor->draw(cam, shaderProgram);

	portal1->draw(cam, shaderProgram);
	portal2->draw(cam, shaderProgram);
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
	int k = (int)((pos.getY() - 0.5*cell_width) / cell_width);
	levels_array[k][j*width + i] = new WorldObject(); //make it empty!
}

/*----------------------------*/
// PRIVATE FUNCTIONS
/*----------------------------*/
//start will start off as a green sphere
WorldObject* World::buildStart(Coord2D c, int level)
{
	start_indices = c;
	start_level = level;
	
	WorldObject* start = new WO_Start(c);
	start->setVertStartIndex(SPHERE_START);
	start->setTotalVertices(SPHERE_VERTS);

	Material mat = Material();
	mat.setAmbient(glm::vec3(0, 1, 0));
	mat.setDiffuse(glm::vec3(0, 1, 0));
	mat.setSpecular(glm::vec3(0.75, 0.75, 0.75));

	start->setMaterial(mat);
	start->setSize(Vec3D(0.1, 0.1, 0.1));
	Vec3D p = getWorldPosition(c, level);
	start->setWPosition(p);
	start->setLevel(level);

	return start;
}

//a wall is a white cube
WorldObject* World::buildWall(Coord2D c, int level)
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
	wall->setWPosition(getWorldPosition(c, level));
	wall->setLevel(level);

	return wall;
}

//goal is a golden sphere
WorldObject* World::buildGoal(Coord2D c, int level)
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
	goal->setWPosition(getWorldPosition(c, level));
	goal->setLevel(level);

	return goal;
}

//key is a small cube
WorldObject* World::buildKey(Coord2D c, int level, int i)
{
	WorldObject* key = new WO_Key(c, i);
	key->setVertStartIndex(CUBE_START);
	key->setTotalVertices(CUBE_VERTS);

	Material mat = Material();
	glm::vec3 color = getLetterColor(i);
	mat.setAmbient(color);
	mat.setDiffuse(color);
	mat.setSpecular(glm::vec3(0.5, 0.5, 0.5));

	key->setMaterial(mat);
	key->setSize(cell_width*0.05*Vec3D(1,1,1));
	key->setWPosition(getWorldPosition(c, level));
	key->setLevel(level);

	return key;
}

//door is a wall-sized cube
WorldObject* World::buildDoor(Coord2D c, int level, int i)
{
	WorldObject* door = new WO_Door(c, i);
	door->setVertStartIndex(CUBE_START);
	door->setTotalVertices(CUBE_VERTS);

	Material mat = Material();
	glm::vec3 color = getLetterColor(i);
	mat.setAmbient(color);
	mat.setDiffuse(color);
	mat.setSpecular(glm::vec3(0.2, 0.2, 0.2));

	door->setMaterial(mat);
	door->setSize(Vec3D(cell_width, cell_width, cell_width));
	door->setWPosition(getWorldPosition(c, level));
	door->setLevel(level);

	return door;
}

//switch-case to get which color corresponds to read-in char
//for doors and keys
glm::vec3 World::getLetterColor(int i)
{
	switch (i)
	{
	case 1:
		return ORANGE;
	case 2:
		return BLUE;
	case 3:
		return RED;
	case 4:
		return PINK;
	case 5:
		return YELLOW;
	default:
		printf("\nERROR. Invalid id number entered for key/door ID.\n");
		return glm::vec3(-1, -1, -1);
	}
}

void World::movePortal1To(Vec3D pos)
{
	WO_Portal* portal = (WO_Portal*)portal1;
	portal->moveTo(pos);
}

void World::movePortal2To(Vec3D pos)
{
	WO_Portal* portal = (WO_Portal*)portal2;
	portal->moveTo(pos);
}
