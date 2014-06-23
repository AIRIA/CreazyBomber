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
    virtual void onEnter();
    virtual void onExit();
    
    void initBombAnimations();
    void bomb();
    virtual void doTileDestory();
    /**
     * 更新UI层可以使用的炸弹的数量
     */
    void updateBombnum();

protected:
    bool _isBombed;
    CC_SYNTHESIZE(BombType, m_BombType, BombType);
    CC_SYNTHESIZE(int , m_iRow, Row);
    CC_SYNTHESIZE(int , m_iCol, Col);
    CC_SYNTHESIZE(int , m_iPower, Power);
    CC_SYNTHESIZE(float , m_iBombTime, BombTime);
    
    
};

class BombFire:public MapObject
{
public:
    static BombFire *create();
    virtual void onEnter();
    virtual void onExit();
};

#endif /* defined(__CreazyBomber__Bomb__) */
