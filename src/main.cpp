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

//MY CLASSES
#include "World.h"
#include "Camera.h"
#include "Character.h"
#include "Util.h"

using namespace std;

/*=============================*/
// Global Default Parameters
/*=============================*/
bool fullscreen = false;
int screen_width = 800;
int screen_height = 600;

const float step_size = 0.25f;

//shader globals
string vertFile = "Shaders/phong.vert";
string fragFile = "Shaders/phong.frag";

/*=============================*/
// Helper Function Declarations
/*=============================*/
ifstream checkSceneFile(char* fileName);
SDL_Window* initSDL(SDL_GLContext& context);

int main(int argc, char const *argv[]) {
  //used to for "speed" of character/camera
	const float cell_width = 1.0;

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
	//PARSE SCENE FILE
	/////////////////////////////////
  World* myWorld = new World();
	myWorld->setCellWidth(cell_width);
	cout << "\nCell width set to " << cell_width << endl;
	myWorld->setCollisionRadius(cell_width/2.0);
	cout << "\nCollision radius set to " << cell_width/2.0 << endl;

	if (!myWorld->parseFile(scene_input))
	{
		cout << "ERROR: file unsuccessfully parsed." << endl;

		//clean up and exit
		delete[] cubeData;
		delete[] sphereData;
		delete[] modelData;
		myWorld->~World();
		scene_input.close();
		exit(0);
	}
	scene_input.close();
	cout << "\nSuccessfully parsed scenefile." << endl;

  /////////////////////////////////
	//SETUP CHARACTER
	/////////////////////////////////
	Vec3D start_pos = myWorld->getStartWorldPosition();
	//start_pos.setY(0.25*cell_width);
	cout << "Camera starting position : ";
	start_pos.print();

	Camera* cam = new Camera();

	Character* player = new Character();
	player->setDir(Vec3D(1, 0, 0));					//look along +x
	player->setPos(start_pos);						//start at the starting position
	player->setUp(Vec3D(0, 1, 0));					//map is in xz plane
	player->setRight(Vec3D(0, 0, 1));				//look along +x

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

	/////////////////////////////////
	//BUILD MODELDATA ARRAY
	/////////////////////////////////
	if (!(cubeData != nullptr && sphereData != nullptr))
	{
		cout << "ERROR. Unable to load model data." << endl;
		myWorld->~World();
		delete[] cubeData;
		delete[] sphereData;
		SDL_GL_DeleteContext(context);
		SDL_Quit();
		exit(0);
	}
	float* modelData = new float[total_verts * 8];
	//copy data into modelData array
	copy(cubeData, cubeData + CUBE_VERTS * 8, modelData);
	copy(sphereData, sphereData + SPHERE_VERTS * 8, modelData + (CUBE_VERTS * 8));

  /////////////////////////////////
	//BUILD VERTEX ARRAY OBJECT	   //
	/////////////////////////////////
	//This stores the VBO and attribute mappings in one object
	GLuint vao;
	glGenVertexArrays(1, &vao); //Create a VAO
	glBindVertexArray(vao); //Bind the above created VAO to the current context

	/////////////////////////////////
	//BUILD VERTEX BUFFER OBJECT   //
	/////////////////////////////////
	//Allocate memory on the graphics card to store geometry (vertex buffer object)
	GLuint vbo[1];
	glGenBuffers(1, vbo);  //Create 1 buffer called vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //Set the vbo as the active array buffer (Only one buffer can be active at a time)
	glBufferData(GL_ARRAY_BUFFER, total_verts * 8 * sizeof(float), modelData, GL_STATIC_DRAW); //upload vertices to vbo
	//GL_STATIC_DRAW means we won't change the geometry, GL_DYNAMIC_DRAW = geometry changes infrequently
	//GL_STREAM_DRAW = geom. changes frequently.  This effects which types of GPU memory is used

  /////////////////////////////////
  //SETUP SHADERS				   //
  /////////////////////////////////
  GLuint shaderProgram = util::LoadShader("Shaders/phongTex.vert", "Shaders/phongTex.frag");

  //load in textures
  GLuint tex0 = util::LoadTexture("Shaders/wood.bmp");
  GLuint tex1 = util::LoadTexture("Shaders/grey_stones.bmp");

  if (tex0 == -1 || tex1 == 1 || shaderProgram == -1)
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
  glEnableVertexAttribArray(texAttrib);
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

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

	while (!quit && !complete)
	{



    glClearColor(.2f, 0.4f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram); //Set the active shader (only one can be used at a time)

    //vertex shader uniforms
		GLint uniView = glGetUniformLocation(shaderProgram, "view");
		GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
		GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");

		//build view matrix from Camera
		glm::mat4 view = glm::lookAt(
			util::vec3DtoGLM(cam->getPos()),
			util::vec3DtoGLM(cam->getPos() + cam->getDir()),  //Look at point
			util::vec3DtoGLM(cam->getUp()));

		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 proj = glm::perspective(3.14f / 4, 800.0f / 600.0f, 0.1f, 1000.0f); //FOV, aspect, near, far
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex0);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex0"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex1);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 1);

		glBindVertexArray(vao);

		//draw all WObjs
		myWorld->draw(cam, shaderProgram, uniTexID);
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
