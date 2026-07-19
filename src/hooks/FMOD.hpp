#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/System.hpp>

using namespace geode::prelude;

// ill admit this is probably some of the most evil code ive written
// but like, these now have sounds in the pause menu
static FMOD::ChannelGroup* g_group;

class $modify(HookedFMODSystem, FMOD::System) {
    FMOD_RESULT playSound(FMOD::Sound *sound, FMOD::ChannelGroup *channelgroup, bool paused, FMOD::Channel **channel);
};