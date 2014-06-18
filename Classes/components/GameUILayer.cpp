//
//  GameUILayer.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-3.
//
//

#include "GameUILayer.h"
#include "game/GameManager.h"
#include "game/MapUtil.h"

enum Tags
{
    kTagWraper,
    kTagLevelLabel,
    kTagMonsterCountLabel,
    kTagCoinLabel,
    kTagSpeedShoe,
    kTagBombLabel,
    kTagPowerLabel
};

bool GameUILayer::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    m_fScaleFactor = m_winSize.width/DESIGN_WIDTH;
    return true;
}

void GameUILayer::onTexturesLoaded()
{
    m_pLeft->setAnchorPoint(Point(0.0f,1.0f));
    m_pLeft->setPosition(VisibleRect::leftTop());
    
    //玩家控制的角色的头像
    auto playerIcon = SPRITE(GameConfig::selectedRoleName+"_icon_big.png");
    m_pLeft->addChild(playerIcon);
    playerIcon->setAnchorPoint(Point(0.0f,1.0f));
    playerIcon->setPosition(10, DESIGN_HEIGHT-10);
    //玩家的HP条
    auto hpBg = SPRITE("hp_value_bg.png");
    auto hp = SPRITE("hp_value.png");
    hpBg->setAnchorPoint(Point(0.0f,1.0f));
    hpBg->setPosition(10,DESIGN_HEIGHT - playerIcon->getContentSize().height - 20);
    
    hpBar = ProgressTimer::create(hp);
    hpBar->setAnchorPoint(Point(0.0f,1.0f));
    hpBar->setType(ProgressTimer::Type::BAR);
    hpBar->setMidpoint(Point::ZERO);
    hpBar->setBarChangeRate(Point(0,1));
    hpBar->setPercentage(100);
    hpBar->setPosition(hpBg->getPosition());
    m_pLeft->addChild(hpBg);
//    m_pLeft->addChild(hp);
    m_pLeft->addChild(hpBar);
    //玩家的装备信息
    auto infoNode = Node::create();
    infoNode->setScale(m_fScaleFactor);
    auto playerInfo = SPRITE("role_info_bg.png");
    playerInfo->setAnchorPoint(Point(0.5f,1.0f));
    infoNode->addChild(playerInfo);
    infoNode->setPosition(VisibleRect::top()-Point(100,10));
    auto shoe = Label::createWithBMFont("font/font_02.fnt", "0");
    auto bomb = Label::createWithBMFont("font/font_02.fnt", "0");
    auto power = Label::createWithBMFont("font/font_02.fnt", "0");
    auto coin = Label::createWithBMFont("font/font_02.fnt", "000000");
    shoe->setPosition(playerInfo->getContentSize().width/2*-1+50,-20);
    auto pad = 80;
    bomb->setPosition(shoe->getPosition().x+pad,shoe->getPosition().y);
    power->setPosition(bomb->getPosition().x+pad,shoe->getPosition().y);
    coin->setPosition(power->getPosition().x+pad,shoe->getPosition().y);
    coin->setAnchorPoint(Point(0,0.5));
    infoNode->addChild(shoe);
    infoNode->addChild(bomb);
    infoNode->addChild(power);
    infoNode->addChild(coin);
    addChild(infoNode);
    
    //定时器 血瓶
    auto hpMenuItem = MenuItemSprite::create(SPRITE("hp_icon_normal.png"), SPRITE("hp_icon_press.png"),SPRITE("hp_icon_disable.png"));
    auto timerBombItem = MenuItemSprite::create(SPRITE("timer_bomb_normal.png"), SPRITE("timer_bomb_press.png"),SPRITE("timer_bomb_disable.png"));
    hpMenuItem->setAnchorPoint(Point(1.0f,0.5f));
    hpMenuItem->setPosition(DESIGN_WIDTH-20,DESIGN_HEIGHT/2+30);
    timerBombItem->setAnchorPoint(hpMenuItem->getAnchorPoint());
    timerBombItem->setPosition(DESIGN_WIDTH-10,hpMenuItem->getPosition().y - hpMenuItem->getContentSize().height/2-40);
    auto menu = Menu::create(hpMenuItem,timerBombItem,nullptr);
    m_pRight->addChild(menu);
    menu->setPosition(Point::ZERO);
    
    
    //选择的模式以及关卡
    auto padding = 50;
    auto wrapper = Node::create();
    auto mode = SPRITE(GameConfig::selectedSceneName+"_main_sign.png");
    auto levelLabel = Label::createWithBMFont("font/font_01.fnt", __String::createWithFormat("%d",GameConfig::selectedLevel)->getCString());
    auto monstersLabel = Label::createWithBMFont("font/font_01.fnt", __String::createWithFormat("%d/%d",0,GameManager::getInstance()->getMonsterCount())->getCString());
    levelLabel->setPosition(mode->getPosition().x+mode->getContentSize().width+5,mode->getPosition().y);
    monstersLabel->setPosition(levelLabel->getPosition().x+levelLabel->getContentSize().width+padding,mode->getPosition().y);
    monstersLabel->setTag(kTagMonsterCountLabel);
    levelLabel->setTag(kTagLevelLabel);
    wrapper->setTag(kTagWraper);
    wrapper->addChild(mode);
    wrapper->addChild(levelLabel);
    wrapper->addChild(monstersLabel);
    wrapper->setScale(m_fScaleFactor);
    wrapper->setPosition(playerInfo->getContentSize().width/2+120+infoNode->getPosition().x,VisibleRect::top().y-30);
    addChild(wrapper);
}

void GameUILayer::onEnter()
{
    BaseLayer::onEnter();
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameUILayer::_updateHpHandler), UPDATE_HP, nullptr);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameUILayer::_onMonsterDestroy), MONSTER_DESTROY, nullptr);
}

void GameUILayer::onExit()
{
    BaseLayer::onExit();
    NotificationCenter::getInstance()->removeObserver(this, UPDATE_HP);
    NotificationCenter::getInstance()->removeObserver(this, MONSTER_DESTROY);
}

void GameUILayer::_updateHpHandler(cocos2d::Ref *pSender)
{
    auto player = GameManager::getInstance()->getPlayer();
    int *hurt = (int*)static_cast<Node*>(pSender)->getUserData();
    auto progressTo = ProgressFromTo::create(1.0f, player->getHP()+*hurt, player->getHP());
    hpBar->runAction(progressTo);
}

void GameUILayer::_onMonsterDestroy(cocos2d::Ref *pSender)
{
    auto monsterCountLabel = static_cast<Label*>(getChildByTag(kTagWraper)->getChildByTag(kTagMonsterCountLabel));
    monsterCountLabel->setString(__String::createWithFormat("%ld/%d",GameManager::getInstance()->getMonsterCount()-MapUtil::getInstance()->getMonsters().size(),GameManager::getInstance()->getMonsterCount())->getCString());
}