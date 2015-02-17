#version 400

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform vec2 chunkOrigin;
uniform vec2 chunkSize;

uniform sampler2D heightTex;
uniform float terrainBaseHeight;
uniform float terrainScale;

in vec3 position;
in vec2 texCoord;

out Vertex {
	vec2 texCoord;
} OUT;

void main(void)
{
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	
	float height = terrainBaseHeight + texture(heightTex, texCoord).r * terrainScale;
	
	vec2 pos = chunkOrigin + position.xz * chunkSize;
	gl_Position = mvp * vec4(vec3(pos.x, height, pos.y), 1.0);
	
	// N.B. Not the input texture coords, which are used for the heightmap lookup.
	OUT.texCoord = vec2(position.x, -position.z);
}
