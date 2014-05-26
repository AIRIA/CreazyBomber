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
    m_fScaleFactor = 1;
    textureFiles.push_back("textures/monster_1-hd");
    textureFiles.push_back("textures/scenetex_small-hd");
    textureFiles.push_back("textures/scenetex_medium");
    textureFiles.push_back("textures/scenetex_big");
    return true;
}

void GameScene::onTexturesLoaded()
{
    
    auto mapLayer = Node::create();
    auto baseTileLayer = MapUtil::getInstance()->getBaseTileLayer();
    auto tmxLayer = MapUtil::getInstance()->getTmxTileLayer();
    auto commonTileLayer = MapUtil::getInstance()->getCommonTileLayer();
    auto borderLayer = MapUtil::getInstance()->addTileMapBorder();
    
    auto mapSizeInPixle = MapUtil::getInstance()->getMapSizeInPixle();
    mapLayer->setContentSize(mapSizeInPixle);
    mapLayer->setAnchorPoint(Point(0.5f,0.0f));
    mapLayer->setPosition(Point(DESIGN_WIDTH/2,DESIGN_HEIGHT-mapSizeInPixle.height+40));
    mapLayer->addChild(baseTileLayer);
    mapLayer->addChild(tmxLayer);
    mapLayer->addChild(borderLayer);
    mapLayer->addChild(commonTileLayer);
    
    m_pBody->addChild(mapLayer);
    
//    auto monster = Monster::create("md_M_kulou.png");
//    monster->walk(Monster::WalkDirection::kWalkLeft);
//    monster->setPosition(DESIGN_CENTER);
//    m_pBody->addChild(monster);
    
}