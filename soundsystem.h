#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include "fmod.hpp"
#include <map>
#include <string>

class SoundSystem
{
public:
    static SoundSystem& GetInstance();
    static void DestroyInstance();

    bool Initialise();
    void Process();
    void Release();

    FMOD::Sound* CreateSound(const std::string& filename, bool looping = false);
    FMOD::Channel* PlaySound(FMOD::Sound* pSound);

private:
    SoundSystem();
    ~SoundSystem();
    SoundSystem(const SoundSystem&);
    SoundSystem& operator=(const SoundSystem&);

    static SoundSystem* sm_pInstance;
    FMOD::System* m_pFMODSystem;
};

#endif // SOUNDSYSTEM_H