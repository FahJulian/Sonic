#version 450 core

in vec4 f_Color;
in vec2 f_TextureCoords;
in float f_TextureSlot;

in vec2 f_RectPosition;
in vec2 f_RectScale;

in float f_BorderWeight;
in vec4 f_BorderColor;

in float f_EdgeRadius;
in vec2 f_BottomLeft;
in vec2 f_BottomRight;
in vec2 f_TopLeft;
in vec2 f_TopRight;

out vec4 out_Color;

uniform sampler2D u_Textures[16];


void main()
{   
	vec2 innerPos = gl_FragCoord.xy - f_RectPosition;

	if (f_EdgeRadius != 0)
	{
		if (innerPos.x <= f_BottomLeft.x && innerPos.y <= f_BottomLeft.y)
		{
			if (distance(innerPos, f_BottomLeft) >= f_EdgeRadius)
			{
				out_Color = vec4(0.0, 0.0, 0.0, 0.0);
				return;
			}
			else if (distance(innerPos, f_BottomLeft) >= f_EdgeRadius - f_BorderWeight)
			{
				out_Color = f_BorderColor;
				return;
			}
		}

		if (innerPos.x >= f_BottomRight.x && innerPos.y <= f_BottomRight.y)
		{
			if (distance(innerPos, f_BottomRight) >= f_EdgeRadius)
			{
				out_Color = vec4(0.0, 0.0, 0.0, 0.0);
				return;
			}
			else if (distance(innerPos, f_BottomRight) >= f_EdgeRadius - f_BorderWeight)
			{
				out_Color = f_BorderColor;
				return;
			}
		}

		if (innerPos.x <= f_TopLeft.x && innerPos.y >= f_TopLeft.y)
		{
			if (distance(innerPos, f_TopLeft) >= f_EdgeRadius)
			{
				out_Color = vec4(0.0, 0.0, 0.0, 0.0);
				return;
			}
			else if (distance(innerPos, f_TopLeft) >= f_EdgeRadius - f_BorderWeight)
			{
				out_Color = f_BorderColor;
				return;
			}
		}

		if (innerPos.x >= f_TopRight.x && innerPos.y >= f_TopRight.y)
		{
			if (distance(innerPos, f_TopRight) >= f_EdgeRadius)
			{
				out_Color = vec4(0.0, 0.0, 0.0, 0.0);
				return;
			}
			else if (distance(innerPos, f_TopRight) >= f_EdgeRadius - f_BorderWeight)
			{
				out_Color = f_BorderColor;
				return;
			}
		}
	}

	if (f_BorderWeight != 0)
	{
		if (innerPos.x < f_BorderWeight || f_RectScale.x - innerPos.x < f_BorderWeight || 
			innerPos.y < f_BorderWeight || f_RectScale.y - innerPos.y < f_BorderWeight)
		{
			out_Color = f_BorderColor;
			return;
		}
	}

    int textureSlot = int(f_TextureSlot);

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
