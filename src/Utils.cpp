#include "Utils.hpp"

#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>

using namespace geode::prelude;

// This weird system is to stop the other loop from overwriting the level's song

Hook* fadeHook = nullptr;
Hook* fmodHook = nullptr;

class $modify(RewardGM, GameManager) {
    static void onModify(auto& self) {
        if (auto h = self.getHook("GameManager::fadeInMusic")) {
            fadeHook = h.unwrap();
            fadeHook->setAutoEnable(false);
            auto res = fadeHook->disable();
            if (res.isErr()) {
                log::warn("Failed to disable GameManager::fadeInMusic hook - {}", res.unwrapErr());
            }
        }
    }

    void fadeInMusic(gd::string p0) {

    }
};

class $modify(RewardFMOD, FMODAudioEngine) {
    static void onModify(auto& self) {
        if (auto h = self.getHook("FMODAudioEngine::playMusic")) {
            fmodHook = h.unwrap();
            fmodHook->setAutoEnable(false);
            auto res = fmodHook->disable();
            if (res.isErr()) {
                log::warn("Failed to disable FMODAudioEngine::playMusic hook - {}", res.unwrapErr());
            }
        }
    }

    void playMusic(gd::string path, bool shouldLoop, float fadeInTime, int channel) {

    }
};

void toggleGMFadeHook(bool enabled) {
    if (fadeHook) {
        if (enabled) {
            auto res = fadeHook->enable();
            if (res.isErr()) {
                log::warn("Failed to enable GameManager::fadeInMusic hook - {}", res.unwrapErr());
            }
        } else {
            auto res = fadeHook->disable();
            if (res.isErr()) {
                log::warn("Failed to disable GameManager::fadeInMusic hook - {}", res.unwrapErr());
            }
        }
    } else {
        log::warn("Hook not found!");
    }
}

void toggleFMODHook(bool enabled) {
    if (fadeHook) {
        if (enabled) {
            auto res = fmodHook->enable();
            if (res.isErr()) {
                log::warn("Failed to enable FMODAudioEngine::playMusic hook - {}", res.unwrapErr());
            }
        } else {
            auto res = fmodHook->disable();
            if (res.isErr()) {
                log::warn("Failed to disable FMODAudioEngine::playMusic hook - {}", res.unwrapErr());
            }
        }
    } else {
        log::warn("Hook not found!");
    }
}