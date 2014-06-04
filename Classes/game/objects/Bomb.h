//
//  Bomb.h
//  CreazyBomber
//
//  Created by AIRIA on 14-6-3.
//
//

#ifndef __CreazyBomber__Bomb__
#define __CreazyBomber__Bomb__

#include "game/objects/MapObject.h"

class Bomb:public MapObject
{
public:
    enum BombType{
        kBombTimer,
        kBombNormal
    };
    static Bomb *create(BombType type);
    virtual bool init();
    CC_SYNTHESIZE(BombType, m_BombType, BombType);
    void onEnter();
    void initBombAnimations();
    void bomb();
};

#endif /* defined(__CreazyBomber__Bomb__) */
