#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/Popup.hpp>

class RewardsPopup : public geode::Popup {
    protected:
	bool init(CCMenuItemSpriteExtra* btn);
    public:
	static RewardsPopup* create(CCMenuItemSpriteExtra* btn);
};
