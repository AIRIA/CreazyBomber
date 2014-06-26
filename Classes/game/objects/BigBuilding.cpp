//
//  BigBuilding.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-27.
//
//

#include "BigBuilding.h"
#include "game/GameManager.h"
#include "game/MapUtil.h"

void BigBuilding::onEnter()
{
    MapObject::onEnter();
    auto mapCell = getMapCell();
    auto width = atoi(mapCell->getArgs().at(1)->getValue().c_str());
    auto height = atoi(mapCell->getArgs().at(1)->getValue().c_str());
    auto collision = mapCell->getArgs().at(2);
    auto val = collision->getValue();
    
    int ax = width*getAnchorPoint().x;
    setCol(getCol()-ax);
    
    auto start = atoi(val.substr(0,1).c_str());
    auto end = atoi(val.substr(2,1).c_str());
    
    for (auto i=0;i<height; i++) {
        for (auto j=start; j<=end; j++) {
            auto emptyObj = new EmptyObject();
            emptyObj->autorelease();
            emptyObj->setCol(getCol()+j);
            emptyObj->setRow(getRow()-i);
            GameManager::getInstance()->getMapTileLayer()->addChild(emptyObj);
            MapUtil::getInstance()->getCommonTiles().pushBack(emptyObj);
        }
    }
}

void BigBuilding::onExit()
{
    MapObject::onExit();
}

void BigBuilding::update(float delta)
{
    auto status = GameManager::getInstance()->getWalkDirection();
    if(status!=kWalkStand)
    {
        auto mapCell = getMapCell();
        auto height = atoi(mapCell->getArgs().at(1)->getValue().c_str());
        auto collision = mapCell->getArgs().at(2);
        auto val = collision->getValue();
        
        auto start = atoi(val.substr(0,1).c_str());
        auto end = atoi(val.substr(2,1).c_str());
        auto rect = getBoundingBox();
        rect.origin.x = rect.origin.x+start*TILE_WIDTH;
        rect.size.width = (end+1)*TILE_WIDTH;
        rect.size.height = height*TILE_HEIGHT;
        
        auto playerRect = GameManager::getInstance()->getPlayer()->getBoundingBox();
        auto isCollision = playerRect.intersectsRect(rect);
        if(isCollision)
        {
            GameManager::getInstance()->setIsCollision(isCollision);
        }
    }
}