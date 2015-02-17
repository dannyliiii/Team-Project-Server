#version 150
uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
} IN;

out vec4 out_color;

void main(void){
	out_color = texture(diffuseTex, IN.texCoord);
}