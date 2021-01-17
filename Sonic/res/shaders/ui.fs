#version 450 core

in vec4 f_Color;
in vec2 f_TextureCoords;
in float f_TextureSlot;
in vec2 f_RectPosition;
in vec2 f_RectScale;
in vec4 f_BorderColor;
in float f_BorderWidth;
in float f_EdgeRadius;

out vec4 out_Color;

uniform sampler2D u_Textures[16];

void main()
{   
	vec2 innerPos = gl_FragCoord.xy - f_RectPosition;

	// Check if in border radius area -> make transparent
	vec2 bottomLeft = vec2(f_EdgeRadius, f_EdgeRadius);
	vec2 bottomRight = vec2(f_RectScale.x - f_EdgeRadius, f_EdgeRadius);
	vec2 topLeft = vec2(f_EdgeRadius, f_RectScale.y - f_EdgeRadius);
	vec2 topRight = vec2(f_RectScale.x - f_EdgeRadius, f_RectScale.y - f_EdgeRadius);

	if (innerPos.x <= bottomLeft.x && innerPos.y <= bottomLeft.y)
	{
		if (distance(innerPos, bottomLeft) >= f_EdgeRadius)
		{
			out_Color = vec4(0.0, 0.0, 0.0, 0.0);
			return;
		}
		else if (distance(innerPos, bottomLeft) >= f_EdgeRadius - f_BorderWidth)
		{
			out_Color = f_BorderColor;
			return;
		}
	}

	if (innerPos.x >= bottomRight.x && innerPos.y <= bottomRight.y)
	{
		if (distance(innerPos, bottomRight) >= f_EdgeRadius)
		{
			out_Color = vec4(0.0, 0.0, 0.0, 0.0);
			return;
		}
		else if (distance(innerPos, bottomRight) >= f_EdgeRadius - f_BorderWidth)
		{
			out_Color = f_BorderColor;
			return;
		}
	}

	if (innerPos.x <= topLeft.x && innerPos.y >= topLeft.y)
	{
		if (distance(innerPos, topLeft) >= f_EdgeRadius)
		{
			out_Color = vec4(0.0, 0.0, 0.0, 0.0);
			return;
		}
		else if (distance(innerPos, topLeft) >= f_EdgeRadius - f_BorderWidth)
		{
			out_Color = f_BorderColor;
			return;
		}
	}

	if (innerPos.x >= topRight.x && innerPos.y >= topRight.y)
	{
		if (distance(innerPos, topRight) >= f_EdgeRadius)
		{
			out_Color = vec4(0.0, 0.0, 0.0, 0.0);
			return;
		}
		else if (distance(innerPos, topRight) >= f_EdgeRadius - f_BorderWidth)
		{
			out_Color = f_BorderColor;
			return;
		}
	}

	if (innerPos.x < f_BorderWidth || f_RectScale.x - innerPos.x < f_BorderWidth || 
		innerPos.y < f_BorderWidth || f_RectScale.y - innerPos.y < f_BorderWidth)
	{
		out_Color = f_BorderColor;
		return;
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
