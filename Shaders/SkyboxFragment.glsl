#version 420
uniform samplerCube cubeTex;
uniform vec3 cameraPos;

in Vertex {
	vec3 normal;
} IN;

out vec4 out_color;

void main(void){
	out_color = texture(cubeTex, normalize(IN.normal));
	
}