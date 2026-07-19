#include "Utils.hpp"

#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>

#include "ui/MiniTreasureRoom.hpp"

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

    void fadeInMusic(gd::string p0) {}
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

    void playMusic(gd::string path, bool shouldLoop, float fadeInTime, int channel) {}
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

CCMenuItemSpriteExtra* makeButton(ZStringView name, Function<void(CCMenuItemSpriteExtra*)> callback, CCNode* sprite, float spriteScale, bool noCircle) {
    if (!noCircle) {
        if (!sprite) sprite = CCSprite::create();
        auto circleSprite = CircleButtonSprite::create(sprite, CircleBaseColor::Green, CircleBaseSize::MediumAlt);
        circleSprite->setScale(0.6f);
        sprite->setScale(spriteScale);
        auto button = CCMenuItemExt::createSpriteExtra(circleSprite, std::move(callback));
        button->setID(fmt::format("{}-button"_spr, name));
        return button;
    }
    sprite->setScale(spriteScale);
    auto button = CCMenuItemExt::createSpriteExtra(sprite, std::move(callback));
    button->setID(fmt::format("{}-button"_spr, name));
    return button;
}

CCMenuItemSpriteExtra* createQuestButton(bool alt) {
    auto quest = CCSprite::create("quests.png"_spr);
    if (checkQuests()) {
        if (auto alert = CCSprite::createWithSpriteFrameName("exMark_001.png")) {
            alert->setScale(.75f);
            alert->setID("alert");
            alert->setPosition(CCPoint{40.f, 37.5f});
            quest->addChild(alert);
        }
    }
    return makeButton("quests", [](auto) {
        ChallengesPage::create()->show();
    }, quest, alt ? 0.8f : .575f, alt);
}

CCMenuItemSpriteExtra* createTreasureButton(CCNode* dialogParent, bool alt) {
    return makeButton("treasure", [dialogParent](auto) {
        int count = GameStatsManager::get()->getStat("21");
        if (GameManager::get()->getUGV("5")) {
            auto layer = MiniTreasureRoom::create();
            layer->show();
        } else if (count < 5) {
            int needed = 5 - count;
            auto dialog = DialogObject::create("The Keymaster", fmt::format("Collect <cy>{}</c> more <cg>key{}</c>,<d020>\nand I will let you pass.", needed, needed > 1 ? "s" : ""), 2, 1.0, false, ccWHITE);
            auto dialogLayer = DialogLayer::createDialogLayer(dialog, nullptr, 2);
            dialogLayer->setZOrder(9999);
            dialogLayer->animateInRandomSide();
            dialogParent->addChild(dialogLayer);
        } else {
            auto arr = CCArray::create();
            auto dialog1 = DialogObject::create("The Keymaster", "Well, well, well.<d035>\nLook who it is.", 2, 1.0, false, ccWHITE);
            auto dialog2 = DialogObject::create("The Keymaster", "I see you have the <cg>keys</c>.<d040>\nBut what comes next?", 2, 1.0, false, ccWHITE);
            auto dialog3 = DialogObject::create("The Keymaster", "The <co>door</c> is open.<d040>\nTime to find out<d010>.<d010>.<d010>.", 2, 1.0, false, ccWHITE);

            arr->addObject(dialog1);
            arr->addObject(dialog2);
            arr->addObject(dialog3);

            auto dialogLayer = DialogLayer::createDialogLayer(nullptr, arr, 2);
            dialogLayer->setZOrder(9999);
            dialogLayer->animateInRandomSide();
            dialogParent->addChild(dialogLayer);
            GameManager::get()->setUGV("5", true);
        }
    }, CCSprite::createWithSpriteFrameName(alt ? (GameManager::get()->getUGV("5") ? "secretDoorBtn_open_001.png" : "secretDoorBtn_closed_001.png") : "chest03_small_001.png"), alt ? 1.f : .95, alt);
}

CCMenuItemSpriteExtra* createDailyButton(bool alt) {
    auto chest = CCSprite::createWithSpriteFrameName("GJ_dailyRewardBtn_001.png");
    if (checkChests()) {
        if (auto alert = CCSprite::createWithSpriteFrameName("exMark_001.png")) {
            alert->setScale(.6f);
            alert->setID("alert");
            alert->setPosition(CCPoint{35.f, 35.f});
            chest->addChild(alert);
        }
    }
    return makeButton("chests", [](auto) {
        auto page = RewardsPage::create();
        if (auto bibutton = typeinfo_cast<CCMenuItemSpriteExtra*>(page->m_buttonMenu->getChildByID("cvolton.betterinfo/history-button"))) {
            bibutton->setUserObject("intercepter"_spr, Intercepter::create(bibutton, [](Intercepter* inter, CCObject* sender) {
                FLAlertLayer::create("Sorry!", "BetterInfo's Chest History is disabled in the pause menu. Please exit the level to view this.", "Ok")->show();
            }));
            bibutton->setOpacity(127);
            bibutton->setColor(ccColor3B{127, 127, 127});
        }
        page->show();
    }, chest, alt ? 1.f : .65f, alt);
}

bool checkChests() {
    auto gsm = GameStatsManager::get();
    if (gsm->m_rewardItems->count() == 0) {
        GameLevelManager::get()->m_GJRewardDelegate = nullptr;
        GameLevelManager::get()->getGJRewards(0);
        return false;
    }
    // theres a chance these are backwards
    auto* bigChest = typeinfo_cast<GJRewardItem *>(gsm->m_rewardItems->objectForKey(1));
    auto* smallChest = typeinfo_cast<GJRewardItem *>(gsm->m_rewardItems->objectForKey(2));

    return bigChest && smallChest && (bigChest->m_timeRemaining == 0 || smallChest->m_timeRemaining == 0);
}

bool checkQuests() {
    for (int i = 1; i < 4; i++) {
        auto c = GameStatsManager::get()->getChallenge(i);
        if (c && c->m_canClaim) {
            return true;
        }
    }
    return false;
}

CCMenuItemSpriteExtra* createPathButton(bool alt) {
    auto pathId = GameStatsManager::get()->m_activePath - 29;
    CCSprite *path;
    if (pathId > 0 && pathId < 11) {
        auto p = GJPathSprite::create(pathId);
        p->updateState();
        path = p;
    } else {
        path = CCSprite::create("noPath.png"_spr);
    }
    return makeButton("paths", [](auto) {
        GJPathsLayer::create()->show();
    }, path, alt ? 1.f : 0.75f, alt);
}