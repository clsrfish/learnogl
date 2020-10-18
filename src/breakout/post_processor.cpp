#include "./post_processor.hpp"
#include <glad/glad.h>
#include "../utils/log.h"

breakout::PostProcessor::PostProcessor(unsigned int width, unsigned int height, Shader &shader)
    : effectShader(shader), width(width), height(height), Confuse(false), Chaos(false), Shake(false), texture()
{
    glGenFramebuffers(1, &this->MSFBO);
    glGenFramebuffers(1, &this->FBO);
    glGenRenderbuffers(1, &this->RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_E("Failed to initialize MSFBO");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    this->texture.Generate(width, height, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture.ID, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_E("Failed to initialize MSFBO");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->initRenderData();
    this->effectShader.SetInteger("scene", 0, true);
    float offset = 1.0f / 300.0f;
    float offsets[9][2] = {
        {-offset, offset},  // top-left
        {0.0f, offset},     // top-center
        {offset, offset},   // top-right
        {-offset, 0.0f},    // center-left
        {0.0f, 0.0f},       // center-center
        {offset, 0.0f},     // center - right
        {-offset, -offset}, // bottom-left
        {0.0f, -offset},    // bottom-center
        {offset, -offset}   // bottom-right
    };
    glUniform2fv(glGetUniformLocation(this->effectShader.ID, "offset"), 9, reinterpret_cast<float *>(offsets));
    int edge_kernel[9] = {
        -1, -1, -1,
        -1, 8, -1,
        -1, -1, -1};
    glUniform1iv(glGetUniformLocation(this->effectShader.ID, "edge_kernel"), 9, edge_kernel);
    float blur_kernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f};
    glUniform1fv(glGetUniformLocation(this->effectShader.ID, "blur_kernel"), 9, blur_kernel);
}

breakout::PostProcessor::~PostProcessor()
{
}

void breakout::PostProcessor::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f};
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void breakout::PostProcessor::BeginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void breakout::PostProcessor::EndRender()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
    glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void breakout::PostProcessor::Render(float time)
{
    this->effectShader.Use();
    this->effectShader.SetFloat("time", time);
    this->effectShader.SetInteger("confuse", this->Confuse);
    this->effectShader.SetInteger("chaos", this->Chaos);
    this->effectShader.SetInteger("shake", this->Shake);

    glActiveTexture(GL_TEXTURE0);
    this->texture.Bind();
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}