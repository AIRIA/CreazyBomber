//
//  MapObject.h
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-25.
//
//

#ifndef __CreazyBomber__MapObject__
#define __CreazyBomber__MapObject__

#include "common/CommonHeaders.h"
#include "game/objects/MapCell.h"

#define MAPOBJ_CREATE(CLASS) static CLASS *create(MapCell *mapCell){ \
auto tile = new CLASS(); \
if(tile&&tile->initWithSpriteFrameName(mapCell->getFileName())) \
{ \
tile->autorelease(); \
tile->setMapCell(mapCell); \
return tile; \
} \
CC_SAFE_DELETE(tile); \
return nullptr; \
}

class MapObject : public Sprite
{
public:
    virtual void onEnter();
    bool initWithMapCell(MapCell *mapCell);
    void createAnimation(MapCell *mapCell,CellAnimation *cellAnimation,std::string suffix="");
    
    CC_SYNTHESIZE(MapCell*, m_pMapCell, MapCell);
    CC_SYNTHESIZE(SpriteFrame*, m_pFirstFrame, FirstFrame);
    CC_SYNTHESIZE(int, m_iCol, Col);
    CC_SYNTHESIZE(int, m_iRow, Row);
    /**
     * 地图对象的类型(比如地图瓦片,出生点,怪物,箱子,树木 等等)
     **/
    CC_SYNTHESIZE(int, m_iType, Type);
};

/**
 *
 * 地图背景 以及静止不动的tile
 **/
class GroundTile:public MapObject
{
public:
    static GroundTile *create(MapCell *mapCell);
    bool initWithFileName(std::string name);
    void doAnimation();
};
/**
 * monster类型的tile 可以攻击英雄
 */
class Monster:public MapObject
{
public:
    static Monster *create(MapCell *mapCell);
    
    enum WalkDirection{
        kWalkUp,
        kWalkLeft,
        kWalkRight,
        kWalkDown
    };
    bool initWithMapCell(MapCell *mapCell);
    /**
     * 根据传入的方向 执行对应的动画
     * @param direct 
     **/
    void walk(WalkDirection direc);
};

#endif /* defined(__CreazyBomber__MapObject__) */
