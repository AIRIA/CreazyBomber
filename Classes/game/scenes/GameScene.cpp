//
//  GameScene.cpp
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-25.
//
//

#include "GameScene.h"
#include "game/MapUtil.h"
#include "game/objects/MapObject.h"

bool GameScene::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    textureFiles.push_back("textures/monster_1-hd");
    textureFiles.push_back("textures/scenetex_small-hd");
    textureFiles.push_back("textures/scenetex_medium");
    textureFiles.push_back("textures/scenetex_big");
    return true;
}

void GameScene::onTexturesLoaded()
{
    auto baseTileLayer = MapUtil::getInstance()->getBaseTileLayer();
    auto tmxLayer = MapUtil::getInstance()->getTmxTileLayer();
    m_pBody->addChild(baseTileLayer);
    m_pBody->addChild(tmxLayer);
    
    auto monster = Monster::create("md_M_kulou.png");
    monster->walk(Monster::WalkDirection::kWalkLeft);
    monster->setPosition(DESIGN_CENTER);
    m_pBody->addChild(monster);
    
}