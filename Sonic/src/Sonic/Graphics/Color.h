#pragma once

namespace Sonic {

    using ColorCode = unsigned int;

    struct Color
    {
        float r;
        float g;
        float b;
        float a;

        Color(float r, float g, float b, float a)
            : r(r), g(g), b(b), a(a)
        {
        }

        Color(int r, int g, int b, int a)
            : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f)
        {
        }

        Color(ColorCode colorCode)
            : Color(((colorCode >> 24)&255) / 255.0f, ((colorCode >> 16)&255) / 255.0f,
                    ((colorCode >> 8)&255) / 255.0f, (colorCode&255) / 255.0f)
        {
        }
    };

    namespace Colors {

        enum : ColorCode
        {
            Transparent = 0x00000000,
            White = 0xffffffff,
            Black = 0x000000ff,
            DarkGray = 0x555555ff,
            LightGray = 0x999999ff,

            Red = 0xff0000ff,
            Green = 0x00ff00ff,
            Blue = 0x0000ffff,
            Magenta = 0xff00ffff,
            Cyan = 0x00ffffff,
            Yellow = 0xffff00ff,

            LightGreen = 0x12db13ff,
            DarkGreen = 0x084d08ff,
            LightRed = 0xf42828ff,
            DarkRed = 0x810707ff,
            LightBlue = 0x4ab8f7ff,
            DarkBlue = 0x071463ff,

            Orange = 0xff8c00ff,
        };

    }
    
}
