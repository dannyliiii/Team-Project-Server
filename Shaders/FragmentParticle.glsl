#version 150 core

uniform sampler2D diffuseTex;

in Vertex	{
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 out_color;

void main(void)	{
	vec4 final_color = IN.colour* texture(diffuseTex, IN.texCoord);
	if(final_color.a == 0.0f){
		discard;
	} else {
		out_color = final_color;
	}
	
}
