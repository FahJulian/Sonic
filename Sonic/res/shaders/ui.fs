#version 450 core

in vec4 f_Color;
in vec2 f_TextureCoords;
in float f_TextureSlot;
in vec2 f_RectPosition;
in vec2 f_RectScale;
in float f_BorderRadius;
in float f_BorderWidth;
in vec4 f_BorderColor;

out vec4 out_Color;

uniform sampler2D u_Textures[16];

void main()
{   
	vec2 innerPos = gl_FragCoord.xy - f_RectPosition;

	// Check if in border radius area -> make transparent
	vec2 bottomLeft = vec2(f_BorderRadius, f_BorderRadius);
	vec2 bottomRight = vec2(f_RectScale.x - f_BorderRadius, f_BorderRadius);
	vec2 topLeft = vec2(f_BorderRadius, f_RectScale.y - f_BorderRadius);
	vec2 topRight = vec2(f_RectScale.x - f_BorderRadius, f_RectScale.y - f_BorderRadius);

	if ((innerPos.x <= bottomLeft.x && innerPos.y <= bottomLeft.y && distance(innerPos, bottomLeft) >= f_BorderRadius) ||
		(innerPos.x >= bottomRight.x && innerPos.y <= bottomRight.y && distance(innerPos, bottomRight) >= f_BorderRadius) ||
		(innerPos.x <= topLeft.x && innerPos.y >= topLeft.y && distance(innerPos, topLeft) >= f_BorderRadius) ||
		(innerPos.x >= topRight.x && innerPos.y >= topRight.y && distance(innerPos, topRight) >= f_BorderRadius))
	{
		out_Color = vec4(0.0, 0.0, 0.0, 0.0);
		return;
	}

	// Check if in border area -> border color
	if (innerPos.x < f_BorderWidth || f_RectScale.x - innerPos.x < f_BorderWidth || 
		innerPos.y < f_BorderWidth || f_RectScale.y - innerPos.y < f_BorderWidth)
	{
		out_Color = f_BorderColor;
		return;
	}


    int textureSlot = int(f_TextureSlot);

	if (textureSlot == -1)
    {
        out_Color = f_Color;
        return;
    }

    switch (textureSlot)
    {
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
