#version 450 core

in vec4 f_Color;
in vec2 f_TextureCoords;
in float f_FontIndex;

out vec4 out_Color;

uniform sampler2D u_Textures[16];

void main()
{
	int fontIndex = int(f_FontIndex);

	switch (fontIndex)
	{
		case 0: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[0], f_TextureCoords).r); break;
		case 1: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[1], f_TextureCoords).r); break;
		case 2: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[2], f_TextureCoords).r); break;
		case 3: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[3], f_TextureCoords).r); break;
		case 4: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[4], f_TextureCoords).r); break;
		case 5: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[5], f_TextureCoords).r); break;
		case 6: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[6], f_TextureCoords).r); break;
		case 7: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[7], f_TextureCoords).r); break;
		case 8: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[8], f_TextureCoords).r); break;
		case 9: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[9], f_TextureCoords).r); break;
		case 10: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[10], f_TextureCoords).r); break;
		case 11: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[11], f_TextureCoords).r); break;
		case 12: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[12], f_TextureCoords).r); break;
		case 13: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[13], f_TextureCoords).r); break;
		case 14: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[14], f_TextureCoords).r); break;
		case 15: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[15], f_TextureCoords).r); break;
	}
};
