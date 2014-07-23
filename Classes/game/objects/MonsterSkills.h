//
//  MonsterSkills.h
//  CreazyBomber
//
//  Created by AIRIA on 14-7-2.
//
//

#ifndef __CreazyBomber__MonsterSkills__
#define __CreazyBomber__MonsterSkills__

#include "common/CommonHeaders.h"
#include "game/objects/MapObject.h"
#include "game/objects/Bomb.h"

class MonsterFire:public Node
{
public:
    virtual bool init();
    CREATE_FUNC(MonsterFire);
    virtual void update(float delta);
    virtual void onEnter();
    
    CC_SYNTHESIZE(std::string, m_sFileName, FileName);
    CC_SYNTHESIZE(int, m_iCol, Col);
    CC_SYNTHESIZE(int, m_iRow, Row);
};

class MonsterDiCi:public MonsterFire
{
public:
    virtual bool init();
    CREATE_FUNC(MonsterDiCi);
};


class MonsterBomb:public Bomb
{
public:
    virtual bool init();
    static MonsterBomb *create(BombType type);
    virtual void updateBombnum() {};
    virtual void onEnter();
    virtual void onExit();
    virtual void initBombAnimations();
    virtual void bomb();
};

class MonsterBombFire:public BombFire
{
public:
    static MonsterBombFire *create();
//    virtual void onEnter();
//    virtual void onExit();
};

#endif /* defined(__CreazyBomber__MonsterSkills__) */
