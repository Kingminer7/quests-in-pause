#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MiniTreasureRoom : public Popup<>, DynamicScrollDelegate {
    protected:
        bool setup() override;
	    void onClose(CCObject* sender) override;
        void onNav(CCObject* sender);
        void updatePageWithObject(cocos2d::CCObject* p0, cocos2d::CCObject* p1) override;

        int m_page = 0;

        CCLabelBMFont* m_keyLab;
        CCSprite* m_keySpr;
        CCLabelBMFont* m_goldLab;
        CCSprite* m_goldSpr;
        BoomScrollLayer* m_bs;
    public:
        static MiniTreasureRoom* create();

        void goToPage(int page);
        void onChest(CCObject* sender);
        void onGoldChest();
};
