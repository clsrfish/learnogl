#if !defined(POST_PROCESSOR_H)
#define POST_PROCESSOR_H

#include "./shader.hpp"
#include "./texture.hpp"

namespace breakout
{
    class PostProcessor
    {
    private:
        unsigned int MSFBO, FBO;
        unsigned int RBO;
        unsigned int VAO;

        unsigned int width, height;
        Shader effectShader;
        Texture2D texture;

        void initRenderData();

    public:
        bool Confuse, Chaos, Shake;
        PostProcessor(unsigned int width, unsigned int height, Shader &shader);
        ~PostProcessor();

        void BeginRender();
        void EndRender();
        void Render(float time);
    };

} // namespace breakout

#endif // POST_PROCESSOR_H
