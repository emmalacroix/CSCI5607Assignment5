#version 150 core

in vec3 position;
in vec3 inNormal;

out vec3 normal;
out vec3 pos;
out vec3 lightDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

const vec3 inLightDir = normalize(vec3(-1,1,-1));

void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0);
	vec4 norm4 = transpose(inverse(view*model)) * vec4(inNormal, 0.0);
	normal = normalize(norm4.xyz);
	pos = (view * model * vec4(position,1.0)).xyz;

	lightDir = (view * vec4(inLightDir,0.0)).xyz; //It's a vector!
}