#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

#include "FMOD.hpp"
#include "../Utils.hpp"
#include "../ui/RewardsPopup.hpp"

#define addToMenu(button, name) if (Mod::get()->getSettingValue<std::string_view>(name) != "Off") {\
    auto btn = button; \
    if (Mod::get()->getSettingValue<std::string_view>(name) == "On Left") { \
        leftMenu->addChild(btn); \
        leftMenu->updateLayout(); \
    } else if (Mod::get()->getSettingValue<std::string_view>(name) == "On Right") { \
        rightMenu->addChild(btn); \
        rightMenu->updateLayout(); \
    }  \
}
using namespace geode::prelude;

class $modify(RewardPause, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        if (!Mod::get()->getSettingValue<bool>("pause")) return;
        if (Mod::get()->getSettingValue<bool>("pause-popup")) {
            auto circle = CircleButtonSprite::createWithSprite("rewards.png"_spr, .85f, CircleBaseColor::Green, CircleBaseSize::MediumAlt);
            circle->setScale(.6f);
            auto btn = CCMenuItemExt::createSpriteExtra(circle, [](auto){
                RewardsPopup::create()->show();
            });
            if (checkChests() || checkQuests()) {
                if (auto alert = CCSprite::createWithSpriteFrameName("exMark_001.png")) {
                    alert->setScale(.6f);
                    alert->setID("alert");
                    alert->setPosition(CCPoint{35.f, 35.f});
                    circle->getTopNode()->addChild(alert);
                }
            }
            btn->setID("rewards-button"_spr);
            auto menu = getChildByID("left-button-menu");
            menu->addChild(btn);
            menu->updateLayout();
            return;
        }
        g_group->setVolume(FMODAudioEngine::get()->m_sfxVolume);

        auto rightMenu = this->getChildByID("right-button-menu");
        auto leftMenu = this->getChildByID("left-button-menu");

        addToMenu(createDailyButton(), "chests")
        addToMenu(createQuestButton(), "quests")
        addToMenu(createTreasureButton(this), "treasure")
        addToMenu(createPathButton(), "paths")
    }

    void sfxSliderChanged(CCObject* sender) {
        PauseLayer::sfxSliderChanged(sender);
        g_group->setVolume(FMODAudioEngine::get()->m_sfxVolume);
    }
};



class $modify(RewardEditorPause, EditorPauseLayer) {
    bool init(LevelEditorLayer* editor) {
        if (!EditorPauseLayer::init(editor)) return false;
        if (Mod::get()->getSettingValue<bool>("editor")) {
            auto circle = CircleButtonSprite::createWithSprite("rewards.png"_spr, .85f, CircleBaseColor::Green, CircleBaseSize::Medium);
            circle->setScale(.85f);
            auto btn = CCMenuItemExt::createSpriteExtra(circle, [](auto btn){
                RewardsPopup::create()->show();
            });
            btn->setID("rewards-button"_spr);
            auto menu = getChildByID("guidelines-menu");
            menu->addChild(btn);
            menu->updateLayout();
        }
        return true;
    }
};