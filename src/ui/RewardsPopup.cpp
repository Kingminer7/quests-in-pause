#include "RewardsPopup.hpp"

#include "MiniTreasureRoom.hpp"
#include "../Utils.hpp"

using namespace geode::prelude;

bool RewardsPopup::init() {
    if (!Popup::init(250, 80)) return false;
    setID("RewardsPopup"_spr);
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("close-menu");
    m_closeBtn->setID("close-btn");
    m_bgSprite->setID("background");
    auto menu = CCMenu::create();
    menu->setContentSize({260, 60});
    menu->setLayout(RowLayout::create());
    menu->setID("rewards-menu");
    m_mainLayer->addChildAtPosition(menu, Anchor::Center);

    menu->addChild(createQuestButton(true));
    menu->addChild(createDailyButton(true));
    menu->addChild(createPathButton(true));
    menu->addChild(createTreasureButton(this, true));

    menu->updateLayout();

    return true;
}

RewardsPopup* RewardsPopup::create() {
    auto ret = new RewardsPopup;
    if (!ret->init()) return delete ret, nullptr;
    return ret->autorelease(), ret;
}
