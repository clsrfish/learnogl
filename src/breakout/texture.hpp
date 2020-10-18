#if !defined(TEXTURE_H)
#define TEXTURE_H

namespace breakout
{

    class Texture2D
    {
    private:
        /* data */
    public:
        Texture2D(/* args */);
        ~Texture2D();

        unsigned int ID;
        unsigned int Width, Height;

        unsigned int InternalFormat;
        unsigned int ImageFormat;
        unsigned int WrapS, WrapT;
        unsigned int FilterMin, FilterMag;

        void Generate(unsigned int width, unsigned int height, unsigned char *data);

        void Bind() const;
    };

} // namespace breakout

#endif // TEXTURE_H
