#pragma once

using namespace geode::prelude;

template <class T>
class MiniLayer : public Popup {
    protected:
        bool init(T* layer, CCMenuItemSpriteExtra* closeBtn) {
            const auto ws = CCDirector::get()->getWinSize();
            if (!Popup::init(ws.width, ws.height)) return false;
            m_layer = layer;
            m_mainLayer->setVisible(false);
            addChild(m_layer);
            setID("MiniLayer"_spr);
            setOpacity(0);

            if (closeBtn) {
                closeBtn->m_pfnSelector = menu_selector(MiniLayer::onClose);
                closeBtn->m_pListener = this;
            }

            return true;
        }

    public:
    static MiniLayer* create(T* layer, CCMenuItemSpriteExtra* closeBtn) {
        auto ret = new MiniLayer;
        if (ret->init(layer, closeBtn)) return ret->autorelease(), ret;
        return delete ret, nullptr;
    }

    void onClose(CCObject* sender) override {
        if (!m_layer->m_inMainLayer) return m_layer->onBack(sender);
        m_layer->stopAllActions();
        m_layer->runAction(
            CCSequence::createWithTwoActions(
                CCMoveTo::create(0.3, {0, CCDirector::sharedDirector()->getWinSize().height + 5}),
                CallFuncExt::create([this]{
                    removeFromParent();
            })));
    }
    void show() override {
        if (m_noElasticity) return Popup::show();
        const auto dir = CCDirector::sharedDirector();
        m_noElasticity = true;
        Popup::show();
        m_mainLayer->setVisible(false);
        m_noElasticity = false;
        m_layer->setPosition({0, dir->getWinSize().height + 5});
        m_layer->stopAllActions();
        m_layer->runAction(CCEaseBounceOut::create(CCMoveTo::create(0.5, {0, 0})));
    }

    T* m_layer;
};
