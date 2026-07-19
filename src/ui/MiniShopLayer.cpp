#include "MiniShopLayer.hpp"

#include "../Utils.hpp"

bool MiniShopLayer::init(ShopType type) {
    toggleFMODHook(true);
    auto layer = GJShopLayer::create(type);
    toggleFMODHook(false);
    if (!MiniLayer::init(layer, layer->getChildByType<CCMenu*>(0)->getChildByType<CCMenuItemSpriteExtra*>(0))) return false;
    addBackgroundNode(layer->getChildByType<CCSprite>(0));
    return true;
}

MiniShopLayer* MiniShopLayer::create(ShopType type)  {
    auto ret = new MiniShopLayer;
    if (ret->init(type)) return ret->autorelease(), ret;
    return delete ret, nullptr;
}
