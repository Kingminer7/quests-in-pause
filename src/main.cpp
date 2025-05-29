#include <Geode/modify/PauseLayer.hpp>
#include <Geode/binding/ChallengesPage.hpp>
#include <Geode/binding/MenuLayer.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

#define addButton(name, callback, sprite, spriteScale) \
    if (Mod::get()->getSettingValue<std::string>(name) != "Off") { \
		auto circleSprite = CircleButtonSprite::create(sprite, CircleBaseColor::Green, CircleBaseSize::MediumAlt); \
		circleSprite->setScale(0.6f); \
        sprite->setScale(spriteScale); \
		auto button = CCMenuItemSpriteExtra::create( \
			circleSprite, \
			this, \
			menu_selector(callback) \
		); \
		button->setID(fmt::format("{}-button"_spr, name)); \
		if (Mod::get()->getSettingValue<std::string>(name) == "On Left") { \
			leftMenu->addChild(button); \
			leftMenu->updateLayout(); \
		} else if (Mod::get()->getSettingValue<std::string>(name) == "On Right") { \
			rightMenu->addChild(button); \
			rightMenu->updateLayout(); \
		} \
    }

class $modify(RewardPause, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		auto rightMenu = this->getChildByID("right-button-menu");
		auto leftMenu = this->getChildByID("left-button-menu");

        auto chest = CCSprite::createWithSpriteFrameName("GJ_dailyRewardBtn_001.png");
        addButton("chests", MenuLayer::onDaily, chest, .65f);
        auto quest = CCSprite::create("quests.png"_spr);
        addButton("quests", RewardPause::onQuests, quest, .575f);
        auto pathId = GameStatsManager::get()->m_activePath - 29;
        CCSprite *path;
        if (pathId > 0 && pathId < 11) {
            auto p =  GJPathSprite::create(pathId);
            p->updateState();
            path = p;
        } else {
            path = CCSprite::create("noPath.png"_spr);
        }
        addButton("paths", CreatorLayer::onPaths, path, .75f);
	}
		
	void onQuests(CCObject *sender) {
		auto layer = ChallengesPage::create();
		layer->show();
	}
};