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
	vec3 worldPos;
} OUT;

void main(void) {
	OUT.colour = colour;
	OUT.texCoord = texCoord;
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	OUT.normal = normalize(normalMatrix * normalize(normal));
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}