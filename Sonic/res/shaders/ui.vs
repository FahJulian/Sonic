#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec4 v_Color;
layout (location = 2) in vec2 v_TextureCoords;
layout (location = 3) in float v_TextureSlot;

layout (location = 4) in vec2 v_RectPosition;
layout (location = 5) in vec2 v_RectScale;
layout (location = 6) in vec4 v_BorderColor;
layout (location = 7) in float v_BorderWeight;
layout (location = 8) in float v_EdgeRadius;

out vec4 f_Color;
out vec2 f_TextureCoords;
out float f_TextureSlot;

out vec2 f_RectPosition;
out vec2 f_RectScale;

out float f_BorderWeight;
out vec4 f_BorderColor;

out float f_EdgeRadius;
out vec2 f_BottomLeft;
out vec2 f_BottomRight;
out vec2 f_TopLeft;
out vec2 f_TopRight;

uniform float u_ViewportWidth;
uniform float u_ViewportHeight;

void main()
{
	gl_Position = vec4(2 * v_Position.x / u_ViewportWidth - 1.0, 2 * v_Position.y /u_ViewportHeight - 1.0, v_Position.z, 1-0);

	f_Color = v_Color;
	f_TextureCoords = v_TextureCoords;
	f_TextureSlot = v_TextureSlot;

	f_RectPosition = v_RectPosition;
	f_RectScale = v_RectScale;

	if (v_BorderWeight != 0.0)
	{
		f_BorderColor = v_BorderColor;
		f_BorderWeight = v_BorderWeight;
	}

	if (v_EdgeRadius != 0.0)
	{
		f_EdgeRadius = v_EdgeRadius;
		f_BottomLeft = vec2(v_EdgeRadius, v_EdgeRadius);
		f_BottomRight = vec2(v_RectScale.x - v_EdgeRadius, v_EdgeRadius);
		f_TopLeft = vec2(v_EdgeRadius, v_RectScale.y - v_EdgeRadius);
		f_TopRight = vec2(v_RectScale.x - v_EdgeRadius, v_RectScale.y - v_EdgeRadius);
	}
}
