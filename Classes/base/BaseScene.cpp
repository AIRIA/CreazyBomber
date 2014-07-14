//
//  BaseScene.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-7-14.
//
//

#include "BaseScene.h"
#include "game/GameManager.h"

void BaseScene::onEnter()
{
    BaseLayer::onEnter();
    NotificationCenter::getInstance()->removeAllObservers(this);
}

void BaseScene::onExit()
{
    BaseLayer::onExit();
}

bool BaseScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    manager = GameManager::getInstance();
    m_winSize = Director::getInstance()->getWinSize();
    m_fScaleFactor = m_winSize.height/DESIGN_HEIGHT;
    return true;
}

void BaseScene::__loadAssets()
{
    onTexturesLoaded();
}

void BaseScene::onTexturesLoaded()
{
    
}