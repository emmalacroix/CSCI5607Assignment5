#version 150 core

in vec3 Color;
in vec3 normal;
in vec3 pos;
in vec3 lightDir;
in vec2 texcoord;

out vec4 outColor;

uniform sampler2D tex;


const float ambient = .3;
void main() {
   vec3 Color = texture(tex, texcoord).rgb;
   //vec3 Color = vec3(.2f,.2f,.6f);
   vec3 diffuseC = Color*max(dot(-lightDir,normal),0.0);
   vec3 ambC = Color*ambient;
   vec3 viewDir = normalize(-pos); //We know the eye is at (0,0)!
   vec3 reflectDir = reflect(viewDir,normal);
   float spec = max(dot(reflectDir,lightDir),0.0);
   if (dot(-lightDir,normal) <= 0.0)spec = 0;
   vec3 specC = .8*vec3(1.0,1.0,1.0)*pow(spec,4);
   vec3 oColor = ambC+diffuseC+specC;
   outColor = vec4(oColor,1);
}