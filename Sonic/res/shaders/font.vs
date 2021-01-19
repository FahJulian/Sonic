#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec4 v_Color;
layout (location = 2) in vec2 v_TextureCoords;
layout (location = 3) in float v_FontIndex;

out vec4 f_Color;
out vec2 f_TextureCoords;
out float f_FontIndex;

uniform float u_WindowWidth;
uniform float u_WindowHeight;

void main()
{
	f_Color = v_Color;
	f_FontIndex = v_FontIndex;
	f_TextureCoords = v_TextureCoords;
	gl_Position = vec4(2 * v_Position.x / u_WindowWidth - 1, 2 * v_Position.y / u_WindowHeight - 1, 0.0, 1.0);
};
