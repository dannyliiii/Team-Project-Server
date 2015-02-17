#version 150 core

uniform sampler2D diffuseTex;
uniform int useTexture;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 out_color;

void main(void)	{
	out_color = IN.colour;
	if(useTexture > 0) {
		out_color	*= texture(diffuseTex, IN.texCoord);
	}
}