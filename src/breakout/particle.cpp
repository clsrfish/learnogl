#include <glad/glad.h>
#include "./particle.hpp"
#include "./resource_manager.hpp"
#include "../gl/gl_utils.h"

breakout::Particle::Particle(/* args */)
    : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(1.0f)
{
}

breakout::Particle::~Particle()
{
}

breakout::ParticleEmmiter::ParticleEmmiter(breakout::Shader &shader, breakout::Texture2D &texture, unsigned int count)
    : particleShader(shader), particleTexture(texture), nrParticles(count)
{
    for (unsigned i = 0; i < nrParticles; i++)
    {
        particles.push_back(Particle());
    }

    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f};
    GL_CHECK(glGenVertexArrays(1, &this->particleVAO));
    GL_CHECK(glBindVertexArray(this->particleVAO));
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)0);
    GL_CHECK(glBindVertexArray(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

breakout::ParticleEmmiter::~ParticleEmmiter()
{
    particles.clear();
}

void breakout::ParticleEmmiter::Draw()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    particleShader.Use();
    GL_CHECK(glBindVertexArray(this->particleVAO));
    for (auto &particle : particles)
    {
        if (particle.Life <= 0.0f)
        {
            continue;
        }
        particleShader.SetVector2f("offset", particle.Position);
        particleShader.SetVector4f("color", particle.Color);
        particleTexture.Bind();
        GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6));
    }
    GL_CHECK(glBindVertexArray(0));
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void breakout::ParticleEmmiter::Update(float dt, breakout::GameObject &obj, unsigned int newParticles, glm::vec2 offset)
{
    // reuse deleted particles
    for (unsigned int i = 0; i < newParticles; i++)
    {
        int index = firstUnusedParticle();
        respawnParticle(this->particles[index], obj, offset);
    }

    // update particles
    for (unsigned int i = 0; i < nrParticles; i++)
    {
        Particle &p = particles[i];
        p.Life -= dt;
        if (p.Life > 0.0f)
        {
            // p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

int breakout::ParticleEmmiter::firstUnusedParticle()
{
    for (unsigned int i = lastUnusedParticle; i < particles.size(); i++)
    {
        if (particles[i].Life <= 0.0f)
        {
            lastUnusedParticle = i;
            return i;
        }
    }
    for (unsigned int i = 0; i < lastUnusedParticle; i++)
    {
        if (particles[i].Life <= 0.0f)
        {
            lastUnusedParticle = i;
            return i;
        }
    }
    lastUnusedParticle = 0;
    return 0;
}

void breakout::ParticleEmmiter::respawnParticle(breakout::Particle &particle, breakout::GameObject &obj, glm::vec2 offset)
{
    float particleSize = 10.0f;
    glm::vec2 threshold = (obj.Size - particleSize) / 2.0f;
    auto randomX = ((rand() % 100) - 50) / 100.0f;
    auto randomY = ((rand() % 100) - 50) / 100.0f;
    auto random = glm::vec2(randomX, randomY);

    particle.Position = obj.Position + offset - particleSize / 2.0f + random * threshold;
    float rColor = 0.5f + ((rand() % 100) / 100.f);
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = obj.Velocity * 0.1f;
}