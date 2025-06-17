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

  #define CREATE_CHEST(tier, menu, id, pos, costSprite, rewardType, costScale, addCounter) \
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
        chestCost->setScale(costScale); \
        chestCost->setID(fmt::format("{}-cost", id)); \
        tier->addChildAtPosition(chestCost, Anchor::Center, {pos.x, pos.y - 54}); \
        \
        if (addCounter) { \
            CCLabelBMFont* chestCounter = CCLabelBMFont::create(fmt::format("{} / {}", GameStatsManager::get()->countUnlockedSecretChests(rewardType), GameStatsManager::get()->countSecretChests(rewardType)).c_str(), "bigFont.fnt"); \
            chestCounter->setColor(ccColor3B(187, 122, 107)); \
            chestCounter->setScale(.4f); \
            chestCounter->setID(fmt::format("{}-counter", id)); \
            tier->addChildAtPosition(chestCounter, Anchor::Center, {pos.x, pos.y - 89}); \
        } \
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
  CREATE_CHEST(tierOne, tierOneMenu, "one-key-chest", CCPoint(-100, 10), "chest_03_price_001.png", GJRewardType::SmallTreasure, .8f, true)
  CREATE_CHEST(tierOne, tierOneMenu, "five-key-chest", CCPoint(0, 10), "chest_04_price_001.png", GJRewardType::LargeTreasure, .8f, true)
  CREATE_CHEST(tierOne, tierOneMenu, "ten-key-chest", CCPoint(100, 10), "chest_05_price_001.png", GJRewardType::Key10Treasure, .8f, true)

  // tier 2 page
  tierTwo->setID("tier-two");
  
  CCSprite* tierTwoTitle = CCSprite::createWithSpriteFrameName("tier2label_001.png");
  tierTwoTitle->setScale(.8f);
  tierTwo->addChildAtPosition(tierTwoTitle, Anchor::Center, {0, 70});
 
  CCMenu* tierTwoMenu = CCMenu::create();
  tierTwoMenu->setID("chest-menu");
  tierTwoMenu->ignoreAnchorPointForPosition(false);
  tierTwo->addChildAtPosition(tierTwoMenu, Anchor::Center, {0, -0});  
  CREATE_CHEST(tierTwo, tierTwoMenu, "twenty-five-key-chest", CCPoint(-100, 10), "chest_06_price_001.png", GJRewardType::Key25Treasure, .8f, true)
  CREATE_CHEST(tierTwo, tierTwoMenu, "fifty-key-chest", CCPoint(0, 10), "chest_07_price_001.png", GJRewardType::Key50Treasure, .8f, true)
  CREATE_CHEST(tierTwo, tierTwoMenu, "hundred-key-chest", CCPoint(100, 10), "chest_08_price_001.png", GJRewardType::Key100Treasure, .8f, true)

  // bonus page
  bonuses->setID("bonuses");
  
  
 
  CCMenu* bonusesMenu = CCMenu::create();
  bonusesMenu->setID("chest-menu");
  bonusesMenu->ignoreAnchorPointForPosition(false);
  bonuses->addChildAtPosition(bonusesMenu, Anchor::Center, {0, -0});  
  CREATE_CHEST(bonuses, bonusesMenu, "50-milestone-chest", CCPoint(-100, 10), "chestSpecial_01_price_001.png", GJRewardType::Large, 1.f, false)
  CREATE_CHEST(bonuses, bonusesMenu, "100-milestone-chest", CCPoint(0, 10), "chestSpecial_02_price_001.png", GJRewardType::Large, 1.f, false)
  CREATE_CHEST(bonuses, bonusesMenu, "200-milestone-chest", CCPoint(100, 10), "chestSpecial_03_price_001.png", GJRewardType::Large, 1.f, false)

  // bonus page
  event->setID("bonuses");
 
  CCMenu* eventMenu = CCMenu::create();
  eventMenu->setID("chest-menu");
  eventMenu->ignoreAnchorPointForPosition(false);
  event->addChildAtPosition(eventMenu, Anchor::Center, {0, -0}); 

  GJChestSprite* chest = GJChestSprite::create((int)GJRewardType::Gold);
  chest->setScale(.6f);
  CCMenuItemSpriteExtra* chestBtn = CCMenuItemSpriteExtra::create(chest, this, menu_selector(MiniTreasureRoom::onChest));
  chestBtn->setContentSize(chest->getChildByType<CCSprite*>(0)->getContentSize());
  chest->setPosition(chestBtn->getContentSize() / 2);
  chestBtn->setID("event-chest");
  eventMenu->addChildAtPosition(chestBtn, Anchor::Center, CCPoint(0, 10));

  CCSprite* chestCost = CCSprite::createWithSpriteFrameName("chest_09_price_001.png");
  chestCost->setScale(.96f);
  chestCost->setID("event-chest-cost");
  event->addChildAtPosition(chestCost, Anchor::Center, {0, 10 - 54});

  CCLabelBMFont* chestCounter = CCLabelBMFont::create(fmt::format("{} / {}", GameStatsManager::get()->countUnlockedSecretChests(GJRewardType::Gold), GameStatsManager::get()->countSecretChests(GJRewardType::Gold)).c_str(), "bigFont.fnt");
  chestCounter->setColor(ccColor3B(100, 87, 112));
  chestCounter->setScale(.5f);
  chestCounter->setID("event-chest-counter");
  event->addChildAtPosition(chestCounter, Anchor::Center, {0, 10 + 72});
  
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

    if (btn->getID() == "event-chest") {
        onGoldChest();
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

void MiniTreasureRoom::onGoldChest() {
  GameStatsManager *statsManager = GameStatsManager::get();

  int goldKeyCount = statsManager->getStat("43");
  int chestId = 6001;

  while (chestId < 6021) {
    if (!statsManager->isSecretChestUnlocked(chestId)) {
      break;
    }
    chestId++;
  }

  if (goldKeyCount < 1) {
    DialogObject *dialog = DialogObject::create("The Key", "You need a <cy>Gold Key</c> to unlock this <cg>chest</c>.", 2, 1.0, false, ccWHITE);
    DialogLayer *dialogLayer = DialogLayer::createDialogLayer(dialog, nullptr, 2);
    dialogLayer->addToMainScene();

    CCDirector *director = CCDirector::sharedDirector();
    CCPoint center = director->getWinSize() * 0.5;
    dialogLayer->setPosition(center);
    dialogLayer->animateInRandomSide();
  }
  else if (chestId == 0x1785) {
    DialogObject *dialog = DialogObject::create("The Key", "All out of <cy>Gold Chests</c> right now. Come back later.", 2, 1.0, false, ccWHITE);
    DialogLayer *dialogLayer = DialogLayer::createDialogLayer(dialog, nullptr, 2);
    dialogLayer->addToMainScene();

    CCDirector *director = CCDirector::sharedDirector();
    CCPoint center = director->getWinSize() * 0.5;
    dialogLayer->setPosition(center);
    dialogLayer->animateInRandomSide();
  } else {
    auto rewardItem = statsManager->getRewardItem(GJRewardType::Gold);
    if (rewardItem && goldKeyCount > 0) {
      statsManager->setStat("43", goldKeyCount - 1);
      statsManager->preProcessReward(rewardItem);
      statsManager->registerRewardsFromItem(rewardItem);

      RewardUnlockLayer *rewardLayer = RewardUnlockLayer::create(rewardItem->m_chestID, nullptr);
      this->addChild(rewardLayer, 100);

      rewardLayer->playRewardEffect();
    //   FMODAudioEngine::getInstance()->fadeMusic(1.0, 100, 1.0, 0.2);

    //   CCCallFunc *callback = CCCallFunc::create(this, callfunc_selector(SecretRewardsLayer::onRewardComplete));
    //   CCDelayTime *delay = CCDelayTime::create(3.2);
    //   CCSequence *sequence = CCSequence::create(delay, callback, nullptr);
    //   this->runAction(sequence);
    }
  }
}