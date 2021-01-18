#version 450 core

in vec4 f_Color;
in vec2 f_TextureCoords;
in float f_TextureSlot;

out vec4 out_Color;

uniform sampler2D u_Textures[16];

void main()
{
    int textureSlot = int(f_TextureSlot);

    if (textureSlot != -1)
    {
        out_Color = f_Color * vec4(1.0, 1.0, 1.0, texture(u_Textures[0], f_TextureCoords).r);
        return;
    }

    switch (textureSlot)
    {
        case -1: out_Color = f_Color; break;
        case 0: out_Color = f_Color * texture(u_Textures[0], f_TextureCoords); break;
        case 1: out_Color = f_Color * texture(u_Textures[1], f_TextureCoords); break;
        case 2: out_Color = f_Color * texture(u_Textures[2], f_TextureCoords); break;
        case 3: out_Color = f_Color * texture(u_Textures[3], f_TextureCoords); break;
        case 4: out_Color = f_Color * texture(u_Textures[4], f_TextureCoords); break;
        case 5: out_Color = f_Color * texture(u_Textures[5], f_TextureCoords); break;
        case 6: out_Color = f_Color * texture(u_Textures[6], f_TextureCoords); break;
        case 7: out_Color = f_Color * texture(u_Textures[7], f_TextureCoords); break;
        case 8: out_Color = f_Color * texture(u_Textures[8], f_TextureCoords); break;
        case 9: out_Color = f_Color * texture(u_Textures[9], f_TextureCoords); break;
        case 10: out_Color = f_Color * texture(u_Textures[10], f_TextureCoords); break;
        case 11: out_Color = f_Color * texture(u_Textures[11], f_TextureCoords); break;
        case 12: out_Color = f_Color * texture(u_Textures[12], f_TextureCoords); break;
        case 13: out_Color = f_Color * texture(u_Textures[13], f_TextureCoords); break;
        case 14: out_Color = f_Color * texture(u_Textures[14], f_TextureCoords); break;
        case 15: out_Color = f_Color * texture(u_Textures[15], f_TextureCoords); break;
    }
}
