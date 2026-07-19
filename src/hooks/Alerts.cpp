#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/RewardsPage.hpp>
#include <Geode/modify/ChallengeNode.hpp>
#include <utility>

using namespace geode::prelude;

class $modify(RewardsPage) {
    void rewardsStatusFinished(int id) {
        RewardsPage::rewardsStatusFinished(id);
        if ((id == 1 && !m_rightOpen ) || (id == 2 && !m_leftOpen)) {
            if (auto pause = CCScene::get()->getChildByType<PauseLayer>(0)) {
                if (auto btn = pause->getChildByIDRecursive("chests-button"_spr)) {
                    log::debug("Removing chest alert");
                    if (auto alert = btn->getChildByIDRecursive("alert")) {
                        alert->removeFromParent();
                    }
                }
            }
        }
    }
};

class $modify(GameStatsManager) {
    void storeRewardState(GJRewardType type, int id, int remaining, gd::string str) {
        GameStatsManager::storeRewardState(type, id, remaining, std::move(str));
        if (auto pause = CCScene::get()->getChildByType<PauseLayer>(0)) {
            if (auto btn = pause->getChildByIDRecursive("chests-button"_spr)) {
                auto cbs = btn->getChildByIndex(0);
                if (!cbs) return;
                auto chest = cbs->getChildByIndex(0);
                if (!chest) return;
                if (chest->getChildByID("alert")) return;
                if (m_rewardItems->count() == 0) {
                    return;
                }
                auto *bigChest = typeinfo_cast<GJRewardItem *>(m_rewardItems->objectForKey(1));
                auto *smallChest = typeinfo_cast<GJRewardItem *>(m_rewardItems->objectForKey(2));
                if (bigChest && smallChest && (bigChest->m_timeRemaining == 0 || smallChest->m_timeRemaining == 0)) {
                    log::debug("Adding chest alert");
                    if (auto alert = CCSprite::createWithSpriteFrameName("exMark_001.png")) {
                        alert->setScale(.6f);
                        alert->setID("alert");
                        alert->setPosition(CCPoint{35.f, 35.f});
                        chest->addChild(alert);
                    }
                }
            }
        }
    }
};

class $modify(ChallengeNode) {
    void onClaimReward(CCObject *sender) {
        ChallengeNode::onClaimReward(sender);
        auto gsm = GameStatsManager::get();
        if (auto pause = CCScene::get()->getChildByType<PauseLayer>(0)) {
            if (auto btn = pause->getChildByIDRecursive("quests-button"_spr)) {
                log::debug("Removing quest alert");
                if (auto alert = btn->getChildByIDRecursive("alert")) {
                    for (int i = 1; i < 4; i++) {
                        auto c = gsm->getChallenge(i);
                        if (c && c->m_canClaim) return;
                    }
                    alert->removeFromParent();
                }
            }
        }
    }
};