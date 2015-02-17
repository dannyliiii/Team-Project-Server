#version 400

uniform mat4 projMatrix;

// TODO: 
// Use transform and proper offset in the proper x direction!

uniform vec2 splineCoords[2];
uniform float splineHeights[2];
uniform float splineWidth[2];

in vec3 position;

out Vertex {
	float height;
} OUT;

void main(void)
{
	int vIndex = int(position.y);
	
	vec2 pos = splineCoords[vIndex];
	float width = splineWidth[vIndex];
	
	pos.x += (position.x * 2.0 - 1.0) * width;
	
	gl_Position = projMatrix * vec4(pos, 0.f, 1.0);
	
	OUT.height = mix(splineHeights[0], splineHeights[1], position.y);
}
