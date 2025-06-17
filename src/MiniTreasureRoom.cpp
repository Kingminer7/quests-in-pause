#include "MiniTreasureRoom.hpp"
#include "Geode/binding/CCMenuItemSpriteExtra.hpp"
#include "Geode/cocos/CCDirector.h"
#include "Geode/cocos/menu_nodes/CCMenu.h"
#include "Geode/ui/Layout.hpp"

bool MiniTreasureRoom::setup() {
  m_bgSprite->removeFromParent();
  CCTextureCache::get()->addImage("TreasureRoomSheet.png", false);
  CCSpriteFrameCache::get()->addSpriteFramesWithFile("TreasureRoomSheet.plist");

  auto director = CCDirector::sharedDirector();
  auto winSize = director->getWinSize();

  m_keyLab = CCLabelBMFont::create(fmt::format("{}", GameStatsManager::get()->getStat("21")).c_str(), "bigFont.fnt");
  m_keyLab->setScale(.65f);
  m_keyLab->setAnchorPoint({1, .5});
  m_mainLayer->addChildAtPosition(m_keyLab, Anchor::TopRight, {-55, -18});

  m_keySpr = CCSprite::createWithSpriteFrameName("GJ_bigKey_001.png");
  m_mainLayer->addChildAtPosition(m_keySpr, Anchor::TopRight, {-30, -18.5});

  m_goldLab = CCLabelBMFont::create(fmt::format("{}", GameStatsManager::get()->getStat("43")).c_str(), "bigFont.fnt");
  m_goldLab->setScale(.65f);
  m_goldLab->setAnchorPoint({1, .5});
  m_goldLab->setVisible(false);
  m_mainLayer->addChildAtPosition(m_goldLab, Anchor::TopRight, {-55, -18});

  m_goldSpr = CCSprite::createWithSpriteFrameName("GJ_bigGoldKey_001.png");
  m_goldSpr->setVisible(false);
  m_mainLayer->addChildAtPosition(m_goldSpr, Anchor::TopRight, {-30, -18.5});

  auto title = CCSprite::createWithSpriteFrameName("treasureRoomLabel_001.png");
  m_mainLayer->addChildAtPosition(title, Anchor::Top, {0, -42.f});

  m_buttonMenu->setID("button-menu");
  m_closeBtn->setID("back-button");
  m_closeBtn->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::TopLeft)->setOffset({25, -22}));
  m_closeBtn->setSprite(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"));

  CCMenu* pageNav = CCMenu::create();
  pageNav->setID("page-nav");
  pageNav->ignoreAnchorPointForPosition(false);
  m_mainLayer->addChildAtPosition(pageNav, Anchor::Center, {0, -30});

  CCMenuItemSpriteExtra* leftBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"), this, menu_selector(MiniTreasureRoom::onNav));
  leftBtn->setID("left-btn");
  pageNav->setContentHeight(leftBtn->getContentHeight());
  pageNav->addChildAtPosition(leftBtn, Anchor::Left, {24, 0});

  CCMenuItemSpriteExtra* rightBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"), this, menu_selector(MiniTreasureRoom::onNav));
  rightBtn->setID("right-btn");
  rightBtn->getChildByType<CCSprite*>(0)->setFlipX(true);
  pageNav->addChildAtPosition(rightBtn, Anchor::Right, {-24, 0});
  
  CCLayer* tierOne = CCLayer::create();
  CCLayer* tierTwo = CCLayer::create();
  CCLayer* bonuses = CCLayer::create();
  CCLayer* event = CCLayer::create();
  auto pages = CCArray::create();
  pages->addObject(tierOne);
  pages->addObject(tierTwo);
  pages->addObject(bonuses);
  pages->addObject(event);
  m_bs = BoomScrollLayer::create(pages, 0, true, nullptr, this);
  m_bs->setID("scroll-layer");
  m_bs->updateDots(0);
  m_bs->ignoreAnchorPointForPosition(false);
  m_mainLayer->addChildAtPosition(m_bs, Anchor::Center, {0, 0});

  #define CREATE_CHEST(tier, menu, id, pos, costSprite, rewardType) \
    { \
        GJChestSprite* chest = GJChestSprite::create((int) rewardType); \
        chest->setScale(.6f); \
        CCMenuItemSpriteExtra* chestBtn = CCMenuItemSpriteExtra::create(chest, this, menu_selector(MiniTreasureRoom::onChest)); \
        chestBtn->setContentSize(chest->getChildByType<CCSprite*>(0)->getContentSize()); \
        chest->setPosition(chestBtn->getContentSize() / 2); \
        chestBtn->setID(id); \
        menu->addChildAtPosition(chestBtn, Anchor::Center, pos); \
        \
        CCSprite* chestCost = CCSprite::createWithSpriteFrameName(costSprite); \
        chestCost->setScale(.8f); \
        tier->addChildAtPosition(chestCost, Anchor::Center, {pos.x, pos.y - 54}); \
        \
        CCLabelBMFont* chestCounter = CCLabelBMFont::create(fmt::format("{} / {}", GameStatsManager::get()->countUnlockedSecretChests(rewardType), GameStatsManager::get()->countSecretChests(rewardType)).c_str(), "bigFont.fnt"); \
        chestCounter->setColor(ccColor3B{187, 122, 107}); \
        chestCounter->setScale(.4f); \
        tier->addChildAtPosition(chestCounter, Anchor::Center, {pos.x, pos.y - 89}); \
    }

  // tier 1 page
  tierOne->setID("tier-one");
  
  CCSprite* tierOneTitle = CCSprite::createWithSpriteFrameName("tier1label_001.png");
  tierOneTitle->setScale(.8f);
  tierOne->addChildAtPosition(tierOneTitle, Anchor::Center, {0, 70});

  CCMenu* tierOneMenu = CCMenu::create();
  tierOneMenu->setID("chest-menu");
  tierOneMenu->ignoreAnchorPointForPosition(false);
  tierOne->addChildAtPosition(tierOneMenu, Anchor::Center, {0, -0});  
  CREATE_CHEST(tierOne, tierOneMenu, "one-key-chest", CCPoint(-100, 10), "chest_03_price_001.png", GJRewardType::SmallTreasure);
  CREATE_CHEST(tierOne, tierOneMenu, "five-key-chest", CCPoint(0, 10), "chest_04_price_001.png", GJRewardType::LargeTreasure);
  CREATE_CHEST(tierOne, tierOneMenu, "ten-key-chest", CCPoint(100, 10), "chest_05_price_001.png", GJRewardType::Key10Treasure);

  // tier 2 page
  tierOne->setID("tier-two");
  
  CCSprite* tierTwoTitle = CCSprite::createWithSpriteFrameName("tier2label_001.png");
  tierTwoTitle->setScale(.8f);
  tierTwo->addChildAtPosition(tierTwoTitle, Anchor::Center, {0, 70});
 
  CCMenu* tierTwoMenu = CCMenu::create();
  tierTwoMenu->setID("chest-menu");
  tierTwoMenu->ignoreAnchorPointForPosition(false);
  tierTwo->addChildAtPosition(tierTwoMenu, Anchor::Center, {0, -0});  
  CREATE_CHEST(tierTwo, tierTwoMenu, "twenty-five-key-chest", CCPoint(-100, 10), "chest_06_price_001.png", GJRewardType::Key25Treasure);
  CREATE_CHEST(tierTwo, tierTwoMenu, "fifty-key-chest", CCPoint(0, 10), "chest_07_price_001.png", GJRewardType::Key50Treasure);
  CREATE_CHEST(tierTwo, tierTwoMenu, "hundred-key-chest", CCPoint(100, 10), "chest_08_price_001.png", GJRewardType::Key100Treasure);

  return true;
}

void MiniTreasureRoom::onClose(CCObject* sender) {
  Popup::onClose(sender);
  CCTextureCache::get()->removeTextureForKey("TreasureRoomSheet.png");
  CCSpriteFrameCache::get()->removeSpriteFramesFromFile("TreasureRoomSheet.plist");
}

void MiniTreasureRoom::onNav(CCObject* sender) {
  auto btn = dynamic_cast<CCMenuItemSpriteExtra*>(sender);
  if (!btn) return;

  if (btn->getID() == "left-btn") {
    goToPage((m_page - 1) % 4);
  } else if (btn->getID() == "right-btn") {
    goToPage((m_page + 1) % 4);
  }
}

void MiniTreasureRoom::goToPage(int page) {
    
}

void MiniTreasureRoom::onChest(CCObject* sender) {
    auto btn = dynamic_cast<CCMenuItemSpriteExtra*>(sender);
    if (!btn) return;

    if (btn->getID() == "one-key-chest") {
        
    }
}

void MiniTreasureRoom::updatePageWithObject(cocos2d::CCObject* p0, cocos2d::CCObject* p1) {

}

MiniTreasureRoom* MiniTreasureRoom::create() {
    auto ret = new MiniTreasureRoom;
    auto ws = CCDirector::get()->getWinSize();
    if (ret && ret->initAnchored(ws.width, ws.height)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}