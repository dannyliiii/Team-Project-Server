#version 420
layout(binding=0) uniform sampler2D diffuseTex;
uniform vec2 pixelSize;
uniform int isVertical;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 out_color;
const float weights[5] = float[](0.12, 0.22, 0.32, 0.22, 0.12);

void main(void){
	vec2 values[5];
	if (isVertical == 1) {
		values = vec2[](vec2(-pixelSize.y*3, 0.0), vec2(-pixelSize.y*2, 0.0), vec2(pixelSize.y, 0.0), vec2(pixelSize.y*2, 0.0), vec2(pixelSize.y*3, 0.0));
	}
	else {
		values = vec2[](vec2(-pixelSize.x*3, 0.0), vec2(-pixelSize.x*2, 0.0), vec2(pixelSize.x, 0.0), vec2(pixelSize.x*2, 0.0), vec2(pixelSize.x*3, 0.0));
	}
	
	for (int i = 0; i < 5; ++i) {
		vec4 tmp = texture(diffuseTex, IN.texCoord.xy + values[i]);
		out_color += tmp * weights[i];
	}
	//out_color = vec4(0,1,0,1);
}