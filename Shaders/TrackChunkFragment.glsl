#version 400

uniform sampler2D heightTex;
uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 out_color;

void main(void)
{
	out_color = texture(diffuseTex, IN.texCoord);
	
	//out_color = vec4(vec3(texture(heightTex, IN.texCoord).r), 1.0);
	//out_color = vec4(vec3(IN.texCoord.x), 1.0);
}