#include <Geode/modify/PauseLayer.hpp>
#include <Geode/binding/CreatorLayer.hpp>
#include <Geode/binding/MenuLayer.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;

class $modify(ModifiedPause, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		auto rightMenu = this->getChildByID("right-button-menu");
		auto leftMenu = this->getChildByID("left-button-menu");

		if (Mod::get()->getSettingValue<std::string>("chests") != "Off") {
			auto chestSprite = CCSprite::createWithSpriteFrameName("chest.png"_spr);
			chestSprite->setScale(.675f);
			auto chestButton = CCMenuItemSpriteExtra::create(
				chestSprite,
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
			auto questButton = CCMenuItemSpriteExtra::create(
				questSprite,
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
