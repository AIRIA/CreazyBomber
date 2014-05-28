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
if(tile&&tile->initWithMapCell(mapCell)) \
{ \
tile->autorelease(); \
tile->setMapCell(mapCell); \
return tile; \
} \
CC_SAFE_DELETE(tile); \
return nullptr; \
}

enum TileStatusType{
    kTileStatusNormal = 0,
    kTileStatusDestory = 2,
    kTileStatusPrepare = 4,
    kTileStatusAttack = 6
};

class MapObject : public Sprite
{
public:
    virtual void onEnter();
    bool initWithMapCell(MapCell *mapCell);
    void createAnimation(MapCell *mapCell,CellAnimation *cellAnimation,std::string suffix="");
    virtual void run();
    /**
     * 被炮弹炸掉以后 执行此方法
     */
    virtual void doTileDestory();
    
    /**
     * 随机的时间发起攻击
     */
    virtual void doTileAttack();
    
    /**
     * tile正常状态动画
     */
    virtual void doTileAnimation();
    
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
};
/**
 * monster类型的tile 可以攻击英雄
 */
class Monster:public MapObject
{
public:
    static Monster *create(MapCell *mapCell);
    virtual void run();
    
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

/**
 * 炸弹人在上面走的时候会有速度的变化
 **/
class LvDai:public MapObject
{
public:
    MAPOBJ_CREATE(LvDai);
    virtual void doTileAnimation();
};

#endif /* defined(__CreazyBomber__MapObject__) */
