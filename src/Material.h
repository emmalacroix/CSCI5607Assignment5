#ifndef MATERIAL_INCLUDED
#define MATERIAL_INCLUDED

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Material
{
public:
	//CONSTRUCTORS AND DESTRUCTORS
	Material();
	~Material();

	//SETTERS
	void setAmbient(glm::vec3 c);
	void setDiffuse(glm::vec3 c);
	void setSpecular(glm::vec3 c);
	void setNS(float n);
	void setIOR(float i);
	void setTransmissive(glm::vec3 c);

	//GETTERS
	glm::vec3 getAmbient();
	glm::vec3 getDiffuse();
	glm::vec3 getSpecular();
	float getNS();
	float getIOR();
	glm::vec3 getTransmissive();

private:
	glm::vec3 ambient_COLOR;
	glm::vec3 diffuse_COLOR;
	glm::vec3 specular_COLOR;
	float phong_cosine_power;
	float index_of_refraction;
	glm::vec3 transmissive_COLOR;

};

#endif