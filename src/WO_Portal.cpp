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

/*----------------------------*/
// VIRTUALS
/*----------------------------*/
int WO_Portal::getType()
{
	return PORTAL_WOBJ;
}

//assumes that the models have already been loaded into the VBO before this call
void WO_Portal::draw(Camera* cam, GLuint shaderProgram)
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

	if (angle != 0.0) model = glm::rotate(model, angle, crossp);

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

bool WO_Portal::getIntersection(Vec3D origin, Vec3D dir, Intersection& iSect)
{
	if (findPlaneIntersection(origin, dir, normal, world_pos, iSect))
	{
		//check to see if intersection pt is within portal bounds
		Vec3D inter_pt = iSect.getPoint();



	}

	return false;
}

bool WO_Portal::findPlaneIntersection(Vec3D origin, Vec3D dir, Vec3D face_norm, Vec3D face_pt, Intersection & iSect)
{
	//algorithm below is a modification of the one described by:
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection

	float denom = dotProduct(dir, face_norm);

	if (denom > kEpsilon) //save time; don't worry about really small numbers
	{
		Vec3D dist = face_pt - origin;
		float t = dotProduct(dist, face_norm) / denom;

		bool result = (t >= 0);

		if (result)
		{
			//store intersection point (need this for Triangle)
			iSect.setPoint(origin + t*dir);
			return true;
		}
	}
	return false;
}