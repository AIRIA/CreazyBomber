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

#define TILE_WIDTH 80
#define TILE_HEIGHT 80

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


class MonsterProperty:public Ref
{
public:
    virtual bool init(){return true;};
    CREATE_FUNC(MonsterProperty);
    CC_SYNTHESIZE(int, m_iID, ID);
    CC_SYNTHESIZE(std::string, m_sMonsterName, MonsterName);
    CC_SYNTHESIZE(int, m_iHP, HP);
    CC_SYNTHESIZE(int, m_iPower, Power);
    CC_SYNTHESIZE(int, m_iReward, Reward);
    CC_SYNTHESIZE(std::string, m_sFileName, FileName);
    CC_SYNTHESIZE(std::string, m_sImageFileName, ImageFileName);
    CC_SYNTHESIZE(int,m_iShiYe, ShiYe);
    CC_SYNTHESIZE(int, m_iZhuiZong, ZhuiZong);
    CC_SYNTHESIZE(int, m_iWidth, Width);
    CC_SYNTHESIZE(int, m_iHeight, Height);
    CC_SYNTHESIZE(int, m_iFrameNum, FrameNum);
    CC_SYNTHESIZE(int, m_iFootPos, FootPos);
    CC_SYNTHESIZE(float, m_fSpeed, Speed);
    CC_SYNTHESIZE(int, m_iAi, Ai);
    CC_SYNTHESIZE(int, m_iWuDi, WuDi);
    CC_SYNTHESIZE(std::string,m_sSkills,Skills);
};

class GameManager;

class MapObject : public Sprite
{
public:
    GameManager *manager;
    virtual void onEnter();
    virtual void onExit();
    bool initWithMapCell(MapCell *mapCell);
    void createAnimation(MapCell *mapCell,CellAnimation *cellAnimation,std::string suffix="");
    virtual void run();
    virtual void onAttack(){};
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
    
    virtual void update(float delta);
    
    Animate *getAnimateAt(int idx);
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
protected:
    int getArgAt(int idx);
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
    virtual void onExit();
};

/**
 * 可以晃动的背景 但是没有攻击能力的tile对象
 */
class CommonTile:public MapObject
{
public:
    MAPOBJ_CREATE(CommonTile);
    virtual void run();
    virtual void onExit();
};

/**
 * 传送门
 */
class TransferDoor:public MapObject
{
public:
    MAPOBJ_CREATE(TransferDoor);
    virtual void onEnter();
    virtual void onExit();
    virtual void run();
    virtual void update(float delta);
protected:
    void _enableTransfor(Ref *pSender);
};

#pragma mark monster类型的tile 可以攻击英雄
class Monster:public MapObject
{
public:
    static Monster *create(MapCell *mapCell);
    virtual void run();
    virtual void onEnter();
    virtual void update(float delta);
    virtual void doTileDestory();
    virtual void doTileAttack();
    
    bool initWithMapCell(MapCell *mapCell);
    /**
     * 根据传入的方向 执行对应的动画
     * @param direct 
     **/
    void walk(WalkDirection direc);
    
    /**
     * 怪物走过的路径 保存怪物的巡逻范围
     */
    std::vector<Point> &getRoutePoint(){return m_vRoutePoint;};
    
    /**
     * 定距离范围内放火
     */
    void fire(int length);
    /**
     * 召唤怪物
     */
    void callMonster(int type);
    /**
     * 发射子弹 飞行指定的距离
     */
    void bullet(int length);
    /**
     * 添加骨爪
     */
    void guzhua(int length);
    
    
protected:
    std::vector<WalkDirection> testDirections;
    std::vector<Point> m_vRoutePoint;
    int m_iBombCollisionFrames = 0;
    
    CC_SYNTHESIZE(MonsterProperty*, m_pMonsterProperty, MonsterProperty);
    CC_SYNTHESIZE(bool , m_bIsCollision, IsCollison);
    CC_SYNTHESIZE(Point, m_VecSpeed, VecSpeed);
    CC_SYNTHESIZE(Point, m_CornerPoint, CornerPoint);
    CC_SYNTHESIZE(WalkDirection, m_eDirection, Direction);
    CC_SYNTHESIZE(Point, m_nextCoordinate, NextCoordinate);
    CC_SYNTHESIZE(Point, m_prevPosition, PrevPosition);
    CC_SYNTHESIZE(bool,m_bCanMove,CanMove);
    /* 被撞到之后 判断出来要继续行走的方向 */
    CC_SYNTHESIZE(WalkDirection, m_smartDirection, SmartDirection);
    CC_SYNTHESIZE(bool, m_bSmartWalk, SmartWalk);
    CC_SYNTHESIZE(float, m_fSpeedRate, SpeedRate);
    
    void bossDead(Ref *pSender);
    
};

class MonsterBoss:public Monster
{
public:
    bool initWithCell(MapCell *mapCell);
    virtual void run();
};

#pragma mark 炸弹人在上面走的时候会有速度的变化
class LvDai:public MapObject
{
public:
    virtual void onEnter();
    MAPOBJ_CREATE(LvDai);
    virtual void doTileAnimation();
    virtual void update(float delta);
};

#pragma mark 鬼灯 可以移动
class GuiHuo:public MapObject
{
public:
    MAPOBJ_CREATE(GuiHuo);
    virtual void run();
};

#pragma mark 食人鱼 食人花 幽灵 吃人的建筑
class ManEater:public MapObject
{
public:
    MAPOBJ_CREATE(ManEater);
    virtual void run();
    virtual void update(float delta);
    virtual void onAttack();
};

#pragma mark 张嘴吐雪球的人

class SnowBallOrWorm;

class SnowBallMan:public MapObject
{
public:
    MAPOBJ_CREATE(SnowBallMan);
    virtual void run();
protected:
    void doAnimate();
    int cdTime = 0;
    SnowBallOrWorm *ball;
};

#pragma mark 地图的边界元素
class MapBorder:public MapObject
{
public:
    static MapBorder *createWithSpriteFrameName(const std::string &name);
    static MapBorder *createWithSpriteFrame(SpriteFrame *frame);
    virtual void onEnter();
};

#pragma mark ----------木箱子--------------------------------------------
class WoodBox:public MapObject
{
public:
    MAPOBJ_CREATE(WoodBox);
    virtual void update(float delta);
    virtual void onExit();
protected:
    bool _isMoving = false;
    CC_SYNTHESIZE(WalkDirection, m_eMovingDirection, MovingDirection);
public:
    Point m_Scale;
    Point m_Anchor;
};

#pragma mark------------空的占位对象----------------------------

class EmptyObject:public MapObject
{
public:
    static EmptyObject *createWithSpriteFrameName(const std::string &name);
    static EmptyObject *createWithSpriteFrame(SpriteFrame *frame);
    virtual void doTileDestory();
    virtual void onEnter();
    virtual void onExit();
};

#pragma mark-------------地洞------------------------------

class DiDong:public MapObject
{
public:
    MAPOBJ_CREATE(DiDong);
    virtual void update(float delta);
    virtual void run(){};
protected:
    bool m_bIsEnable = true;
    bool m_bIsAttack = false;
    Point m_pPlayerCoordiante;
};

#pragma mark ------------树手-----------------------

class ShuShou:public MapObject
{
public:
    MAPOBJ_CREATE(ShuShou);
    virtual void update(float delta);
    virtual void run();
    virtual void doTileAnimation();
};

#pragma mark ------------地刺-----------------------

class DiCi:public MapObject
{
public:
    MAPOBJ_CREATE(DiCi);
    virtual void update(float delta);
    virtual void run();
    virtual void doTileAnimation();
};

#pragma mark -------------火墙------------------

class FireWall:public MapObject
{
public:
    MAPOBJ_CREATE(FireWall);
    virtual void onEnter();
    virtual void update(float delta);
    virtual void run();
protected:
    Point _originPosition;
    float offsetX = 0;
};
#pragma mark -------------雪球和虫子------------------

class SnowBallOrWorm:public MapObject
{
public:
    MAPOBJ_CREATE(SnowBallOrWorm);
    virtual void onEnter();
    virtual void update(float delta);
    virtual void run();
    int direction = -1;
    int distance;
    bool isByMan = false; //是不是投雪球的人吐的雪球
protected:
    Point speed = Point::ZERO;
    Point _originPosition;
    
    float offset = 0;
    int cdTime = 0;
    bool isCanMove = true;
    void doMove();
};

#pragma mark---------------Monster Home---------------------------
class MonsterHome:public DiDong
{
public:
    MAPOBJ_CREATE(MonsterHome);
    virtual void run();
    virtual void update(float delta);
};
#endif /* defined(__CreazyBomber__MapObject__) */
