#version 150 core

// Lighting
const vec3 Color = vec3(0.5,0.5,0.5);

// Material parameters
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

in vec3 normal;
in vec3 pos;
in vec3 lightDir;

out vec4 outColor;

//const vec3 lightDir = vec3(0,1,1);

void main()
{
	vec3 diffuseC = Color*kd*max(dot(-lightDir, normal), 0);
	vec3 ambC = Color*ka;

	//specular component
	vec3 h = normalize(lightDir - pos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 viewDir = normalize(-pos);
	float spec = max(dot(h, normal),0.0);
	
	if (dot(-lightDir,normal) <= 0.0) spec = 0;
	vec3 specC = Color*ks*pow(spec,s);

	outColor = vec4(diffuseC + ambC + specC, 1.0);
}