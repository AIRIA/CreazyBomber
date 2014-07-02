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

class MonsterFire:public Node
{
public:
    virtual bool init();
    CREATE_FUNC(MonsterFire);
    virtual void onEnter();
    
    CC_SYNTHESIZE(std::string, m_sFileName, FileName);
    CC_SYNTHESIZE(int, m_iCol, Col);
    CC_SYNTHESIZE(int, m_iRow, Row);
};

#endif /* defined(__CreazyBomber__MonsterSkills__) */
