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
    
    /**
     * 如果tile具有攻击行为的话 调用此方法来执行动作
     */
    virtual void doAnimationWithAttack();
    
    Animate *getDefaultAnimate();
    /**
     * 根据当前的坐标获取所在的行列信息
     */
    Point getCurrentCoordinate();
    Point convertCoordinate2Point(const Point &coordinate);
    
#pragma mark ------------getter/setter-----------------
    CC_SYNTHESIZE(MapCell*, m_pMapCell, MapCell);
    CC_SYNTHESIZE(SpriteFrame*, m_pFirstFrame, FirstFrame);
    CC_SYNTHESIZE(int, m_iCol, Col);
    CC_SYNTHESIZE(int, m_iRow, Row);
    CC_SYNTHESIZE(Size, m_MapSizeInPixle, MapSizeInPixle);
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
 * 可以缓动的背景 但是没有攻击能力的tile对象
 */
class CommonTile:public MapObject
{
public:
    MAPOBJ_CREATE(CommonTile);
    virtual void run();
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
    virtual void onEnter();
    MAPOBJ_CREATE(LvDai);
    virtual void doTileAnimation();
};

/**
 * 鬼灯 可以移动
 */
class GuiHuo:public MapObject
{
public:
    MAPOBJ_CREATE(GuiHuo);
    virtual void run();
};

/**
 * 食人鱼 食人花 幽灵 吃人的建筑
 */
class ManEater:public MapObject
{
public:
    MAPOBJ_CREATE(ManEater);
    virtual void run();
};

/**
 *
 * 张嘴吐雪球的人
 */
class SnowBallMan:public MapObject
{
public:
    MAPOBJ_CREATE(SnowBallMan);
    virtual void run();
};

#endif /* defined(__CreazyBomber__MapObject__) */
