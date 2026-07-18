#include "MiniTreasureRoom.hpp"

#include "MiniShopLayer.hpp"
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
            whose->m_pListener = this; \
            whose->m_pfnSelector = menu_selector(MiniTreasureRoom::interceptedShopBtn); \
        } else { \
            log::error("Could not find shop {}!", #whose); \
        }

        replaceShop(scratch);
        replaceShop(creator);
        replaceShop(mechanic);
        replaceShop(diamond);

        if (auto jam = typeinfo_cast<CCMenuItemSpriteExtra*>(sm->getChildByID("zilko.jam/jam-market-button"))) {
            m_jamListener = jam->m_pListener;
            m_jamSelector = jam->m_pfnSelector;
            jam->m_pListener = this;
            jam->m_pfnSelector = menu_selector(MiniTreasureRoom::interceptedJamBtn);
        }
    }

    return true;
}

void MiniTreasureRoom::onClose(CCObject* sender) {
    if (!m_layer->m_inMainLayer) return m_layer->onBack(sender);
    MiniLayer::onClose(sender);
}

void MiniTreasureRoom::interceptedShopBtn(CCObject* sender) {
    MiniShopLayer::create(static_cast<ShopType>(sender->getTag()))->show();
}

void MiniTreasureRoom::interceptedJamBtn(CCObject* sender) {
    auto shop = stealLayer<JamMarketLayer>([this] {
        (m_jamListener->*m_jamSelector)(nullptr);
    });
    if (!shop) return;
    MiniJamShopLayer::create(shop)->show();
}

MiniTreasureRoom* MiniTreasureRoom::create()  {
    auto ret = new MiniTreasureRoom;
    if (ret->init()) return ret->autorelease(), ret;
    return delete ret, nullptr;
}
