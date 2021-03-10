#version 450 core

layout (location = 0) in vec3 f_Color;

layout (location = 0) out vec4 out_Color;

void main()
{
	out_Color = vec4(f_Color, 1.0);
}
