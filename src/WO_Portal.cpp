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
	normal = Vec3D();
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

void WO_Portal::setNorm(Vec3D n)
{
	normal = n;
	calcModel();
}

void WO_Portal::setUp(Vec3D u)
{
	up = u;
	calcModel();
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
bool WO_Portal::doesExist()
{
	return exists;
}

Vec3D WO_Portal::getNorm()
{
	return normal;
}

Vec3D WO_Portal::getUp()
{
	return up;
}

glm::mat4 WO_Portal::getModelMat()
{
	return model;
}

/*----------------------------*/
// VIRTUALS
/*----------------------------*/
int WO_Portal::getType()
{
	return PORTAL_WOBJ;
}

//assumes that the models have already been loaded into the VBO before this call
void WO_Portal::draw(Camera* cam, GLuint shaderProgram, glm::mat4 const &viewMat, glm::mat4 const &projMat)
{
	GLint uniModel = glGetUniformLocation(shaderProgram, "model");

	glm::mat4 model;
	glm::vec3 size_v = util::vec3DtoGLM(size);
	glm::vec3 pos_v = util::vec3DtoGLM(world_pos);

	//build model mat specific to this WObj
	model = glm::translate(model, pos_v);

	//rotate assuming that it's originally facing +z
	glm::vec3 crossp = util::vec3DtoGLM(cross(normal, Vec3D(0, 0, 1)));
	glm::normalize(crossp);

	//cout << "Rotation axis: ";
	//cross(normal, Vec3D(0, 0, 1)).print();

	float angle = acos(dotProduct(Vec3D(0, 0, 1), normal));

	//cout << "Rotation angle: " << angle << endl;

	if (crossp == glm::vec3(0, 0, 0)) crossp = glm::vec3(0, 1, 0);
	model = glm::rotate(model, angle, crossp);

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
	glDrawArrays(GL_TRIANGLE_FAN, start_vertex_index, total_vertices);
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
void WO_Portal::moveTo(Vec3D pos)
{
	world_pos = pos;
	exists = true;
}

void WO_Portal::calcModel()
{
	model = glm::mat4();

	glm::vec3 size_v = util::vec3DtoGLM(size);
	glm::vec3 pos_v = util::vec3DtoGLM(world_pos);

	//build model mat specific to this WObj
	model = glm::translate(model, pos_v);

	//rotate assuming that it's originally facing +z
	glm::vec3 crossp = util::vec3DtoGLM(cross(normal, Vec3D(0, 0, 1)));
	glm::normalize(crossp);

	//cout << "Rotation axis: ";
	//cross(normal, Vec3D(0, 0, 1)).print();

	float angle = acos(dotProduct(Vec3D(0, 0, 1), normal));

	//cout << "Rotation angle: " << angle << endl;

	if (crossp == glm::vec3(0, 0, 0)) crossp = glm::vec3(0, 1, 0);
	model = glm::rotate(model, angle, crossp);

	model = glm::scale(model, size_v);
}

// Lengyel, Eric. "Oblique View Frustum Depth Projection and Clipping".
// Journal of Game Development, Vol. 1, No. 2 (2005)
// http://www.terathon.com/code/oblique.html

glm::mat4 const WO_Portal::clippedProjMat(glm::mat4 const &viewMat, glm::mat4 const &projMat) const
{
	/*float dist = glm::length(util::vec3DtoGLM(world_pos));
	glm::vec4 clipPlane(d_orientation * glm::vec3(0.0f, 0.0f, -1.0f), dist);
	clipPlane = glm::inverse(glm::transpose(viewMat)) * clipPlane;

	if (clipPlane.w > 0.0f)
		return projMat;

	glm::vec4 q = glm::inverse(projMat) * glm::vec4(
		glm::sign(clipPlane.x),
		glm::sign(clipPlane.y),
		1.0f,
		1.0f
	);

	glm::vec4 c = clipPlane * (2.0f / (glm::dot(clipPlane, q)));

	glm::mat4 newProj = projMat;
	// third row = clip plane - fourth row
	newProj = glm::row(newProj, 2, c - glm::row(newProj, 3));

	return newProj;*/

	return projMat;
}