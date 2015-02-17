#version 420
layout(binding=0) uniform sampler2D diffuseTex;
uniform vec2 pixelSize;
uniform int isVertical;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 out_color;
const float sampleDist = 1.0;
const float sampleStrength = 2.2;

void main(void){
	float samples[10] = float[](-0.08, -0.05, -0.03, -0.02, -0.01, 0.01, 0.02, 0.03, 0.05, 0.08);
	
	vec2 dir = 0.5 - IN.texCoord;
	
	float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
	dir = dir / dist;
	
	vec4 colour = texture(diffuseTex, IN.texCoord);
	
	vec4 sum = colour;
	
	for (int i = 0; i < 10; ++i) {
		sum += texture(diffuseTex, IN.texCoord + dir * samples[i] * sampleDist);
	}
	
	sum *= 1.0 / 11.0;
	
	float t = dist * sampleStrength;
	t = clamp(t, 0.0, 1.0);
	out_color = mix(colour, sum, t);
	//out_color = vec4(0,1,0,1);
}