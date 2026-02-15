#include "RewardsPopup.hpp"

using namespace geode::prelude;

bool RewardsPopup::init(CCMenuItemSpriteExtra* btn) {
    if (!Popup::init(400, 250)) return false;
    return true;
}

RewardsPopup* RewardsPopup::create(CCMenuItemSpriteExtra* btn) {
    auto ret = new RewardsPopup;
    if (!ret->init(btn)) return delete ret, nullptr;
    return ret->autorelease(), ret;
}
