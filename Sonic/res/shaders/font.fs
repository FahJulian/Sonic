#version 450 core

in vec4 f_Color;
in vec2 f_TextureCoords;
in float f_FontIndex;

out vec4 out_Color;

uniform sampler2D textures[16];

void main()
{
	int fontIndex = int(f_FontIndex);

	switch (fontIndex)
	{
		case 0: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[0], f_TextureCoords).r); break;
		case 1: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[1], f_TextureCoords).r); break;
		case 2: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[2], f_TextureCoords).r); break;
		case 3: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[3], f_TextureCoords).r); break;
		case 4: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[4], f_TextureCoords).r); break;
		case 5: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[5], f_TextureCoords).r); break;
		case 6: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[6], f_TextureCoords).r); break;
		case 7: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[7], f_TextureCoords).r); break;
		case 8: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[8], f_TextureCoords).r); break;
		case 9: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[9], f_TextureCoords).r); break;
		case 10: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[10], f_TextureCoords).r); break;
		case 11: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[11], f_TextureCoords).r); break;
		case 12: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[12], f_TextureCoords).r); break;
		case 13: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[13], f_TextureCoords).r); break;
		case 14: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[14], f_TextureCoords).r); break;
		case 15: out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(textures[15], f_TextureCoords).r); break;
	}
};
