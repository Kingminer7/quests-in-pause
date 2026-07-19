#include "MiniTreasureRoom.hpp"

#include "MiniShopLayer.hpp"
#include "ModLayers.hpp"
#include "../Utils.hpp"

bool MiniTreasureRoom::init() {
    toggleGMFadeHook(true);
    auto layer = SecretRewardsLayer::create(false);
    toggleGMFadeHook(false);
    if (!MiniLayer::init(layer, layer->getChildByID("exit-menu")->getChildByType<CCMenuItemSpriteExtra*>(0))) return false;
    addBackgroundNode(layer->getChildByID("background"));

    auto sm = layer->querySelector("main-contents > scroll-layer > pages > page5 > store-menu");
    if (sm) {
        #define replaceShop(whose) \
        if (auto whose = typeinfo_cast<CCMenuItemSpriteExtra*>(sm->getChildByID(#whose"-shop"))) { \
            whose->setUserObject("intercepter"_spr, Intercepter::create(whose, [](Intercepter* inter, CCObject* sender) {  \
                MiniShopLayer::create(static_cast<ShopType>(sender->getTag()))->show(); \
            })); \
        } else { \
            log::error("Could not find shop {}!", #whose); \
        }

        replaceShop(scratch);
        replaceShop(creator);
        replaceShop(mechanic);
        replaceShop(diamond);

        if (auto jam = typeinfo_cast<CCMenuItemSpriteExtra*>(sm->getChildByID("zilko.jam/jam-market-button"))) {
            jam->setUserObject("intercepter"_spr, Intercepter::create(jam, [](Intercepter* inter, CCObject* sender) {
                auto shop = stealLayer<JamMarketLayer>([inter] { inter->callOriginal(nullptr); });
                if (!shop) return;
                auto miniJam = MiniLayer<JamMarketLayer>::create(shop, shop->getChildByType<CCMenu*>(0)->getChildByType<CCMenuItemSpriteExtra*>(0));
                miniJam->addBackgroundNode(shop->getChildByType<CCSprite*>(0));
                miniJam->show();
            }));
        }
    }

    return true;
}

void MiniTreasureRoom::onClose(CCObject* sender) {
    if (!m_layer->m_inMainLayer) return m_layer->onBack(sender);
    MiniLayer::onClose(sender);
}

MiniTreasureRoom* MiniTreasureRoom::create()  {
    auto ret = new MiniTreasureRoom;
    if (ret->init()) return ret->autorelease(), ret;
    return delete ret, nullptr;
}
