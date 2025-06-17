#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MiniTreasureRoom : public Popup<> {
    protected:
        bool setup() override;
	void onClose(CCObject* sender) override;
    public:
        static MiniTreasureRoom* create();
};
