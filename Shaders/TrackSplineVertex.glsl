#version 400

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

// TODO: 
// Use transform and proper offset in the proper x direction!

uniform vec3 splinePoint[2];
uniform float splineWidth[2];

// TODO:
// Hopefully decrease this value once other things are done...
const float splineYOffset = 1.0;

in vec3 position;

out Vertex {
	vec2 texCoord;
} OUT;

void main(void)
{
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	
	int vIndex = int(position.y);
	
	vec3 pos = splinePoint[vIndex];
	float width = splineWidth[vIndex];
	
	pos.x += (position.x * 2.0 - 1.0) * width;
	
	pos.y += splineYOffset;
	
	gl_Position = mvp * vec4(pos, 1.0);
}
