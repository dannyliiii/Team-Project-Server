#version 420
layout(binding=0) uniform sampler2D diffuseTex;
layout(binding=1) uniform sampler2D diffuseTexx;
in Vertex {
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
} IN;

out vec4 out_color;

void main(void){
	vec4 a = texture(diffuseTex, IN.texCoord);
	vec4 b = texture(diffuseTexx, IN.texCoord);
	out_color = mix(a,b,0.5);
}