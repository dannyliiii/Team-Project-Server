#version 150

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec2 texCoord;
in vec4 colour;

out Vertex {
	vec2 texCoord;
	vec4 colour;
} OUT;

void main(void) {
	gl_Position = vec4(position.xy, 0.0, 1.0);
	gl_Position = sign(gl_Position);
	OUT.texCoord = (vec2(gl_Position.x, -gl_Position.y) + vec2(1.0)) / vec2(2.0);
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}