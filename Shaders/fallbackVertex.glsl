#version 150

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec2 texCoord;
in vec4 colour;
in vec3 normal;

out Vertex {
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
} OUT;

void main(void) {
	OUT.colour = colour;
	OUT.texCoord = texCoord;
	OUT.normal = normal;
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}