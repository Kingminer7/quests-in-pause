#include <Geode/modify/PauseLayer.hpp>
#include <Geode/binding/ChallengesPage.hpp>
#include <Geode/binding/MenuLayer.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

class $modify(RewardPause, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		auto rightMenu = this->getChildByID("right-button-menu");
		auto leftMenu = this->getChildByID("left-button-menu");

		if (Mod::get()->getSettingValue<std::string>("chests") != "Off") {
			auto chestSprite = CCSprite::createWithSpriteFrameName("GJ_dailyRewardBtn_001.png");
			auto chestCBS = CircleButtonSprite::create(chestSprite,CircleBaseColor::Green, CircleBaseSize::MediumAlt);
			chestSprite->setScale(.65f);
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
			auto questSprite = CCSprite::create("quests.png"_spr);
			auto questCBS = CircleButtonSprite::create(questSprite,CircleBaseColor::Green, CircleBaseSize::MediumAlt);
			questSprite->setScale(.575f);
			questCBS->setScale(0.6f);
			auto questButton = CCMenuItemSpriteExtra::create(
				questCBS,
				this,
				menu_selector(RewardPause::onQuests)
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
		
	void onQuests(CCObject *sender) {
		auto layer = ChallengesPage::create();
		layer->show();
	}
};
