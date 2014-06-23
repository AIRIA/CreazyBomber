//
//  PlayerInfoParam.h
//  CreazyBomber
//
//  Created by AIRIA on 14-6-23.
//
//

#ifndef __CreazyBomber__PlayerInfoParam__
#define __CreazyBomber__PlayerInfoParam__

#include "common/CommonHeaders.h"

class PlayerInfoParam:public Ref
{
public:
    virtual bool init(){return true;};
    CREATE_FUNC(PlayerInfoParam);
    enum TYPE{
        kTypeBomb,
        kTypeShoe,
        kTypePower,
        kTypeCoin
    };
    
    CC_SYNTHESIZE(TYPE, m_eType, Type);
    CC_SYNTHESIZE(int, m_iValue, Value);
};

#endif /* defined(__CreazyBomber__PlayerInfoParam__) */
