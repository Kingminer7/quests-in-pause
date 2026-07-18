#pragma once

#include "MiniLayer.hpp"

using namespace geode::prelude;

class MiniTreasureRoom : public MiniLayer<SecretRewardsLayer> {
protected:
    bool init() override;
    void onClose(CCObject* sender) override;
    void interceptedShopBtn(CCObject* sender);
    void interceptedJamBtn(CCObject* sender);

    CCObject* m_jamListener = nullptr;
    SEL_MenuHandler m_jamSelector = nullptr;
public:
    static MiniTreasureRoom* create();
};
