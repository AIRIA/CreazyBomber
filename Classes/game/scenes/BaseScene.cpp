//
//  BaseScene.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-7-11.
//
//

#include "BaseScene.h"
#include "game/GameManager.h"

bool BaseScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    manager = GameManager::getInstance();
    m_winSize = Director::getInstance()->getWinSize();
    m_fScaleFactor = m_winSize.height/DESIGN_HEIGHT;
}