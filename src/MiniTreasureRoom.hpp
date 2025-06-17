#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MiniTreasureRoom : public Popup<> {
    protected:
        bool setup() override;
    public:
        static MiniTreasureRoom* create();
};