#include <gl/glew.h>
#include "Sonic/Log/Log.h"
#include "Font.h"
#include "FontRenderer.h"


unsigned int shader;
unsigned int vao;
unsigned int vbo;


namespace Sonic {

	namespace FontRenderer {

		void init()
		{
			const char* vertex =
				"#version 450 core\n"
				"layout (location = 0) in vec4 vertex;\n"
				"out vec2 f_v;\n"
				"void main()\n"
				"{\n"
				"f_v = vertex.zw;\n"
				"gl_Position = vec4(vertex.x / 250 - 1, vertex.y / 250 - 1, 0.0, 1.0);\n"
				"}\n";

			const char* frag =
				"#version 450 core\n"
				"in vec2 f_v;\n"
				"out vec4 color;\n"
				"uniform sampler2D texture;"
				"void main()\n"
				"{\n"
				"if (f_v.x == 3.0)\n"
				"color = vec4(1.0, 0.0, 1.0, 1.0);\n"
				"else\n"
				"color = vec4(1.0, 0.0, 1.0, texture(texture, f_v).r);\n"
				"}\n";

			vao = 0;
			glCreateVertexArrays(1, &vao);
			glBindVertexArray(vao);
			vbo = 0;
			glCreateBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glEnableVertexAttribArray(0);

			shader = glCreateProgram();
			unsigned int v = glCreateShader(GL_VERTEX_SHADER);
			unsigned int f = glCreateShader(GL_FRAGMENT_SHADER);
			int vl = static_cast<int>(std::string(vertex).length());
			int fl = static_cast<int>(std::string(frag).length());
			glShaderSource(v, 1, &vertex, &vl);
			glShaderSource(f, 1, &frag, &fl);
			glCompileShader(v);
			glCompileShader(f);
			glAttachShader(shader, v);
			glAttachShader(shader, f);
			glLinkProgram(shader);	
		}

		void drawCharacter(float x, float y, unsigned char c, const Font& font)
		{
 			Character character = font.GetCharacter(c);

			float vertices[]
			{
				x, y, 0.0f, 1.0f,
				x + character.width, y, 1.0f, 1.0f,
				x, y + character.height, 0.0f, 0.0f,

				x + character.width, y, 1.0f, 1.0f,
				x + character.width, y + character.height, 1.0f, 0.0f,
				x, y + character.height, 0.0f, 0.0f,
			};

			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glUseProgram(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, character.textureID);

			glDrawArrays(GL_TRIANGLES, 0, 12);
		}

		void drawString(float x, float y, const std::string& string, const Font& font)
		{
			int size = string.size();
			float kerning = 0.0f;
			for (int i = 0; i < size; i++)
			{
				char c = string[i];
				Character ch = font.GetCharacter(c);

				drawCharacter(x + ch.bearingX + kerning, y - (ch.height - ch.bearingY), c, font);
				x += ch.advanceX + kerning;

				if (i + 1 < size)
					kerning = font.GetKerning(string[i], string[i + 1]);
			}
		}

	}

}
