//
//  MapLayer.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-10.
//
//

#include "MapLayer.h"
#include "game/MapUtil.h"
#include "game/GameManager.h"

#define DESIGN_TILES_HORIZONTAL_NUMS 13
#define DESIGN_TILES_VERTICAL_NUMS   9

void MapLayer::onEnter()
{
    Node::onEnter();
    auto mapSize = MapUtil::getInstance()->getMapSize();
    auto scaleFactor = GameManager::getInstance()->getScaleFactor();
    auto winSize = Director::getInstance()->getWinSize();
    auto isScroll = false;
    
    if(mapSize.width - DESIGN_TILES_HORIZONTAL_NUMS>0)
    {
        _rangeWidth = (mapSize.width-1)*TILE_WIDTH*scaleFactor-winSize.width;
        isScroll = true;
    }
    
    if(mapSize.height - DESIGN_TILES_VERTICAL_NUMS>0)
    {
        _rangeHeight = (mapSize.height-1)*TILE_HEIGHT*scaleFactor-winSize.height;
        isScroll = true;
    }
    
    if(isScroll)
    {
        scheduleUpdateWithPriority(1);
        _originPosition = getPosition();
        _offset = Point::ZERO;
    }
    
}

bool MapLayer::init()
{
    if(!Node::init())
    {
        return false;
    }
    
    return true;
}

void MapLayer::update(float delta)
{
    auto player = GameManager::getInstance()->getPlayer();
    auto pos = player->getPosition();
    auto worldPos = player->getParent()->convertToWorldSpace(pos);
    auto direction = GameManager::getInstance()->getWalkDirection();
//    log("wx:%f , wy:%f",worldPos.x,worldPos.y);
    auto range = 20;
    if(GameManager::getInstance()->getIsCollision()==false)
    {
        auto speed = GameManager::getInstance()->getSpeed()*GameManager::getInstance()->getScaleFactor();
        
        
        switch (direction) {
            case Player::kWalkRight:
                if(worldPos.x>VisibleRect::center().x+range)
                    _offset += speed;
                _offset.x = _offset.x>_rangeWidth?_rangeWidth:_offset.x;
                break;
            case Player::kWalkLeft:
                if(worldPos.x < VisibleRect::center().x-range)
                    _offset += speed;
                _offset.x = _offset.x<0?0:_offset.x;
                break;
            case Player::kWalkUp:
                if(worldPos.y>VisibleRect::center().y+range)
                    _offset += speed;
                _offset.y = _offset.y>0?0:_offset.y;
                break;
            case Player::kWalkDown:
                
                if(worldPos.y<VisibleRect::center().y-range)
                    _offset += speed;
                _offset.y = _offset.y<-_rangeHeight?-_rangeHeight:_offset.y;
                break;
            default:
                break;
        }
        setPosition(_originPosition+_offset*-1);
    }
}