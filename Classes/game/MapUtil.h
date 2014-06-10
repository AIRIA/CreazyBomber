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
    
    
};

#endif /* defined(__CreazyBomber__MapUtil__) */
