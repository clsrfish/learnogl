#include "./text_renderer.hpp"
#include <glad/glad.h>
#include "../utils/log.h"
#include "./resource_manager.hpp"

breakout::TextRenderer::TextRenderer()
{
    loadVertices();
    loadCharacters();
}

breakout::TextRenderer::~TextRenderer()
{
}

void breakout::TextRenderer::loadVertices()
{
    glGenVertexArrays(1, &textVAO);
    glBindVertexArray(textVAO);
    glGenBuffers(1, &textVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void breakout::TextRenderer::loadCharacters()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        LOG_E("Could not init FreeType Library");
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, "/System/Library/Fonts/Supplemental/Arial.ttf", 0, &face))
    {
        LOG_E("Failed to load font");
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned int i = 0; i < 128; i++)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            LOG_W("Failed to load Glyph: %c", i);
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     face->glyph->bitmap.width, face->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)};
        characters.insert(std::pair<char, Character>(i, character));
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void breakout::TextRenderer::RenderText(const std::string &text, float x, float y, float scale, glm::vec3 color)
{
    breakout::Shader shader = breakout::ResourceManager::GetShader("text");
    shader.Use();
    shader.SetVector3f("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->textVAO);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); ++c)
    {
        Character ch = characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f},
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}