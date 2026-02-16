#pragma once

#include "MiniLayer.hpp"

using namespace geode::prelude;

class MiniTreasureRoom : public MiniLayer<SecretRewardsLayer> {
protected:
    bool init() override;
public:
    static MiniTreasureRoom* create();
};
