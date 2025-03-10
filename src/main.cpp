#include <Geode/modify/PauseLayer.hpp>
#include <Geode/binding/CreatorLayer.hpp>
#include <Geode/binding/MenuLayer.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

class $modify(ModifiedPause, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		auto rightMenu = this->getChildByID("right-button-menu");
		auto leftMenu = this->getChildByID("left-button-menu");

		if (Mod::get()->getSettingValue<std::string>("chests") != "Off") {
			auto chestSprite = CCSprite::createWithSpriteFrameName("GJ_dailyRewardBtn_001.png");
			chestSprite->setScale(.9f);
			auto chestCBS = CircleButtonSprite::create(chestSprite,CircleBaseColor::Green, CircleBaseSize::MediumAlt);
			chestCBS->setScale(0.6f);
			auto chestButton = CCMenuItemSpriteExtra::create(
				chestCBS,
				this,
				menu_selector(MenuLayer::onDaily)
			);
			chestButton->setID("chest-button"_spr);

			if (Mod::get()->getSettingValue<std::string>("chests") == "On Left") {
				leftMenu->addChild(chestButton);
				leftMenu->updateLayout();
			} else if (Mod::get()->getSettingValue<std::string>("chests") == "On Right") {
				rightMenu->addChild(chestButton);
				rightMenu->updateLayout();
			}
		}

		if (Mod::get()->getSettingValue<std::string>("quests") != "Off") {
			auto questSprite = CCSprite::createWithSpriteFrameName("quests.png"_spr);
			questSprite->setScale(.675f);
			auto questCBS = CircleButtonSprite::create(questSprite,CircleBaseColor::Green, CircleBaseSize::MediumAlt);
			questCBS->setScale(0.6f);
			auto questButton = CCMenuItemSpriteExtra::create(
				questCBS,
				this,
				menu_selector(CreatorLayer::onChallenge)
			);
			questButton->setID("quest-button"_spr);

			if (Mod::get()->getSettingValue<std::string>("quests") == "On Left") {
				leftMenu->addChild(questButton);
				leftMenu->updateLayout();
			} else if (Mod::get()->getSettingValue<std::string>("quests") == "On Right") {
				rightMenu->addChild(questButton);
				rightMenu->updateLayout();
			}
		}
		
	}
};
