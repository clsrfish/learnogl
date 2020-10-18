#if !defined(AUDIO_MANAGER_H)
#define AUDIO_MANAGER_H

#include <string>
#include <irrklang/irrKlang.h>

namespace breakout
{

    class AudioManager
    {
    private:
        irrklang::ISoundEngine *soundEngine;

    public:
        AudioManager(/* args */);
        ~AudioManager();

        void PlaySound(const std::string &mediaFile, bool loop = false);
    };

} // namespace breakout

#endif // AUDIO_MANAGER_H
