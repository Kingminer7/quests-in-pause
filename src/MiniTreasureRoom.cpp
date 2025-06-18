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

bool MiniTreasureRoom::setup() {
    if (fadeHook) {
        auto res = fadeHook->enable();
        if (res.isErr()) {
            log::warn("Failed to enable GameManager::fadeInMusic - {}", res.unwrapErr());
        }
    } else {
        log::warn("Hook not found!");
    }
    m_rewardLayer = SecretRewardsLayer::create(false);
    if (fadeHook) {
        auto res = fadeHook->disable();
        if (res.isErr()) {
            log::warn("Failed to disable GameManager::fadeInMusic - {}", res.unwrapErr());
        }
    }
    m_mainLayer->setVisible(false);
    addChild(m_rewardLayer);
    setID("MiniTreasureRoom"_spr);
    setOpacity(0);

    if (auto closeBtn = m_rewardLayer->getChildByID("exit-menu")->getChildByType<CCMenuItemSpriteExtra*>(0)) {
        closeBtn->m_pfnSelector = menu_selector(MiniTreasureRoom::onClose);
        closeBtn->m_pListener = this;
    }
    auto bs = m_rewardLayer->m_mainScrollLayer;

    auto page = static_cast<CCLayer*>(bs->m_pages->objectAtIndex(4));
    bs->m_pages->removeObjectAtIndex(4);
    page->removeFromParent();
    bs->updatePages();
    bs->updateDots(0);
    static_cast<CCNode*>(bs->m_dots->objectAtIndex(4))->removeFromParent();
    bs->m_dots->removeObjectAtIndex(4);

    return true;
}

MiniTreasureRoom* MiniTreasureRoom::create()  {
    auto ret = new MiniTreasureRoom;
    auto ws = CCDirector::get()->getWinSize();
    if (ret && ret->initAnchored(ws.width, ws.height)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void MiniTreasureRoom::onClose(CCObject* sender) {
    if (!m_rewardLayer->m_inMainLayer) return m_rewardLayer->onBack(sender);
    m_rewardLayer->stopAllActions();
    m_rewardLayer->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(0.3, {0, CCDirector::sharedDirector()->getWinSize().height + 5}), CCCallFunc::create(this, callfunc_selector(MiniTreasureRoom::transitionFinished))));
}

void MiniTreasureRoom::transitionFinished() {
    removeFromParent();
}

void MiniTreasureRoom::show() {
    if (m_noElasticity) return Popup::show();
    auto dir = CCDirector::sharedDirector();
    m_noElasticity = true;
    Popup::show();
    m_mainLayer->setVisible(false);
    m_noElasticity = false;
    m_rewardLayer->setPosition({0, dir->getWinSize().height + 5});
    m_rewardLayer->stopAllActions();
    m_rewardLayer->runAction(CCEaseBounceOut::create(CCMoveTo::create(0.5, {0, 0})));
}