#include "MiniTreasureRoom.hpp"

bool MiniTreasureRoom::setup() {
    m_rewardLayer = SecretRewardsLayer::create(false);
    m_mainLayer->removeFromParent();
    addChild(m_rewardLayer);
    m_mainLayer = m_rewardLayer;
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
    m_mainLayer->stopAllActions();
    m_mainLayer->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(0.3, {0, CCDirector::sharedDirector()->getWinSize().height + 5}), CCCallFunc::create(this, callfunc_selector(MiniTreasureRoom::transitionFinished))));
}

void MiniTreasureRoom::transitionFinished() {
    removeFromParent();
}

void MiniTreasureRoom::show() {
    if (m_noElasticity) return Popup::show();
    auto dir = CCDirector::sharedDirector();
    m_noElasticity = true;
    Popup::show();
    m_noElasticity = false;
    m_mainLayer->setPosition({0, dir->getWinSize().height + 5});
    m_mainLayer->stopAllActions();
    m_mainLayer->runAction(CCEaseBounceOut::create(CCMoveTo::create(0.5, {0, 0})));
}