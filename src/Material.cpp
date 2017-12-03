#include "Material.h"


/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
Material::Material()
{
	ambient_COLOR = glm::vec3(0, 0, 0);
	diffuse_COLOR = glm::vec3(1, 1, 1);
	specular_COLOR = glm::vec3(0, 0, 0);
	phong_cosine_power = 5.0;
	index_of_refraction = 1.0;
	transmissive_COLOR = glm::vec3();
}


Material::~Material()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void Material::setAmbient(glm::vec3 c)
{
	ambient_COLOR = c;
}

void Material::setDiffuse(glm::vec3 c)
{
	diffuse_COLOR = c;
}

void Material::setSpecular(glm::vec3 c)
{
	specular_COLOR = c;
}

void Material::setNS(float n)
{
	phong_cosine_power = n;
}

void Material::setIOR(float i)
{
	index_of_refraction = i;
}

void Material::setTransmissive(glm::vec3 c)
{
	transmissive_COLOR = c;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
glm::vec3 Material::getAmbient()
{
	return ambient_COLOR;
}

glm::vec3 Material::getDiffuse()
{
	return diffuse_COLOR;
}

glm::vec3 Material::getSpecular()
{
	return specular_COLOR;
}

float Material::getNS()
{
	return phong_cosine_power;
}

float Material::getIOR()
{
	return index_of_refraction;
}

glm::vec3 Material::getTransmissive()
{
	return transmissive_COLOR;
}
