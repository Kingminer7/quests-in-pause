#pragma once

#include <Geode/ui/Popup.hpp>

class RewardsPopup : public geode::Popup {
protected:
	bool init() override;
public:
	static RewardsPopup* create();
};
