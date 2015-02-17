#version 420

#ifdef _VERT_

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

#endif

#ifdef _FRAG_

layout(binding=0) uniform sampler2D diffuseTex;
layout(binding=1) uniform sampler2D diffuseTexx;
in Vertex {
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
} IN;

out vec4 gl_FragColor;

void main(void){
	vec4 a = texture(diffuseTex, IN.texCoord);
	vec4 b = texture(diffuseTexx, IN.texCoord);
	gl_FragColor = mix(a,b,0.5);
}

#endif