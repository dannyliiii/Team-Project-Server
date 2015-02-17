#version 400

uniform mat4 projMatrix;
uniform vec2 textureSize;

in vec3 position;

out Vertex {
	vec2 texCoord;
} OUT;

void main(void)
{
	vec2 vertex = position.xy * textureSize;
	
	gl_Position = projMatrix * vec4(vertex, 0.0, 1.0);
	
	OUT.texCoord = position.xy;
}
