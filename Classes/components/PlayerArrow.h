//
//  PlayerArrow.h
//  CreazyBomber
//
//  Created by AIRIA on 14-6-12.
//
//

#ifndef __CreazyBomber__PlayerArrow__
#define __CreazyBomber__PlayerArrow__

#include "common/CommonHeaders.h"

class PlayerArrow:public Node
{
public:
    static PlayerArrow *getInstance();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(PlayerArrow)
};

#endif /* defined(__CreazyBomber__PlayerArrow__) */
