#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec4 v_Color;
layout (location = 2) in vec2 v_TextureCoords;
layout (location = 3) in float v_TextureSlot;

out vec4 f_Color;
out vec2 f_TextureCoords;
out float f_TextureSlot;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(v_Position, 1.0);

    f_Color = v_Color;
    f_TextureCoords = v_TextureCoords;
    f_TextureSlot = v_TextureSlot;
}
