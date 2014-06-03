//
//  GameUILayer.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-3.
//
//

#include "GameUILayer.h"

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
    
    hp->setAnchorPoint(hpBg->getAnchorPoint());
    hp->setPosition(hpBg->getPosition());
    m_pLeft->addChild(hpBg);
    m_pLeft->addChild(hp);
    //玩家的装备信息
    auto playerInfo = SPRITE("role_info_bg.png");
    playerInfo->setAnchorPoint(Point(0.5f,1.0f));
    playerInfo->setPosition(DESIGN_WIDTH/2,DESIGN_HEIGHT-10);
    m_pBody->addChild(playerInfo);
    
    //定时器 血瓶
    auto hpMenuItem = MenuItemSprite::create(SPRITE("hp_icon_normal.png"), SPRITE("hp_icon_press.png"),SPRITE("hp_icon_disable.png"));
    auto timerBombItem = MenuItemSprite::create(SPRITE("timer_bomb_normal.png"), SPRITE("timer_bomb_press.png"),SPRITE("timer_bomb_disable.png"));
    hpMenuItem->setAnchorPoint(Point(1.0f,0.5f));
    hpMenuItem->setPosition(DESIGN_WIDTH-10,DESIGN_HEIGHT/2+30);
    timerBombItem->setAnchorPoint(hpMenuItem->getAnchorPoint());
    timerBombItem->setPosition(DESIGN_WIDTH-10,hpMenuItem->getPosition().y - hpMenuItem->getContentSize().height/2-40);
    auto menu = Menu::create(hpMenuItem,timerBombItem,nullptr);
    m_pRight->addChild(menu);
    menu->setPosition(Point::ZERO);
}

void GameUILayer::onEnter()
{
    BaseLayer::onEnter();
}

void GameUILayer::onExit()
{
    BaseLayer::onExit();
}