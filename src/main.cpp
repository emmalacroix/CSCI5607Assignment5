////////////////////////////
//CSCI 5607 - Final Project
//Emma Lacroix - lacro058
//Nikki Kyllonen - kyllo089
////////////////////////////

#include "glad/glad.h"  //Include order can matter here

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#else
#include <SDL.h>
#include <SDL_opengl.h>
#endif

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

//OUR CLASSES
#include "World.h"
#include "Camera.h"
#include "Character.h"
#include "Util.h"
#include "Intersection.h"

using namespace std;

/*=============================*/
// Global Default Parameters
/*=============================*/
bool fullscreen = false;
int screen_width = 800;
int screen_height = 600;
float fov = 3.14f / 4;
//float horizontal_angle = 0; //keep track of angle the mouse has changed
//float vertical_angle = 0;   //the player view through!

//used for "speed" of character/camera
const float cell_width = 1.0;

const int jump_duration = 1000; //in milliseconds
const float mouse_speed = 10;

#ifdef __APPLE__
const float step_size = 0.006f * cell_width;
const float acceleration = 0.006f;
const float jump_height = 2.0;
#else
const float step_size = 0.002f * cell_width;
const float acceleration = 0.002f;
const float jump_height = 0.7*cell_width;
#endif

//shader globals
string vertFile = "Shaders/phong.vert";
string fragFile = "Shaders/phong.frag";

/*=============================*/
// Helper Function Declarations
/*=============================*/
ifstream checkSceneFile(char* fileName);
SDL_Window* initSDL(SDL_GLContext& context);
void onKeyDown(SDL_KeyboardEvent & event, Character* player, World* myWorld);
void mouseMove(SDL_MouseMotionEvent & event, Character* player, float horizontal_angle, float vertical_angle);
bool checkPosition(Vec3D& temp_pos, World* myWorld, Character* player);
bool updateCharacter(Character* player, World* myWorld);
void updateForFalling(Character* player, World* myWorld);
void updateForJumping(Character* player, World* myWorld);
bool updatePortalShot(World* myWorld, int time);

int main(int argc, char *argv[]) {
	/////////////////////////////////
	//OPEN SCENE FILE
	/////////////////////////////////
	if (argc != 2) {
		cout << "\nERROR: Incorrect usage. Expected ./a.out scenefile\n";
		exit(0);
	}

	char* fileName = argv[1];

	//open the file containing the scene description
	ifstream scene_input = checkSceneFile(fileName);
	if (scene_input.fail())
	{
		scene_input.close();
		exit(0);
	}

	/////////////////////////////////
	//INITIALIZE SDL WINDOW
	/////////////////////////////////
	SDL_GLContext context;
	SDL_Window* window = initSDL(context);

	if (window == NULL)
	{
		cout << "ERROR: initSDL() failed." << endl;
		scene_input.close();
		SDL_GL_DeleteContext(context);
		SDL_Quit();
		exit(0);
	}

	/////////////////////////////////
	//LOAD IN MODELS
	/////////////////////////////////
	World* myWorld = new World();

	int total_verts = 0;

	//CUBE
	int CUBE_VERTS = 0;
	float* cubeData = util::loadModel("models/cube.txt", CUBE_VERTS);
	cout << "Number of vertices in cube model : " << CUBE_VERTS << endl;
	total_verts += CUBE_VERTS;
	myWorld->setCubeIndices(0, CUBE_VERTS);

	//SPHERE
	int SPHERE_VERTS = 0;
	float* sphereData = util::loadModel("models/sphere.txt", SPHERE_VERTS);
	cout << "Number of vertices in sphere model : " << SPHERE_VERTS << endl;
	total_verts += SPHERE_VERTS;
	myWorld->setSphereIndices(CUBE_VERTS, SPHERE_VERTS);

	//OVAL
	int OVAL_VERTS = 0;
	float* ovalData = util::loadModel("models/oval.txt", OVAL_VERTS);
	cout << "Number of vertices in oval model : " << OVAL_VERTS << endl;
	total_verts += OVAL_VERTS;
	myWorld->setOvalIndices(CUBE_VERTS + SPHERE_VERTS, OVAL_VERTS);

	/////////////////////////////////
	//BUILD MODELDATA ARRAY
	/////////////////////////////////
	if (!(cubeData != nullptr && sphereData != nullptr && ovalData != nullptr))
	{
		cout << "ERROR. Unable to load model data." << endl;
		myWorld->~World();
		delete[] cubeData;
		delete[] sphereData;
		delete[] ovalData;
		SDL_GL_DeleteContext(context);
		SDL_Quit();
		exit(0);
	}
	float* modelData = new float[total_verts * 8];
	//copy data into modelData array
	copy(cubeData, cubeData + CUBE_VERTS * 8, modelData);
	copy(sphereData, sphereData + SPHERE_VERTS * 8, modelData + (CUBE_VERTS * 8));
	copy(ovalData, ovalData + OVAL_VERTS * 8, modelData + (CUBE_VERTS * 8) + (SPHERE_VERTS * 8));

	/////////////////////////////////
	//PARSE SCENE FILE
	/////////////////////////////////
	myWorld->setCellWidth(cell_width);
	cout << "\nCell width set to " << cell_width << endl;
	myWorld->setCollisionRadius(cell_width / 2.0);
	cout << "\nCollision radius set to " << cell_width / 2.0 << endl;

	if (!myWorld->parseFile(scene_input))
	{
		cout << "ERROR: file unsuccessfully parsed." << endl;

		//clean up and exit
		myWorld->~World();
		delete[] cubeData;
		delete[] sphereData;
		delete[] ovalData;
		scene_input.close();
		exit(0);
	}
	scene_input.close();
	cout << "\nSuccessfully parsed scenefile." << endl;

	/////////////////////////////////
	//SETUP CHARACTER
	/////////////////////////////////
	Vec3D start_pos = myWorld->getStartWorldPosition();
	cout << "Camera starting position : ";
	start_pos.print();

	Camera* cam = new Camera();

	Character* player = new Character();
	player->setDir(Vec3D(1, 0, 0));					//look along +x
	player->setPos(start_pos);						//start at the starting position
	player->setUp(Vec3D(0, 1, 0));					//map is in xz plane
	player->setRight(Vec3D(0, 0, 1));				//look along +x
	player->setJumpStart(-jump_duration);			//not jumping at start of program
	player->setTraveling(false);					//not traveling through a portal at start

	////////////////////////////////////////////////////
	//MOUSE : keep track of angle the mouse has changed
	//			player view through!
	////////////////////////////////////////////////////
	float horizontal_angle = 1.57f;
	float vertical_angle = 0;

	/////////////////////////////////
	//BUILD VERTEX ARRAY OBJECT
	/////////////////////////////////
	//This stores the VBO and attribute mappings in one object
	GLuint vao;
	glGenVertexArrays(1, &vao); //Create a VAO
	glBindVertexArray(vao); //Bind the above created VAO to the current context

	/////////////////////////////////
	//BUILD VERTEX BUFFER OBJECT
	/////////////////////////////////
	//Allocate memory on the graphics card to store geometry (vertex buffer object)
	GLuint vbo[1];
	glGenBuffers(1, vbo);  //Create 1 buffer called vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //Set the vbo as the active array buffer (Only one buffer can be active at a time)
	glBufferData(GL_ARRAY_BUFFER, total_verts * 8 * sizeof(float), modelData, GL_STATIC_DRAW); //upload vertices to vbo
	//GL_STATIC_DRAW means we won't change the geometry, GL_DYNAMIC_DRAW = geometry changes infrequently
	//GL_STREAM_DRAW = geom. changes frequently.  This effects which types of GPU memory is used

	/////////////////////////////////
	//SETUP SHADERS
	/////////////////////////////////
	GLuint shaderProgram = util::LoadShader("Shaders/phongTex.vert", "Shaders/phongTex.frag");

	//load in textures
	GLuint tex0 = util::LoadTexture("textures/metal_wall.bmp");
	GLuint tex1 = util::LoadTexture("textures/metal_floor2.bmp");
	GLuint tex2 = util::LoadTexture("textures/wood.bmp");

	if (tex0 == -1 || tex1 == -1 || tex2 == -1 || shaderProgram == -1)
	{
		//Clean Up
		SDL_GL_DeleteContext(context);
		SDL_Quit();
		myWorld->~World();
		cam->~Camera();
		player->~Character();
		delete[] modelData;
		delete[] cubeData;
		delete[] sphereData;
		delete[] ovalData;
	}

	//Tell OpenGL how to set fragment shader input
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	//Attribute, vals/attrib., type, normalized?, stride, offset
	//Binds to VBO current GL_ARRAY_BUFFER
	glEnableVertexAttribArray(posAttrib);

	GLint normAttrib = glGetAttribLocation(shaderProgram, "inNormal");
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(normAttrib);

	GLint texAttrib = glGetAttribLocation(shaderProgram, "inTexcoord");
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(texAttrib);

	glBindVertexArray(0); //Unbind the VAO in case we want to create a new one

	glEnable(GL_DEPTH_TEST);

	/*===========================================================================================
	* EVENT LOOP (Loop forever processing each event as fast as possible)
	* List of keycodes: https://wiki.libsdl.org/SDL_Keycode - You can catch many special keys
	* Scancode referes to a keyboard position, keycode referes to the letter (e.g., EU keyboards)
	===========================================================================================*/
	SDL_Event windowEvent;
	bool quit = false;
	bool complete = false;
	bool mouseActive = false;
	bool recenterMouse = false;
	Vec3D front_pos;
	Intersection sect;

	while (!quit && !complete)
	{
		while (SDL_PollEvent(&windowEvent))
		{
			switch (windowEvent.type)
			{
			case SDL_QUIT:
				quit = true; //Exit event loop
				break;
			case SDL_KEYUP:
				player->setVelocity(Vec3D(0, 0, 0));
				break;
			case SDL_KEYDOWN:
				//check for escape or fullscreen before checking other commands
				if (windowEvent.key.keysym.sym == SDLK_ESCAPE) quit = true; //Exit event loop
				else if (windowEvent.key.keysym.sym == SDLK_f) fullscreen = !fullscreen;
				onKeyDown(windowEvent.key, player, myWorld);
				break;
			case SDL_MOUSEMOTION:
				if (mouseActive)
				{
					mouseMove(windowEvent.motion, player, horizontal_angle, vertical_angle);
					recenterMouse = true;
				}
				else {
					SDL_WarpMouseInWindow(window, screen_width / 2, screen_height / 2);
					mouseActive = true;
				}
			case SDL_MOUSEBUTTONDOWN:
				if (windowEvent.button.button == SDL_BUTTON_LEFT)
				{
					cout << "Player shot gun at pos : ";
					player->getPos().print();
					myWorld->shootPortal(player->getPos(), player->getDir(), SDL_GetTicks(), myWorld->getPortal1());
				}
				else if (windowEvent.button.button == SDL_BUTTON_RIGHT) {
					myWorld->shootPortal(player->getPos(), player->getDir(), SDL_GetTicks(), myWorld->getPortal2());
				}
			default:
				break;
			}//END polling switch

			SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0); //Set to full screen
		}//END polling while

		// if (recenterMouse)
		// {
		// 	SDL_WarpMouseInWindow(window, screen_width/2, screen_height/2);
		// }

		if (myWorld->getShot()->shooting())
		{
			if (updatePortalShot(myWorld, SDL_GetTicks()))
			{
				front_pos = myWorld->getShot()->getWPosition() + 0.5*myWorld->getCollisionRadius()*myWorld->getShot()->getDir();
				sect = myWorld->checkCollision(front_pos);
				if (sect.getObject() != nullptr && sect.getObject()->getType() == WALL_WOBJ)
				{
					cout << "Shot hit a wall - Creating portal" << endl;
					WO_Wall * wall = (WO_Wall *)sect.getObject();

					if (wall->getIntersection(front_pos, myWorld->getShot()->getDir(), sect))
					{
						sect.setObject(wall);
						cout << "Placing portal at : ";
						Vec3D ppos = sect.getPoint() + 0.02*cell_width*sect.getNormal();
						ppos.print();
						cout << "Portal normal : ";
						sect.getNormal().print();
						myWorld->getShot()->getPortal()->setWPosition(ppos);
						myWorld->getShot()->getPortal()->setNorm(sect.getNormal());
						myWorld->movePortal(myWorld->getShot()->getPortal(), ppos);
						myWorld->getShot()->ceaseShot();
					}
				}
			}
			else {
				cout << "out of bounds" << endl;
				myWorld->getShot()->ceaseShot();
			}
		}

		updateForJumping(player, myWorld);
		updateForFalling(player, myWorld);
		complete = updateCharacter(player, myWorld);

		//after we figure out moving the Character - set the Camera params
		//by doing it this way, we could have the Camera and the Character
		//separate in the future or do over the shoulder instead of fps
		cam->setPos(player->getPos());
		cam->setDir(player->getDir());
		cam->setUp(player->getUp());
		cam->setRight(player->getRight());

		glClearColor(.2f, 0.4f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram); //Set the active shader (only one can be used at a time)

		//vertex shader uniforms
		//GLint uniView = glGetUniformLocation(shaderProgram, "view");
		//GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
		GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");

		//build view matrix from Camera
		glm::mat4 view = glm::lookAt(
			util::vec3DtoGLM(cam->getPos()),
			util::vec3DtoGLM(cam->getPos() + cam->getDir()),  //Look at point
			util::vec3DtoGLM(cam->getUp()));

		//glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		fullscreen ? fov = 3.14f / 2 : fov = 3.14f / 4;
		glm::mat4 proj = glm::perspective(fov, 800.0f / 600.0f, 0.01f, 200.0f); //FOV, aspect, near, far
		//glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex0);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex0"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex1);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, tex2);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 2);

		glBindVertexArray(vao);

		//draw all WObjs
		//Camera * cam, GLuint shaderProgram, GLuint uniTexID, glm::mat4 const &viewMat, glm::mat4 const &projMat, int maxRecLevel, int cur_recLevel
		myWorld->draw(cam, shaderProgram, uniTexID, view, proj, 1, 1);
		player->draw(cam, shaderProgram, myWorld, uniTexID);

		SDL_GL_SwapWindow(window);

	}//END event loop


	 //Clean Up
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	myWorld->~World();
	cam->~Camera();
	player->~Character();
	delete[] modelData;
	delete[] cubeData;
	delete[] sphereData;

	return 0;
}

/*--------------------------------------------------------------*/
// initSDL : initializes SDL and returns window pointer
/*--------------------------------------------------------------*/
SDL_Window* initSDL(SDL_GLContext& context)
{
	SDL_Init(SDL_INIT_VIDEO);  //Initialize Graphics (for OpenGL)
	
	//Ask SDL to get a recent version of OpenGL (3.2 or greater)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1); //ask SDL to give us a stencil buffer

	//Create a window (offsetx, offsety, width, height, flags)
	SDL_Window* window = SDL_CreateWindow("My OpenGL Program", 100, 100, screen_width, screen_height, SDL_WINDOW_OPENGL);
	float aspect = screen_width / (float)screen_height; //aspect ratio (needs to be updated if the window is resized)

														//Create a context to draw in
	context = SDL_GL_CreateContext(window);

	if (gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		printf("\nOpenGL loaded\n");
		printf("Vendor:   %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Version:  %s\n\n", glGetString(GL_VERSION));
	}
	else {
		printf("ERROR: Failed to initialize OpenGL context.\n");
		return NULL;
	}

	return window;
}

/*--------------------------------------------------------------*/
// checkSceneFile : opens and determines size of given sceneFile
/*--------------------------------------------------------------*/
ifstream checkSceneFile(char * fileName)
{
	ifstream scene_input(fileName);

	//check for errors in opening the file
	if (scene_input.fail())
	{
		cout << "\nCan't open file '" << fileName << "'" << endl;
		printf(strerror(errno));
		return scene_input;
	}

	//determine the file size
	streampos begin, end;
	begin = scene_input.tellg();
	scene_input.seekg(0, ios::end);
	end = scene_input.tellg();
	cout << "\nFile '" << fileName << "' is: " << (end - begin) << " bytes long.\n\n";
	scene_input.seekg(0, ios::beg);

	return scene_input;
}

/*--------------------------------------------------------------*/
// mouseMove : change player's direction if mouse is moved!
/*--------------------------------------------------------------*/

void mouseMove(SDL_MouseMotionEvent & event, Character* player, float horizontal_angle, float vertical_angle)
{
	Vec3D dir = player->getDir();
	Vec3D right = player->getRight();
	Vec3D up = player->getUp();

	//temps to be modified
	Vec3D temp_dir = dir;
	Vec3D temp_right = right;
	Vec3D temp_up = up;

	horizontal_angle += mouse_speed * step_size * float(screen_width / 2 - event.x);
	vertical_angle += mouse_speed * step_size * float(screen_height / 2 - event.y);

	temp_dir = dir + (Vec3D(cos(vertical_angle) * sin(horizontal_angle), sin(vertical_angle), cos(vertical_angle) * cos(horizontal_angle)));
	temp_right = right + (Vec3D(sin(horizontal_angle - 3.14f / 2.0f), 0, cos(horizontal_angle - 3.14f / 2.0f)));
	temp_up = cross(temp_dir, -1 * temp_right);

	player->setDir(temp_dir);
	player->setRight(temp_right);
	player->setUp(temp_up);
}


/*--------------------------------------------------------------*/
// onKeyDown : determine which key was pressed and how to edit
//				current translation or rotation parameters
/*--------------------------------------------------------------*/
void onKeyDown(SDL_KeyboardEvent & event, Character* player, World* myWorld)
{
	Vec3D pos = player->getPos();
	Vec3D dir = player->getDir();
	Vec3D right = player->getRight();
	Vec3D up = player->getUp();

	//temps to be modified in switch
	Vec3D temp_dir = dir;
	Vec3D temp_right = right;
	Vec3D temp_up = up;

	float collision_radius = myWorld->getCollisionRadius();
	Intersection iSect = myWorld->checkCollision(pos + 0.5*collision_radius*dir);
	WorldObject* front_obj = iSect.getObject();

	switch (event.keysym.sym)
	{
		/////////////////////////////////
		//TRANSLATION WITH WASD		   //
		/////////////////////////////////
	case SDLK_w:
		//printf("Up arrow pressed - step forward\n");
		player->setVelocity(Vec3D(step_size*dir.getX(), 0, step_size*dir.getZ()));
		break;
	case SDLK_s:
		//printf("Down arrow pressed - step backward\n");
		player->setVelocity(Vec3D(-1 * step_size*dir.getX(), 0, -1 * step_size*dir.getZ()));
		break;
	case SDLK_d:
		//printf("Right arrow pressed - step to the right\n");
		player->setVelocity(Vec3D(step_size*right.getX(), 0, step_size*right.getZ()));
		break;
	case SDLK_a:
		//printf("Left arrow pressed - step to the left\n");
		player->setVelocity(Vec3D(-1 * step_size*right.getX(), 0, -1 * step_size*right.getZ()));
		break;
	////////////////////////////////
	//JUMP WITH SPACEBAR		  //
	////////////////////////////////
	case SDLK_SPACE:
	{
		int time = SDL_GetTicks();
		if (time - player->getJumpStart() > jump_duration)
			//we are not already jumping
		{
			player->setJumpStart(time);
		}
		break;
	}

	////////////////////////////////
	//PICK UP ITEM WITH E		  //
	////////////////////////////////
	case SDLK_e:
	{
		//see what's in front of us
		if (front_obj->getType() == KEY_WOBJ)
		{
			WO_Key* key_obj = (WO_Key*)front_obj;
			cout << "Pressed space and grabbed key " << key_obj->getID() << endl;

			//1. place in Character's inventory
			player->addToInventory(key_obj);

			//2. remove from World map
			myWorld->removeWO(key_obj->getWPosition());
		}
		else
		{
			player->nextItem();
		}
		break;
	}
	default:
		break;
	}//END switch key press

	 //new dir, right, and up aren't affected by collisions
	player->setDir(temp_dir);
	player->setRight(temp_right);
	player->setUp(temp_up);
}//END onKeyDown

bool checkPosition(Vec3D& temp_pos, World* myWorld, Character* player)
{
	Vec3D pos = player->getPos();

	Intersection iSect = myWorld->checkCollision(temp_pos);
	WorldObject* collided_obj = iSect.getObject();
	Vec3D collided_pt = iSect.getPoint();

	if (collided_obj != nullptr)
	{
		//check what we collided with!
		switch (collided_obj->getType())
		{
		case GOAL_WOBJ:
			printf("\nCongrats!! You completed this map!!\n");
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
				"Congrats!! You completed this map!!",
				"Load in another map to keep playing!",
				NULL);
			return true; //true -- complete!
		case KEY_WOBJ:
			printf("Collided with a key\n");
			//temp_pos = pos; //don't move into the key
			break;
		case DOOR_WOBJ:
		{
			//check if we have the right key
			WO_Door* door = (WO_Door*)collided_obj;
			char d_id = door->getID();

			if (door->isLocked())
			{
				//cout << "Collided with locked door " << d_id << endl;

				if (player->hasKey(d_id))
				{
					printf("We have the right key (%i)!\n", d_id);
					door->unlock();
				}

				temp_pos = pos;
			}
			else
			{
				//cout << "Collided with unlocked door " << d_id << endl;

				if (door->getWPosition().getY() < myWorld->getCellWidth())
				{
					//if door is unlocked and not all the way up - don't move
					temp_pos = pos;
				}
				//walk through if it's unlocked and all the way up
			}
			break;
		}
		case WALL_WOBJ:
			//cout << "Collided with a wall" << endl;
			temp_pos = pos; //don't move into the wall
			break;
		case PORTAL_WOBJ:
			cout << "Collided with a portal" << endl;
			if (!player->isTraveling()) //we are just now entering a portal
			{
				player->enterPortal();
				WO_Portal * portal = (WO_Portal *)collided_obj;
				WO_Portal * partner_portal;
				if (portal == myWorld->getPortal1())
				{
					partner_portal = myWorld->getPortal2();
				} else {
					partner_portal = myWorld->getPortal1();
				}
				if (partner_portal->doesExist())
				{
					//cout << "h_angle is " << horizontal_angle << endl;
					//cout << "v_angle is " << vertical_angle << endl;
					temp_pos = partner_portal->getWPosition();
					//Vec3D norm = portal->getNorm();
					Vec3D partner_norm = partner_portal->getNorm();
					player->setDir(partner_norm);
					player->setRight(cross(player->getDir(), player->getUp()));
					//float h_angle = acos(dotProduct(Vec3D(norm.getX(), 0, norm.getZ()), Vec3D(partner_norm.getX(), 0, partner_norm.getZ())));
					//float v_angle = acos(dotProduct(Vec3D(0, norm.getY(), 0), Vec3D(0, partner_norm.getY(), 0)));
					//if (h_angle != 0.0) horizontal_angle += h_angle;
					//if (v_angle != 0.0) vertical_angle += v_angle;
					//cout << "h_angle is " << horizontal_angle << endl;
					//cout << "v_angle is " << vertical_angle << endl;
				}
			}
			break;
		default:
			//collided with start -- do nothing
			if (player->isTraveling()) //we were previously traveling through a portal
			{
				player->exitPortal(); //no longer traveling through portal
			}
			break;
		}//END collision switch
	}
	else //else temp_pos is out of bounds
	{
		temp_pos = pos;
	}

	return false;
}//END checkPosition

bool updateCharacter(Character * player, World * myWorld)
{

	Vec3D temp_pos = player->getPos() + player->getVelocity();

	if (!checkPosition(temp_pos, myWorld, player))
	{
		player->setPos(temp_pos);
		return false;
	}

	//returns true if game is complete
	return true;
}

void updateForFalling(Character * player, World * myWorld)
{
	int t = SDL_GetTicks() - player->getJumpStart(); //time since jump started
	if (t >= jump_duration/2)
	{
		Vec3D temp_vel = player->getVelocity();

		Vec3D pos = player->getPos();
		Vec3D dir = player->getDir();
		Vec3D right = player->getRight();
		Vec3D up = player->getUp();

		Vec3D down = Vec3D(0, -1, 0); //direction of "gravity"

		Intersection under_sect = myWorld->checkCollision(pos + 0.5*cell_width*down - 0.1*up);
		WorldObject* under_obj = under_sect.getObject();

		if (under_obj != nullptr)
		{
			if (!(under_obj->getType() == WALL_WOBJ) && !(under_obj->getType() == DOOR_WOBJ) && !(under_obj->getType() == PORTAL_WOBJ))
			{
				Vec3D vel = player->getVelocity();
				Vec3D down = Vec3D(0, -1, 0);

				//if they are parallel, then it was already falling
				if (!(temp_vel == Vec3D(0, 0, 0)) && (cross(temp_vel, down) == Vec3D(0, 0, 0)))
				{
					temp_vel = (step_size + acceleration)*down;
				}
				else
				{
					temp_vel = step_size*down;
				}
			} else {
				temp_vel = Vec3D(temp_vel.getX(), 0, temp_vel.getZ());
			}
			//if not Empty, don't change the velocity
		} else {
			temp_vel = Vec3D(temp_vel.getX(), 0, temp_vel.getZ());
		}
		player->setVelocity(temp_vel);
	}
}

void updateForJumping(Character* player, World* myWorld)
{
	int t = SDL_GetTicks() - player->getJumpStart(); //time since jump started
	if (t < jump_duration) //it has been less than jump_duration ms since jump started
	{
		Vec3D temp_vel = player->getVelocity();

		Vec3D pos = player->getPos();
		Vec3D dir = player->getDir();
		Vec3D right = player->getRight();
		Vec3D up = player->getUp();

		Intersection above_sect = myWorld->checkCollision(pos + 0.1*up);
		WorldObject* above_obj = above_sect.getObject();

		if (above_obj != nullptr)
		{
			if (!(above_obj->getType() == WALL_WOBJ) && !(above_obj->getType() == DOOR_WOBJ) && !(above_obj->getType() == PORTAL_WOBJ))
			{
				float x = (float)t / (float)jump_duration;
				float y = 4 - 8 * x; //derivative of y=-(1/4)*(4x-2)^2+1 (nicely arched jump parabola)
				temp_vel = Vec3D(temp_vel.getX(), y*jump_height*step_size*up.getY(), temp_vel.getZ());
			} else {
				player->setJumpStart(-jump_duration);
			}
		} else {
			player->setJumpStart(-jump_duration);
		}
		player->setVelocity(temp_vel);
	}
}

bool updatePortalShot(World* myWorld, int time) //returns true if update successful
{
	int t = time - myWorld->getShot()->getStartTime(); //time since shot was fired
	Vec3D temp_pos = myWorld->getShot()->getStartPos() + t*step_size*myWorld->getShot()->getDir();
	Intersection iSect = myWorld->checkCollision(temp_pos);
	WorldObject* collided_obj = iSect.getObject();
	if (collided_obj != nullptr)
	{
		myWorld->getShot()->setWPosition(temp_pos);
		return true;
	}
	else {
		cout << "out of bounds" << endl;
		return false;
	}
}