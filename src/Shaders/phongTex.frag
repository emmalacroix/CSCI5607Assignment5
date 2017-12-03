#version 150 core

// Lighting
const vec3 Color = vec3(0.5,0.5,0.5);

// Material parameters
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

//texture parameters
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform int texID;

in vec3 normal;
in vec3 pos;
in vec3 lightDir;
in vec2 texcoord;

out vec4 outColor;

//const vec3 lightDir = vec3(0,1,1);

void main()
{
	vec3 color;

	if (texID == -1) //no texture
			color = Color;
	else if (texID == 0)
		color = texture(tex0, texcoord).rgb;
	else if (texID == 1)
		color = texture(tex1, texcoord).rgb;
	else
	{
		outColor = vec4(1,0,0,1);
   	 	return; //This was an error, stop lighting!
	}
	
	vec3 diffuseC = color*kd*max(dot(-lightDir, normal), 0);
	vec3 ambC = color*ka;

	//specular component
	vec3 h = normalize(lightDir - pos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 viewDir = normalize(-pos);
	float spec = max(dot(h, normal),0.0);
	
	if (dot(-lightDir,normal) <= 0.0) spec = 0;
	vec3 specC = color*ks*pow(spec,s);

	outColor = vec4(diffuseC + ambC + specC, 1.0);
}