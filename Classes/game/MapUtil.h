//
//  MapUtil.h
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-25.
//
//

#ifndef __CreazyBomber__MapUtil__
#define __CreazyBomber__MapUtil__

#include "common/CommonHeaders.h"
#include "game/objects/MapObject.h"
#include "game/objects/MapCell.h"
#include "game/objects/BigBuilding.h"
#include "game/objects/Player.h"
#include "game/objects/Bomb.h"

enum CellType{
    kCellTypeBornPlace    = 88,//英雄出生的地方
    kCellTypeMonster      = 100,//怪物
    kCellTypeGround       = 3,//普通的tile不能穿越 不能被炸坏
    kCellTypeTransfer     = 36,//传送门
    kCellTypeBigBuilding  = 51,//大型建筑
    kCellTypePlant        = 5,//可以晃动的植物
    kCellTypeBox          = 9,// 木头箱子 可以推动的
    kCellTypeCorpseFlower = 7,//食人花
    kCellTypeWanDou       = 55 ,//豌豆 可以发射炮弹
    kCellTypeDiCi         = 40,//地刺
    kCellTypeLvDai        = 8,//履带 可以让英雄加速或者减速
    kCellTypeShuShou      = 38 ,//瘴气
    kCellTypeMonsterHome  = 11,//怪物帐篷 可以生成怪物
    kCellTypeDiDong       = 44,//地洞 可以生成怪物
    kCellTypeFireWall     = 16,//火墙 可以产生火球
    kCellTypePengHuoKou   = 15,//喷火口 可以喷火
    kCellTypeEvilFire     = 4,// 鬼火 可以移动
    kCellTypeWom          = 42,//虫子
    kCellTypeSnowBall     = 59,//雪球 可以滚动
    kCellTypeIce          = 12,//冰面
    kCellBorder ,
    kCellTypeBomb,
    kCellTypeBombFire
};


#define GET_MAPOBJECT_FROM(vec,type) MapObject *getMapObjectFrom##vec(Vector< type *> mapObjs, const cocos2d::Point &coordiante) \
{ \
    auto it = mapObjs.begin(); \
    while(it!=mapObjs.end()) \
    { \
        auto obj = *it; \
        if(obj->getCol()==coordiante.x&&obj->getRow()==coordiante.y) \
        { \
            return obj; \
        } \
        it++; \
    } \
    return nullptr; \
} \

/**
 * 地图由三层组成:
 * 1.每个模式的常规背景 包括三种:丛林 冰川 墓地
 * 2.由tiled地图编辑器编辑生成的tmx层
 * 3.每个关卡对应一个障碍物和怪物坐标文件配置的层
 **/
class MapUtil
{
public:
    void clearMap();
    static MapUtil *getInstance();
    /**
     * 获取所有的地图元素类型
     */
    void initMapCells();
    /**
     * 获取最底层的tile层
     */
    Node *getBaseTileLayer();
    /**
     * 获取由tmx文件生成的地图层
     */
    Node *getTmxTileLayer();
    /**
     * 获取每个地图tmx地图文件对应的sec文件图层
     */
    Node *getCommonTileLayer();
    /**
     * 获取选择关卡的地图文件的名字
     * 在加载地图文件的时候使用
     **/
    std::string getMapName();
    /**
     * 获取tmx文件的名字
     **/
    std::string getTmxMapName();
    /**
     * 获取tmx地图对应的sec文件路径
     */
    std::string getSceMapName();
    /**
     * 获取地图的宽高
     **/
    void initMapSize();
    
    const Size getMapSizeInPixle();
    
    /**
     * 从指定的Vector集合中 获取到指定坐标的地图对象
     */
    GET_MAPOBJECT_FROM(MapObjectVector,MapObject);
    GET_MAPOBJECT_FROM(BombFireVector, BombFire);
    GET_MAPOBJECT_FROM(BombVector, Bomb);
    
    Monster *getMonsterByCoordinate(const Point &coordiante);
    
    Node *addTileMapBorder();
    
    void removeMapObject(MapObject *mapObj);
    
    Vector<MapObject*> &getMapObjects(){ return m_vMapObjects;};
    /**
     * 所有怪物的全部属性
     */
    Vector<MonsterProperty*> &getMonsterProperteis(){ return m_vMonsterProperteis;};
    /**
     * 当前全部的怪物集合
     */
    Vector<Monster*> &getMonsters(){ return m_vMonsters;};
    /**
     * 获取当前放置的全部炸弹
     */
    Vector<Bomb*> &getBomb(){return m_vBombs;};
    /**
     * 获取当前状态下显示的炸弹的火焰集合
     */
    Vector<BombFire*> &getBombFires(){ return m_vFires;};
    /**
     * 获取普通的 阻挡模块 包括 围墙--树木--木箱子
     */
    Vector<MapObject*> &getCommonTiles(){ return m_vCommonTiles; };
    
    void getMonsterInfos();
    
    MonsterProperty *getMonsterProperyById(int id);
    /**
     * 判断指定的坐标是不是地图的边界位置
     */
    bool isBorder(const Point &coordinate);
protected:
    
    /**
     *
     */
    std::string getBaseTileFullName();
    /**
     * 根据名称获取对应的地图元素
     */
    MapObject *getMapObject(std::string name);
    TMXTiledMap *m_pMap;
    Vector<MapCell*> m_vMapCells;
    Vector<MapObject*> m_vMapObjects;
    Vector<MonsterProperty*> m_vMonsterProperteis;
    Vector<Monster*> m_vMonsters;
    Vector<BombFire*> m_vFires;
    Vector<Bomb*> m_vBombs;
    /* 普通建筑 不具有任何攻击力和伤害的 可以阻挡穿透 */
    Vector<MapObject*> m_vCommonTiles;
    CC_SYNTHESIZE(float, m_fMapWidthInPixle, MapWidthInPixle);
    CC_SYNTHESIZE(float, m_fMapHeightInPixle, MapHeightInPixle);
    CC_SYNTHESIZE(Size, m_MapSize, MapSize);
};



#endif /* defined(__CreazyBomber__MapUtil__) */
