#pragma once

#include "MiniLayer.hpp"

struct Intercepter;
using namespace geode::prelude;

class MiniTreasureRoom : public MiniLayer<SecretRewardsLayer> {
protected:
    bool init() override;
    void onClose(CCObject* sender) override;
public:
    static MiniTreasureRoom* create();
};
