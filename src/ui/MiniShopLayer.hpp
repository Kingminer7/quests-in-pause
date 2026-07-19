#pragma once

#include "MiniLayer.hpp"
#include <Geode/binding/GJShopLayer.hpp>

using namespace geode::prelude;

class MiniShopLayer : public MiniLayer<GJShopLayer> {
protected:
    bool init(ShopType type);
public:
    static MiniShopLayer* create(ShopType type);
};