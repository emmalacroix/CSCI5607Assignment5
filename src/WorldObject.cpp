#include "WorldObject.h"

using namespace std;

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
WorldObject::WorldObject()
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = Coord2D(0, 0);
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
}

WorldObject::WorldObject(Coord2D indices)
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = indices;
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
}

WorldObject::~WorldObject()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void WorldObject::setWPosition(Vec3D p)
{
	world_pos = p;
}

void WorldObject::setWIndices(Coord2D i)
{
	world_indices = i;
}

void WorldObject::setVertStartIndex(int i)
{
	start_vertex_index = i;
}

void WorldObject::setTotalVertices(int i)
{
	total_vertices = i;
}

void WorldObject::setMaterial(Material m)
{
	mat = m;
}

void WorldObject::setSize(Vec3D s)
{
	size = s;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
Vec3D WorldObject::getWPosition()
{
	return world_pos;
}

Coord2D WorldObject::getWIndices()
{
	return world_indices;
}

int WorldObject::getStartIndex()
{
	return start_vertex_index;
}

int WorldObject::getTotalVertices()
{
	return total_vertices;
}

Material WorldObject::getMaterial()
{
	return mat;
}

Vec3D WorldObject::getSize()
{
	return size;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
//assumes that the models have already been loaded into the VBO before this call
void WorldObject::draw(Camera* cam, GLuint shaderProgram)
{
	GLint uniModel = glGetUniformLocation(shaderProgram, "model");

	glm::mat4 model;
	glm::vec3 size_v = util::vec3DtoGLM(size);
	glm::vec3 pos_v = util::vec3DtoGLM(world_pos);

	//build model mat specific to this WObj
	model = glm::translate(model, pos_v);
	model = glm::scale(model, size_v);
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	//fragment shader uniforms (from Material)
	GLint uniform_ka = glGetUniformLocation(shaderProgram, "ka");
	GLint uniform_kd = glGetUniformLocation(shaderProgram, "kd");
	GLint uniform_ks = glGetUniformLocation(shaderProgram, "ks");
	GLint uniform_s = glGetUniformLocation(shaderProgram, "s");

	glm::vec3 mat_AMB = mat.getAmbient();
	glUniform3f(uniform_ka, mat_AMB[0], mat_AMB[1], mat_AMB[2]);

	glm::vec3 mat_DIF = mat.getDiffuse();
	glUniform3f(uniform_kd, mat_DIF[0], mat_DIF[1], mat_DIF[2]);

	glm::vec3 mat_SPEC = mat.getSpecular();
	glUniform3f(uniform_ks, mat_SPEC[0], mat_SPEC[1], mat_SPEC[2]);

	glUniform1f(uniform_s, mat.getNS());

	//starts at an offset of start_vertex_index
	//(Primitive Type, Start Vertex, End Vertex)
	glDrawArrays(GL_TRIANGLES, start_vertex_index, total_vertices);
}

/*----------------------------*/
// VIRTUALS
/*----------------------------*/
int WorldObject::getType()
{
	return EMPTY_WOBJ;
}
