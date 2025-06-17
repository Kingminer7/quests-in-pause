#include "MiniTreasureRoom.hpp"

void loopThrough(CCNode* p) {
    for (auto child : CCArrayExt<CCNode*>(p->getChildren())) {
        if (auto td = typeinfo_cast<CCTouchDelegate*>(child)) {
            queueInMainThread([child, td]{
                if (auto handler = CCTouchDispatcher::get()->findHandler(td)) {
                    // CCTouchDispatcher::get()->setPriority(handler->getPriority() - 10, handler->getDelegate());
                }
            });
        }
        loopThrough(child);
    }
}

bool MiniTreasureRoom::init() {
    if (!SecretRewardsLayer::init(false)) return false;

    getChildByID("exit-menu")->getChildByType<CCMenuItemSpriteExtra*>(0)->m_pfnSelector = menu_selector(MiniTreasureRoom::onBack);

    auto page = static_cast<CCLayer*>(m_mainScrollLayer->m_pages->objectAtIndex(4));
    m_mainScrollLayer->m_pages->removeObjectAtIndex(4);
    page->removeFromParent();
    m_mainScrollLayer->updatePages();
    m_mainScrollLayer->updateDots(0);
    static_cast<CCNode*>(m_mainScrollLayer->m_dots->objectAtIndex(4))->removeFromParent();
    m_mainScrollLayer->m_dots->removeObjectAtIndex(4);
    CCTouchDispatcher::get()->addTargetedDelegate(this, -502, true);
    loopThrough(this);

    return true;
}

MiniTreasureRoom* MiniTreasureRoom::create()  {
    auto ret = new MiniTreasureRoom;
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void MiniTreasureRoom::show() {
    auto dir = CCDirector::sharedDirector();
    auto scene = dir->getRunningScene();
    int z = std::max(scene->getHighestChildZ(), 105);
    scene->addChild(this, z);
    setPosition({0, dir->getWinSize().height + 5});
    this->setVisible(true);
    if (auto pl = CCScene::get()->getChildByType<PauseLayer*>(0)) {
        CCTouchDispatcher::get()->setPriority(-501, pl);
    }
    stopAllActions();
    runAction(CCEaseBounceOut::create(CCMoveTo::create(0.5, {0, 0})));
}

void MiniTreasureRoom::onBack(CCObject* sender) {
    if (!m_inMainLayer) return SecretRewardsLayer::onBack(sender);
    setTouchEnabled(false);
    setKeyboardEnabled(false);
    setKeypadEnabled(false);
    stopAllActions();
    runAction(CCSequence::createWithTwoActions(CCMoveTo::create(0.3, {0, CCDirector::sharedDirector()->getWinSize().height + 5}), CCCallFunc::create(this, callfunc_selector(MiniTreasureRoom::transitionFinished))));
    if (auto pl = CCScene::get()->getChildByType<PauseLayer*>(0)) {
        CCTouchDispatcher::get()->setPriority(-502, pl);
    }
}
bool MiniTreasureRoom::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    return true;
}

void MiniTreasureRoom::transitionFinished() {
    removeFromParent();
}

void MiniTreasureRoom::keyBackClicked() {
    onBack(nullptr);
}

void MiniTreasureRoom::onExit() {
    SecretRewardsLayer::onExit();
    CCTouchDispatcher::get()->removeDelegate(this);
}