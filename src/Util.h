#ifndef UTIL_INCLUDED
#define UTIL_INCLUDED

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

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "Vec3D.h"

namespace util
{
	//returns ptr to array holding all vertex data flattened
	//stores number of vertices within ref param num_verts
	float* loadModel(std::string filename, int& num_verts);

	//converts Vec3D to glm vec3
	glm::vec3 vec3DtoGLM(Vec3D v);

	//copied from:
	//http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/
	GLuint LoadShader(const char *vertex_path, const char *fragment_path);

	GLuint LoadTexture(const char* texFile);

	//split a comma-separated string into substrings
	std::vector<std::string> commaSplit(std::string line);

	//plane intersection
	const int kEpsilon = 1e-6; //arbitrary small int for plane intersection
	//bool findIntersectionPoint(Vec3D origin, Vec3D dir, Vec3D face_norm, Vec3D face_pt, Intersection& iSect);

}

#endif
