#version 400

in Vertex {
	float height;
} IN;

out float out_color;

void main(void)
{
	out_color = IN.height;
}
