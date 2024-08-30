#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(ModifiedPause, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		CCNode * sprite = CircleButtonSprite::createWithSpriteFrameName("quests.png"_spr, .8f, CircleBaseColor::Green, CircleBaseSize::MediumAlt);
		sprite->setScale(0.6);

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
