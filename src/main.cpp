#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/RewardsPage.hpp>
#include <Geode/modify/ChallengeNode.hpp>
#include <Geode/binding/ChallengesPage.hpp>
#include <Geode/binding/MenuLayer.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include "MiniTreasureRoom.hpp"

using namespace geode::prelude;

#define addButton(name, callback, sprite, spriteScale) \
    if (sprite && Mod::get()->getSettingValue<std::string>(name) != "Off") { \
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
	auto gsm = GameStatsManager::get();
	if (gsm->m_rewardItems->count() == 0) {
		GameLevelManager::get()->getGJRewards(0);
	} else if(chest) {
		// theres a chance these are backwards
		auto big = static_cast<GJRewardItem*>(gsm->m_rewardItems->objectForKey(1));
		auto small = static_cast<GJRewardItem*>(gsm->m_rewardItems->objectForKey(2));
		if (big && small && (big->m_timeRemaining == 0 || small->m_timeRemaining == 0)) {
			auto alert = CCSprite::createWithSpriteFrameName("exMark_001.png");
			if (alert) {
				alert->setScale(.6f);
				alert->setID("alert");
				alert->setPosition(CCPoint{35.f, 35.f});
				chest->addChild(alert);
			}
		}
	}
        addButton("chests", MenuLayer::onDaily, chest, .65f);

        auto quest = CCSprite::create("quests.png"_spr);
	for (int i = 1; i < 4; i++) {
		auto c = gsm->getChallenge(i);
		if(c && c->m_canClaim) {
			auto alert = CCSprite::createWithSpriteFrameName("exMark_001.png");
			if (alert) {
				alert->setScale(.75f);
				alert->setID("alert");
				alert->setPosition(CCPoint{40.f, 37.5f});
				quest->addChild(alert);
			}
			break;
		}
	}
        addButton("quests", RewardPause::onQuests, quest, .575f);
        auto treasure = CCSprite::createWithSpriteFrameName("chest03_small_001.png");
        addButton("treasure", RewardPause::onTreasureRoom, treasure, .95);
        auto pathId = gsm->m_activePath - 29;
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
	
	void onQuests(CCObject* sender) {
		auto layer = ChallengesPage::create();
		layer->show();
	}

    void onTreasureRoom(CCObject* sender) {
        int count = GameStatsManager::get()->getStat("21");
        if (GameManager::get()->getUGV("5")) {
            auto layer = MiniTreasureRoom::create();
            layer->show();
        } else if (count < 5) {
            int needed = 5 - count;
            auto dialog = DialogObject::create("The Keymaster", fmt::format("Collect <cy>{}</c> more <cg>key{}</c>,<d020>\nand I will let you pass.", needed, needed > 1 ? "s" : "").c_str(), 2, 1.0, false, ccWHITE);
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
    }
};

// indent hell
class $modify(RewardsPage) {
	void onClose(CCObject* sender) {
		RewardsPage::onClose(sender);
		if (auto pause = CCScene::get()->getChildByType<PauseLayer>(0)) {
			if (auto btn = pause->getChildByIDRecursive("chests-button"_spr)) {
				if (auto alert = btn->getChildByIDRecursive("alert")) {
					auto gsm = GameStatsManager::get();
					if (gsm->m_rewardItems->count() == 0) return;
					// theres a chance these are backwards
					auto big = static_cast<GJRewardItem*>(gsm->m_rewardItems->objectForKey(1));
					auto small = static_cast<GJRewardItem*>(gsm->m_rewardItems->objectForKey(2));
					if (big && small && big->m_timeRemaining > 0 && small->m_timeRemaining > 0) {
						alert->removeFromParent();
					}
				}
			}
		}
	}
};

class $modify(ChallengeNode) {
	void onClaimReward(CCObject* sender) {
		auto gsm = GameStatsManager::get();
		if (auto pause = CCScene::get()->getChildByType<PauseLayer>(0)) {
			if (auto btn = pause->getChildByIDRecursive("quests-button"_spr)) {
				if (auto alert = btn->getChildByIDRecursive("alert")) {
					for (int i = 1; i < 4; i++) {
						auto c = gsm->getChallenge(i);
						if(c && c->m_canClaim) return;
					}
					alert->removeFromParent();
				}
			}
			auto pos = getPosition() + m_unkPoint;
			if (this->m_challengesPage && this->m_challengeItem) {
        			this->m_challengesPage->claimItem(this, this->m_challengeItem, pos);
    			}

			// play sfx here
			
			if (auto node = typeinfo_cast<CCMenuItem*>(sender)) {
				node->setVisible(false);
				node->setEnabled(false);
			}
		} else {
			ChallengeNode::onClaimReward(sender);
		}
	}
};
