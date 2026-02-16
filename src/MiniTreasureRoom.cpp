#include "MiniTreasureRoom.hpp"
#include <Geode/modify/GameManager.hpp>

// This weird system is to stop the treasure room loop from overwriting the level's song

Hook* fadeHook = nullptr;

class $modify(RewardGM, GameManager) {
    static void onModify(auto& self) {
        if (auto h = self.getHook("GameManager::fadeInMusic")) {
            fadeHook = h.unwrap();
            fadeHook->setAutoEnable(false);
            auto res = fadeHook->disable();
            if (res.isErr()) {
                log::warn("Failed to disable GameManager::fadeInMusic - {}", res.unwrapErr());
            }
        }
    }

    void fadeInMusic(gd::string p0) {
        
    }
};

bool MiniTreasureRoom::init() {
    if (fadeHook) {
        auto res = fadeHook->enable();
        if (res.isErr()) {
            log::warn("Failed to enable GameManager::fadeInMusic - {}", res.unwrapErr());
        }
    } else {
        log::warn("Hook not found!");
    }
    auto layer = SecretRewardsLayer::create(false);
    if (fadeHook) {
        auto res = fadeHook->disable();
        if (res.isErr()) {
            log::warn("Failed to disable GameManager::fadeInMusic - {}", res.unwrapErr());
        }
    }
    log::info("{}", layer->m_backSprite);
    if (!MiniLayer::init(layer, layer->getChildByID("exit-menu")->getChildByType<CCMenuItemSpriteExtra*>(0))) return false;

    const auto bs = layer->m_mainScrollLayer;

    const auto page = typeinfo_cast<CCLayer*>(bs->m_pages->objectAtIndex(4));
    bs->m_pages->removeObjectAtIndex(4);
    page->removeFromParent();
    bs->updatePages();
    bs->updateDots(0);
    typeinfo_cast<CCNode*>(bs->m_dots->objectAtIndex(4))->removeFromParent();
    bs->m_dots->removeObjectAtIndex(4);

    return true;
}

MiniTreasureRoom* MiniTreasureRoom::create()  {
    auto ret = new MiniTreasureRoom;
    if (ret->init()) return ret->autorelease(), ret;
    return delete ret, nullptr;
}