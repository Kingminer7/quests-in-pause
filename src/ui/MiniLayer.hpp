#pragma once

#include "Geode/utils/cocos.hpp"
#include <Geode/ui/Popup.hpp>

template <class T>
class MiniLayer : public geode::Popup {
protected:
    bool init(T* layer, CCMenuItemSpriteExtra* closeBtn) {
        const auto ws = cocos2d::CCDirector::get()->getWinSize();
        if (!Popup::init(ws.width, ws.height)) return false;
        m_layer = layer;
        m_mainLayer->setVisible(false);
        addChild(m_layer);
        setID(fmt::format("Mini{}"_spr, geode::cocos::getObjectName(layer)));
        setUserFlag("undefined0.draggable-popups/undraggable-popup", true);
        if (auto geodify = layer->getChildByID("omgrod.geodify/swelvy-background")) {
            addBackgroundNode(geodify);
        }
        if (closeBtn) {
            closeBtn->m_pfnSelector = menu_selector(MiniLayer::onClose);
            closeBtn->m_pListener = this;
        }

        return true;
    }

    bool m_closing = false;
    T* m_layer = nullptr;
    std::vector<CCNode*> m_backgrounds;
    bool m_hiddenBg = false;
public:
    static MiniLayer* create(T* layer, CCMenuItemSpriteExtra* closeBtn) {
        auto ret = new MiniLayer;
        if (ret->init(layer, closeBtn)) return ret->autorelease(), ret;
        return delete ret, nullptr;
    }

    void onClose(CCObject* sender) override {
        if (m_closing) return;
        setKeyboardEnabled(false);
        setKeypadEnabled(false);
        setTouchEnabled(false);
        setMouseEnabled(false);
        m_layer->setKeyboardEnabled(false);
        m_layer->setKeypadEnabled(false);
        m_layer->setTouchEnabled(false);
        m_layer->setMouseEnabled(false);
        m_layer->stopAllActions();
        m_layer->runAction(
            cocos2d::CCSequence::createWithTwoActions(
                cocos2d::CCMoveTo::create(0.3, {0, cocos2d::CCDirector::sharedDirector()->getWinSize().height + 5}),
                geode::cocos::CallFuncExt::create([this] {
                    removeFromParent();
                })));
        if (m_hiddenBg) runAction(cocos2d::CCFadeTo::create(0.3, 0));
        m_closing = true;
    }

    void show() override {
        if (m_noElasticity) return Popup::show();
        setOpacity(0);
        if (!m_backgrounds.empty() && geode::Mod::get()->getSettingValue<bool>("transparent")) {
            for (auto bg : m_backgrounds) bg->setVisible(false);
            m_hiddenBg = true;
            runAction(cocos2d::CCFadeTo::create(0.5, 150));
        }
        const auto dir = cocos2d::CCDirector::sharedDirector();
        m_noElasticity = true;
        Popup::show();
        m_mainLayer->setVisible(false);
        m_noElasticity = false;
        m_layer->setPosition({0, dir->getWinSize().height + 5});
        m_layer->stopAllActions();
        m_layer->runAction(cocos2d::CCEaseBounceOut::create(cocos2d::CCMoveTo::create(0.5, {0, 0})));
    }

    bool isClosing() const {
        return m_closing;
    }

    void addBackgroundNode(CCNode* node) {
        m_backgrounds.push_back(node);
    }

    std::vector<CCNode*> getBackgroundNodes() {
        return m_backgrounds;
    }

    T* getLayer() {
        return m_layer;
    }
};
