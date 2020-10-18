#if !defined(PARTICLE_H)
#define PARTICLE_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "./game_object.hpp"
#include "./texture.hpp"

namespace breakout
{

    class Particle
    {
    private:
        /* data */
    public:
        glm::vec2 Position, Velocity;
        glm::vec4 Color;
        float Life;

        Particle(/* args */);
        ~Particle();
    };

    class ParticleEmmiter
    {
    private:
        unsigned int nrParticles;
        Texture2D particleTexture;
        Shader particleShader;
        unsigned int particleVAO;
        unsigned int lastUnusedParticle = 0;
        std::vector<Particle> particles;

        int firstUnusedParticle();

        void respawnParticle(Particle &particle, GameObject &obj, glm::vec2 offset);

    public:
        ParticleEmmiter(Shader &shader, Texture2D &texture, unsigned int count);
        ~ParticleEmmiter();

        void Draw();
        void Update(float dt, GameObject &obj, unsigned int newParticles, glm::vec2 offset);
    };

} // namespace breakout

#endif // PARTICLE_H
