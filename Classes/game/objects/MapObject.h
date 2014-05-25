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

class MapObject : public Sprite
{
public:
    inline void setType(int val){ m_iType = val;};
    inline int getType(){ return m_iType; };
protected:
    /**
     * 地图对象的类型(比如地图瓦片,出生点,怪物,箱子,树木 等等)
     **/
    int m_iType;
};

/**
 *
 * 地图背景 以及静止不动的tile
 **/
class GroundTile:public MapObject
{
public:
    static GroundTile *create(std::string name);
    bool initWithFileName(std::string name);
};
/**
 * monster类型的tile 可以攻击英雄
 */
class Monster:public MapObject
{
public:
    static Monster *create(std::string name);
    
    enum WalkDirection{
        kWalkUp,
        kWalkLeft,
        kWalkRight,
        kWalkDown
    };
    bool initWithMonsterName(std::string name);
    /**
     * 根据传入的方向 执行对应的动画
     * @param direct 
     **/
    void walk(WalkDirection direc);
    
protected:
    std::string m_sName;
};

#endif /* defined(__CreazyBomber__MapObject__) */
