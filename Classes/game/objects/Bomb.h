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
    void onEnter();
    void initBombAnimations();
    void bomb();
    
    CC_SYNTHESIZE(BombType, m_BombType, BombType);
    CC_SYNTHESIZE(int , m_iRow, Row);
    CC_SYNTHESIZE(int , m_iCol, Col);
    CC_SYNTHESIZE(int , m_iPower, Power);
    CC_SYNTHESIZE(float , m_iBombTime, BombTime);
    
};

#endif /* defined(__CreazyBomber__Bomb__) */
