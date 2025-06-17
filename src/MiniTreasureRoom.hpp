#pragma once

using namespace geode::prelude;

class MiniTreasureRoom : public SecretRewardsLayer {
    protected:
        bool init() override;
        void onBack(CCObject* sender);
        void keyBackClicked() override;
        void transitionFinished();
        bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
        void onExit() override;
    public:
        static MiniTreasureRoom* create();
        void show();
};