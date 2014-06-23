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
#include "game/utils/Util.h"
#include "game/data/PlayerInfoParam.h"

enum Tags
{
    kTagWraper=100,
    kTagLevelLabel,
    kTagMonsterCountLabel
};

bool GameUILayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    m_winSize = Director::getInstance()->getWinSize();
    m_fScaleFactor = m_winSize.height/DESIGN_HEIGHT;
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
    m_pLeft->addChild(hpBar);
    
    
    //定时器 血瓶 设置
    auto hpMenuItem = MenuItemSprite::create(SPRITE("hp_icon_normal.png"), SPRITE("hp_icon_press.png"),SPRITE("hp_icon_disable.png"));
    auto timerBombItem = MenuItemSprite::create(SPRITE("timer_bomb_normal.png"), SPRITE("timer_bomb_press.png"),SPRITE("timer_bomb_disable.png"));
    auto settingItem = MenuItemSprite::create(SPRITE("setting_normal.png"), SPRITE("setting_press.png"));
    settingItem->setAnchorPoint(Point(1.0f,1.0f));
    settingItem->setPosition(VisibleRect::rightTop()-Point(20,20));
    settingItem->setCallback([](Ref *pSender)->void{
        NotificationCenter::getInstance()->postNotification(SHOW_PAUSE);
    });
    
    hpMenuItem->setAnchorPoint(Point(1.0f,1.0f));
    hpMenuItem->setPosition(VisibleRect::right()+Point(-20,200));
    timerBombItem->setAnchorPoint(hpMenuItem->getAnchorPoint());
    timerBombItem->setPosition(hpMenuItem->getPosition()-Point(0,180));
    
    timerBombItem->setScale(m_fScaleFactor);
    hpMenuItem->setScale(m_fScaleFactor);
    settingItem->setScale(m_fScaleFactor);
    
    auto menu = Menu::create(settingItem,hpMenuItem,timerBombItem,nullptr);
    addChild(menu);
    menu->ignoreAnchorPointForPosition(false);
    menu->setAnchorPoint(Point::ZERO);
    menu->setPosition(Point::ZERO);
    
    
    //玩家的装备信息
    auto infoNode = Node::create();
    infoNode->setScale(m_fScaleFactor);
    auto playerInfo = SPRITE("role_info_bg.png");
    playerInfo->setAnchorPoint(Point(0.5f,1.0f));
    infoNode->addChild(playerInfo);

    auto shoe = Label::createWithBMFont("font/font_02.fnt", "0");
    auto bomb = Label::createWithBMFont("font/font_02.fnt", Util::itoa(GameManager::getInstance()->getBombNum()));
    auto power = Label::createWithBMFont("font/font_02.fnt", "0");
    auto coin = Label::createWithBMFont("font/font_02.fnt", "000000");
    shoe->setPosition(playerInfo->getContentSize().width/2*-1+40,-20);
    auto pad = 70;
    power->setPosition(shoe->getPosition().x+pad,shoe->getPosition().y);
    bomb->setPosition(power->getPosition().x+pad+10,shoe->getPosition().y);
    coin->setPosition(bomb->getPosition().x+pad+10,shoe->getPosition().y);
    coin->setAnchorPoint(Point(0,0.5));
    infoNode->addChild(shoe);
    infoNode->addChild(bomb);
    infoNode->addChild(power);
    infoNode->addChild(coin);
    addChild(infoNode);
    //选择的模式以及关卡
    auto mode = SPRITE(GameConfig::selectedSceneName+"_main_sign.png");
    auto levelLabel = Label::createWithBMFont("font/font_01.fnt", __String::createWithFormat("%d",GameConfig::selectedLevel)->getCString());
    auto monstersLabel = Label::createWithBMFont("font/font_01.fnt", __String::createWithFormat("%d/%d",0,GameManager::getInstance()->getMonsterCount())->getCString());
    mode->setPosition(coin->getPosition().x+coin->getContentSize().width+pad,shoe->getPosition().y);
    levelLabel->setPosition(mode->getPosition().x+mode->getContentSize().width+5,mode->getPosition().y);
    auto padding = 50;
    monstersLabel->setPosition(levelLabel->getPosition().x+levelLabel->getContentSize().width+padding,mode->getPosition().y);
    monstersLabel->setTag(kTagMonsterCountLabel);
    levelLabel->setTag(kTagLevelLabel);
    infoNode->setTag(kTagWraper);
    infoNode->addChild(mode);
    infoNode->addChild(levelLabel);
    infoNode->addChild(monstersLabel);
    infoNode->setContentSize(Size(monstersLabel->getContentSize().width+monstersLabel->getPosition().x,1));
    infoNode->setAnchorPoint(Point(0.5,1));
    infoNode->setPosition(VisibleRect::top());
    
    shoe->setTag(PlayerInfoParam::kTypeShoe);
    bomb->setTag(PlayerInfoParam::kTypeBomb);
    power->setTag(PlayerInfoParam::kTypePower);
    coin->setTag(PlayerInfoParam::kTypeCoin);
}

void GameUILayer::onEnter()
{
    BaseLayer::onEnter();
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameUILayer::_updateHpHandler), UPDATE_HP, nullptr);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameUILayer::_onMonsterDestroy), MONSTER_DESTROY, nullptr);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameUILayer::_updatePlayerInfoHandler), UPDATE_PLAYER_INFO, nullptr);
}

void GameUILayer::onExit()
{
    BaseLayer::onExit();
    NotificationCenter::getInstance()->removeObserver(this, UPDATE_HP);
    NotificationCenter::getInstance()->removeObserver(this, MONSTER_DESTROY);
    NotificationCenter::getInstance()->removeObserver(this, UPDATE_PLAYER_INFO);
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

void GameUILayer::_updatePlayerInfoHandler(cocos2d::Ref *pSender)
{
    auto param = static_cast<PlayerInfoParam*>(pSender);
    auto wrapper = getChildByTag(kTagWraper);
    auto label = static_cast<Label*>(wrapper->getChildByTag(param->getType()));
    
    auto action = label->getActionByTag(100);
    if(param->getValue()==0)
    {
        label->stopAllActions();
        label->setScale(1.0f);
    }else if(action==nullptr){
        auto scaleBig = ScaleBy::create(0.4f, 1.5f);
        auto scaleSmall = scaleBig->reverse();
        auto seq = Sequence::create(scaleBig,scaleSmall, NULL);
        auto rep = RepeatForever::create(seq);
        rep->setTag(100);
        label->runAction(rep);
    }
    if(param->getType()==PlayerInfoParam::kTypeCoin)
    {
        char buffer[50];
        sprintf(buffer, "%06d",param->getValue());
        label->setString(buffer);
    }else{
        label->setString(Util::itoa(param->getValue()));
    }
}
