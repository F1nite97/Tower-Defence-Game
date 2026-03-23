#include "soundsystem.h"
#include "logmanager.h"

SoundSystem* SoundSystem::sm_pInstance = 0;

SoundSystem& SoundSystem::GetInstance()
{
    if (sm_pInstance == 0)
    {
        sm_pInstance = new SoundSystem();
    }
    return (*sm_pInstance);
}

void SoundSystem::DestroyInstance()
{
    delete sm_pInstance;
    sm_pInstance = 0;
}

SoundSystem::SoundSystem() : m_pFMODSystem(0) {}

SoundSystem::~SoundSystem()
{
    Release();
}

bool SoundSystem::Initialise()
{
    // 1. Create the FMOD System object
    FMOD_RESULT result = FMOD::System_Create(&m_pFMODSystem);
    if (result != FMOD_OK)
    {
        LogManager::GetInstance().Log("FMOD: System_Create failed!");
        return false;
    }

    // 2. Initialise FMOD (max 512 channels)
    result = m_pFMODSystem->init(512, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        LogManager::GetInstance().Log("FMOD: System init failed!");
        return false;
    }

    return true;
}

void SoundSystem::Process()
{
    if (m_pFMODSystem)
    {
        // Must be called once per game loop tick
        m_pFMODSystem->update();
    }
}

void SoundSystem::Release()
{
    if (m_pFMODSystem)
    {
        m_pFMODSystem->release();
        m_pFMODSystem = 0;
    }
}

FMOD::Sound* SoundSystem::CreateSound(const std::string& filename, bool looping)
{
    FMOD::Sound* pSound = 0;
    FMOD_MODE mode = looping ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;

    FMOD_RESULT result = m_pFMODSystem->createSound(filename.c_str(), mode, 0, &pSound);
    if (result != FMOD_OK)
    {
        LogManager::GetInstance().Log("FMOD: createSound failed!");
        return 0;
    }
    return pSound;
}

FMOD::Channel* SoundSystem::PlaySound(FMOD::Sound* pSound)
{
    FMOD::Channel* pChannel = 0;
    if (m_pFMODSystem && pSound)
    {
        // nullptr channel group = use default. false = start un-paused.
        m_pFMODSystem->playSound(pSound, nullptr, false, &pChannel);
    }
    return pChannel;
}