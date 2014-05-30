//
//  Player.h
//  CreazyBomber
//
//  Created by AIRIA on 14-5-29.
//
//

#ifndef __CreazyBomber__Player__
#define __CreazyBomber__Player__

#include "common/CommonHeaders.h"
#include "game/objects/MapObject.h"

class Player:public MapObject
{
public:
    enum WalkDirection{
        kWalkUp,
        kWalkLeft,
        kWalkRight,
        kWalkDown,
        kWalkStand
    };
    /**
     * 根据用户选择的角色名字 来创建对应的角色
     */
    static Player *create(MapCell *mapCell);
    virtual bool init();
    virtual void run();
    void born();
    void walk(WalkDirection direction);
};

#endif /* defined(__CreazyBomber__Player__) */
