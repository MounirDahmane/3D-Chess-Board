#pragma once
#include <iostream>
#include "ShaderClass.h"
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H


class Font
{
public:
    Font()  { Char_loader();}
	~Font() {}

    void RenderText(MyShader& shader, std::string text, float x, float y, float scale, glm::vec3 color, unsigned int& VAO, unsigned int& VBO)
    {
        // activate corresponding render state	
        shader.use();
        glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    
    FT_Library ft_m;
    FT_Face face_m;

    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };
    std::map<char, Character> Characters;
    
    int Char_loader()
    {
        if (FT_Init_FreeType(&(this->ft_m)))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl; 
            return -1;   
        }
        if (FT_New_Face(this->ft_m, "resources/Fonts/Antonio-Bold.ttf", 0, &(this->face_m)))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            return -1;
        }

        else {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(this->face_m, 0, 48); // Setting the width to 0 lets the face dynamically calculate the width based on the given height

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph 
                if (FT_Load_Char(this->face_m, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->face_m->glyph->bitmap.width, this->face_m->glyph->bitmap.rows, 
                                0,GL_RED,GL_UNSIGNED_BYTE,this->face_m->glyph->bitmap.buffer);
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                Character character = {
                    texture,
                    glm::ivec2(this->face_m->glyph->bitmap.width, this->face_m->glyph->bitmap.rows),
                    glm::ivec2(this->face_m->glyph->bitmap_left, this->face_m->glyph->bitmap_top),
                    static_cast<unsigned int>(this->face_m->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
       // FT_Done_FreeType(this->ft_m);
       // FT_Done_Face(this->face_m);
        return 1;
    }

};


