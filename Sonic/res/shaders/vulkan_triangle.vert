#version 450 core

vec2 s_Positions[3] = {
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
};

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

layout (location = 0) out vec3 f_Color;

void main()
{
    f_Color = colors[gl_VertexIndex];
    gl_Position = vec4(s_Positions[gl_VertexIndex], 0.0, 1.0);
}
