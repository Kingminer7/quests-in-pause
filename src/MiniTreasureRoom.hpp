#pragma once

using namespace geode::prelude;

class MiniTreasureRoom : public Popup<> {
    protected:
        bool setup() override;
        void transitionFinished();
    public:
        static MiniTreasureRoom* create();
        void onClose(CCObject* sender) override;
        void show() override;

        SecretRewardsLayer* m_rewardLayer;
};