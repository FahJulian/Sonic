#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec4 v_Color;
layout (location = 2) in vec2 v_TextureCoords;
layout (location = 3) in float v_TextureSlot;

layout (location = 4) in vec2 v_RectPosition;
layout (location = 5) in vec2 v_RectScale;
layout (location = 6) in float v_BorderRadius;
layout (location = 7) in float v_BorderWidth;
layout (location = 8) in vec4 v_BorderColor;

out vec4 f_Color;
out vec2 f_TextureCoords;
out float f_TextureSlot;
out vec2 f_RectPosition;
out vec2 f_RectScale;
out float f_BorderRadius;
out float f_BorderWidth;
out vec4 f_BorderColor;

uniform float u_WindowWidth;
uniform float u_WindowHeight;

void main()
{
	gl_Position = vec4(2 * v_Position.x / u_WindowWidth - 1.0, 2 * v_Position.y /u_WindowHeight - 1.0, v_Position.z, 1-0);

	f_Color = v_Color;
	f_TextureCoords = v_TextureCoords;
	f_TextureSlot = v_TextureSlot;
	f_RectPosition = v_RectPosition;
	f_RectScale = v_RectScale;
	f_BorderRadius = v_BorderRadius;
	f_BorderWidth = v_BorderWidth;
	f_BorderColor = v_BorderColor;
}
