#include "RewardsPopup.hpp"

#include "MiniTreasureRoom.hpp"

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

    auto gsm = GameStatsManager::get();

    auto quests = CCMenuItemExt::createSpriteExtraWithFilename("quests.png"_spr, .8f, [](auto) {
        ChallengesPage::create()->show();
    });
    quests->setID("quests-button");
    menu->addChild(quests);

    auto daily = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_dailyRewardBtn_001.png", 1.f, [](auto) {
        RewardsPage::create()->show();
    });
    daily->setID("daily-button");
    menu->addChild(daily);

    auto pathId = gsm->m_activePath - 29;
    CCSprite *path;
    if (pathId > 0 && pathId < 11) {
        auto p = GJPathSprite::create(pathId);
        p->updateState();
        path = p;
    } else {
        path = CCSprite::create("noPath.png"_spr);
    }
    auto paths = CCMenuItemExt::createSpriteExtra(path, [](auto) {
        GJPathsLayer::create()->show();
    });
    paths->setID("paths-button");
    menu->addChild(paths);

    bool open = GameManager::get()->getUGV("5");
    auto treasure = CCMenuItemExt::createSpriteExtraWithFrameName(open ? "secretDoorBtn_open_001.png" : "secretDoorBtn_closed_001.png", 1.f, [this](auto) {
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
            this->addChild(dialogLayer);
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
            this->addChild(dialogLayer);
            GameManager::get()->setUGV("5", true);
        }
    });
    treasure->setID("treasure-button");
    menu->addChild(treasure);

    menu->updateLayout();

    return true;
}

RewardsPopup* RewardsPopup::create() {
    auto ret = new RewardsPopup;
    if (!ret->init()) return delete ret, nullptr;
    return ret->autorelease(), ret;
}
