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
    m_fScaleFactor = m_winSize.width/DESIGN_WIDTH;
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
    
    auto mapSize = MapUtil::getInstance()->getMapSize();
    auto mapSizeInPixle = MapUtil::getInstance()->getMapSizeInPixle();
    mapLayer->setContentSize(mapSizeInPixle);
    
    mapLayer->addChild(baseTileLayer);
    mapLayer->addChild(tmxLayer);
    mapLayer->addChild(borderLayer);
    mapLayer->addChild(commonTileLayer);
    
    /* mapLayer的布局分为两种情况 滚动和非滚动 */
    if(mapSize.height>9){
        mapLayer->setAnchorPoint(Point(0.5f,1.0f));
        mapLayer->setPosition(Point(m_winSize.width/2,m_winSize.height+40*m_fScaleFactor));
        mapLayer->setScale(m_fScaleFactor);
        addChild(mapLayer);
    }else{
        mapLayer->setAnchorPoint(Point(0.5f,0.5f));
        mapLayer->setPosition(Point(DESIGN_WIDTH/2,DESIGN_HEIGHT/2));
        m_pBody->addChild(mapLayer);
    }
}