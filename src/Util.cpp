#include "Util.h"

using namespace std;

/*--------------------------------------------------------------*/
// loadModel : loads specified model file into float* (vert array)
//				returns number of triangles
/*--------------------------------------------------------------*/
float* util::loadModel(string filename, int& num_verts)
{
	ifstream modelFile;
	modelFile.open(filename);

	if (modelFile.fail())
	{
		cout << "\nCan't load model file '" << filename << "'" << endl;
		printf(strerror(errno));
		return 0;
	}

	int numLines = 0;
	modelFile >> numLines; //first number in the model file is the number of lines

	float* m_array = new float[numLines];
	for (int i = 0; i < numLines; i++) {
		modelFile >> m_array[i];
	}

	cout << "\nLoaded model file " << filename << " successfully." << endl;

	printf("Lines : %d\n\n", numLines);
	num_verts = numLines / 8;
	modelFile.close();

	return m_array;
}

/*--------------------------------------------------------------*/
// vec3DtoGLM : builds glm::vec from Vec3D
/*--------------------------------------------------------------*/
glm::vec3 util::vec3DtoGLM(Vec3D v)
{
	return glm::vec3(v.getX(), v.getY(), v.getZ());
}

/*--------------------------------------------------------------*/
// readFile : builds string out of shader file
// copied from:
// http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/
/*--------------------------------------------------------------*/
std::string readFile(const char *filePath)
{
	printf("Parsing shader file %s\n", filePath);

	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

/*--------------------------------------------------------------*/
// LoadShader : builds shaderProgram out of given shader files
// adapted from:
// http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/
/*--------------------------------------------------------------*/
GLuint util::LoadShader(const char *vertex_path, const char *fragment_path)
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shaders
	std::string vertShaderStr = readFile(vertex_path);
	std::string fragShaderStr = readFile(fragment_path);
	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	GLint result = GL_FALSE;
	int logLength;

	// Compile vertex shader
	std::cout << "Compiling vertex shader." << std::endl;
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	//Double check the vertex shader compiled 
	GLint status;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	if (!status) {
		char buffer[512];
		glGetShaderInfoLog(vertShader, 512, NULL, buffer);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Compilation Error",
			"Failed to Compile: Check Consol Output.",
			NULL);
		printf("\nVertex Shader Compile Failed. Info:\n\n%s\n", buffer);
		return -1;
	}

	// Compile fragment shader
	std::cout << "Compiling fragment shader." << std::endl;
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	//Double check the fragment shader compiled 
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if (!status) {
		char buffer[512];
		glGetShaderInfoLog(fragShader, 512, NULL, buffer);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Compilation Error",
			"Failed to Compile: Check Consol Output.",
			NULL);
		printf("\nFragment Shader Compile Failed. Info:\n\n%s\n", buffer);
		return -1;
	}

	std::cout << "Linking program." << std::endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
	std::cout << &programError[0] << std::endl;

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}

/*--------------------------------------------------------------*/
// LoadTexture : 
/*--------------------------------------------------------------*/
GLuint util::LoadTexture(const char * texFile)
{
	printf("Loading %s texture.\n", texFile);
	SDL_Surface* surface = SDL_LoadBMP(texFile);
	if (surface == NULL) { //If it failed, print the error
		printf("Error: \"%s\"\n", SDL_GetError()); return 1;
	}
	GLuint tex;
	glGenTextures(1, &tex);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	//What to do outside 0-1 range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load the texture into memory
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);


	SDL_FreeSurface(surface);
	
	return tex;
}

/*--------------------------------------------------------------*/
// commaSplit : 
/*--------------------------------------------------------------*/
vector<string> util::commaSplit(string line)
{
	vector<string> split_str = vector<string>();
	string temp = "";

	for (int i = 0; i < line.length(); i++)
	{
		if (line[i] != ',') temp += line[i];
		else
		{
			//cout << "adding : " << temp << endl;
			split_str.push_back(temp);
			temp = "";
		}
	}
	//make sure to get rest after the last comma
	split_str.push_back(temp);
	return split_str;
}
