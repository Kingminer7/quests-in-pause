#include "MiniTreasureRoom.hpp"

bool MiniTreasureRoom::setup() {

  CCTextureCache::get()->addImage("TreasureRoomSheet.png", false);
  CCSpriteFrameCache::get()->addSpriteFramesWithFile("TreasureRoomSheet.plist");

  auto director = CCDirector::sharedDirector();
  auto winSize = director->getWinSize();

  auto keyLab = CCLabelBMFont::create(fmt::format("{}", GameStatsManager::get()->getStat("21")).c_str(), "bigFont.fnt");
  keyLab->setScale(.65f);
  keyLab->setAnchorPoint({1, .5});
  m_mainLayer->addChildAtPosition(keyLab, Anchor::TopRight, {-55, -18});

  auto keySpr = CCSprite::createWithSpriteFrameName("GJ_bigKey_001.png");
  m_mainLayer->addChildAtPosition(keySpr, Anchor::TopRight, {-30, -18.5});

  auto goldLab = CCLabelBMFont::create(fmt::format("{}", GameStatsManager::get()->getStat("43")).c_str(), "bigFont.fnt");
  goldLab->setScale(.65f);
  goldLab->setAnchorPoint({1, .5});
  goldLab->setVisible(false);
  m_mainLayer->addChildAtPosition(goldLab, Anchor::TopRight, {-55, -18});

  auto goldSpr = CCSprite::createWithSpriteFrameName("GJ_bigGoldKey_001.png");
  goldSpr->setVisible(false);
  m_mainLayer->addChildAtPosition(goldSpr, Anchor::TopRight, {-30, -18.5});

  auto title = CCSprite::createWithSpriteFrameName("treasureRoomLabel_001.png");
  m_mainLayer->addChildAtPosition(title, Anchor::Top, {0, -42.f});

  return true;
}

MiniTreasureRoom* MiniTreasureRoom::create() {
    auto ret = new MiniTreasureRoom;
    if (ret && ret->initAnchored(420, 320)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}