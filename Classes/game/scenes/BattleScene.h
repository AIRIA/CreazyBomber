//
//  BattleScene.h
//  CreazyBomber
//
//  Created by AIRIA on 14-5-21.
//
//

#ifndef __CreazyBomber__BattleScene__
#define __CreazyBomber__BattleScene__

#include "base/BaseLayer.h"

class BattleScene:public BaseLayer
{
public:
    virtual void onTexturesLoaded();
    virtual bool init();
    CREATE_FUNC(BattleScene);
    
};

#endif /* defined(__CreazyBomber__BattleScene__) */
