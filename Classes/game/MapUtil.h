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
    kCellBorder
};

class MonsterProperty:public Ref
{
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
    CC_SYNTHESIZE(int, m_iSpeed, Speed);
    CC_SYNTHESIZE(int, m_iAi, Ai);
    CC_SYNTHESIZE(int, m_iWuDi, WuDi);
    CC_SYNTHESIZE(std::string,m_sSkills,Skills);
};

/**
 * 地图由三层组成:
 * 1.每个模式的常规背景 包括三种:丛林 冰川 墓地
 * 2.由tiled地图编辑器编辑生成的tmx层
 * 3.每个关卡对应一个障碍物和怪物坐标文件配置的层
 **/
class MapUtil
{
public:
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
    const Size &getMapSize();
    
    const Size getMapSizeInPixle();
    
    MapObject *getMapObjectByCoordinate(const Point &coordinate);
    
    Node *addTileMapBorder();
    
    void removeMapObject(MapObject *mapObj);
    
    Vector<MapObject*> &getMapObjects(){ return m_vMapObjects;};
    
    Vector<MonsterProperty*> &getMonsterProperteis(){ return m_vMonsterProperteis;};
    
    void getMonsterInfos();
    
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
};



#endif /* defined(__CreazyBomber__MapUtil__) */
