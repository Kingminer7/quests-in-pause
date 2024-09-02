#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;

class $modify(ModifiedPause, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();
		
		auto sprite = CCSprite::createWithSpriteFrameName("quests.png"_spr);
		sprite->setScale(.675f);
		auto questButton = CCMenuItemSpriteExtra::create(
			sprite,
			this,
			menu_selector(CreatorLayer::onChallenge)
		);
		

		auto menu = this->getChildByID("right-button-menu");
		menu->addChild(questButton);

		questButton->setID("quests-button"_spr);

		menu->updateLayout();
	}
};
