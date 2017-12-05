#include "WO_Portal.h"

using namespace std;

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
WO_Portal::WO_Portal()
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = Coord2D(0, 0);
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
	exists = false;
}

WO_Portal::WO_Portal(Coord2D indices)
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = indices;
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
	exists = true;
}

WO_Portal::~WO_Portal()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void WO_Portal::setExists(bool b)
{
	exists = b;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
bool WO_Portal::doesExist()
{
	return exists;
}

/*----------------------------*/
// VIRTUALS
/*----------------------------*/
int WO_Portal::getType()
{
	return PORTAL_WOBJ;
}

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
// OTHERS
/*----------------------------*/
void WO_Portal::moveTo(Coord2D indices)
{
	world_indices = indices;
}