#pragma once

void toggleGMFadeHook(bool enabled);
void toggleFMODHook(bool enabled);

// Based on code from Geode in Pause Menu
// https://github.com/hiimjasmine00/GeodeInPauseMenu/blob/master/src/GeodeInPauseMenu.cpp#L12-L37
template <class InpT = cocos2d::CCLayer*, class T = std::remove_pointer_t<InpT>>
geode::Ref<T> stealLayer(geode::Function<void()> func, bool music = false) {
    if (!music) toggleGMFadeHook(true);

    auto ccd = cocos2d::CCDirector::get();
    auto will = ccd->getDontCallWillSwitch();
    geode::Ref scene = ccd->getRunningScene();

    ccd->setDontCallWillSwitch(true);

    func();
    geode::Ref<T> stolen = nullptr;
    if (auto tr = geode::cast::typeinfo_cast<cocos2d::CCTransitionScene*>(ccd->getNextScene())) {
        if (tr->m_pInScene != nullptr) {
            stolen = tr->m_pInScene->getChildByType<T>(0);
        }
    }

    ccd->setDontCallWillSwitch(true);
    ccd->replaceScene(scene);
    ccd->setDontCallWillSwitch(will);
    ccd->m_pNextScene = nullptr;

    if (!music) toggleGMFadeHook(false);
    if (!stolen) return nullptr;
    stolen->removeFromParentAndCleanup(false);
    return std::move(stolen);
}

struct Intercepter : cocos2d::CCObject {
    using Callback = geode::Function<void(Intercepter* self, CCObject* sender)>;
    Callback callback;

    void callOriginal(CCObject* sender) {
        (origListener->*origSel)(sender);
    }

    void execute(CCObject* sender) {
        if (callback) callback(this, sender);
    }

    static Intercepter* create(cocos2d::CCMenuItem* orig, Callback callback) {
        auto ret = new Intercepter(orig, std::move(callback));
        ret->autorelease();
        return ret;
    }

protected:
    Intercepter(cocos2d::CCMenuItem* orig, Callback callback) {
        this->callback = std::move(callback);
        this->origListener = orig->m_pListener;
        this->origSel = orig->m_pfnSelector;
        orig->m_pListener = this;
        orig->m_pfnSelector = menu_selector(Intercepter::execute);
    }

    CCObject* origListener;
    cocos2d::SEL_MenuHandler origSel;
};

CCMenuItemSpriteExtra* createQuestButton(bool alt = false);
CCMenuItemSpriteExtra* createTreasureButton(cocos2d::CCNode* dialogParent, bool alt = false);
CCMenuItemSpriteExtra* createDailyButton(bool alt = false);
CCMenuItemSpriteExtra* createPathButton(bool alt = false);

bool checkChests();
bool checkQuests();
