#include "FMOD.hpp"

FMOD_RESULT HookedFMODSystem::playSound(FMOD::Sound* sound, FMOD::ChannelGroup* channelgroup, bool paused, FMOD::Channel** channel)  {
    if (CCScene::get()->getChildByType<PauseLayer>(0)) {
        if (channelgroup == FMODAudioEngine::get()->m_globalChannel) {
            channelgroup = g_group;
            g_group->setVolume(FMODAudioEngine::get()->m_sfxVolume);
        }
    }
    return System::playSound(sound, channelgroup, paused, channel);
}

$on_mod(Loaded) {
    auto system = FMODAudioEngine::get()->m_system;
    system->createChannelGroup("SFX", &g_group);
    g_group->setVolume(FMODAudioEngine::get()->m_sfxVolume);
}