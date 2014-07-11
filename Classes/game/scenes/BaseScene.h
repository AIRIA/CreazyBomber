//
//  BaseScene.h
//  CreazyBomber
//
//  Created by AIRIA on 14-7-11.
//
//

#ifndef __CreazyBomber__BaseScene__
#define __CreazyBomber__BaseScene__

#include "base/BaseLayer.h"

class BaseScene:public BaseLayer
{
public:
    /* run self scene */
    void run();
    /* invoke on asset load complete */
    virtual void onTexturesLoaded();
    virtual bool init();
    CREATE_FUNC(BaseLayer);
    virtual void onEnter();
    virtual void onExit();
};

#endif /* defined(__CreazyBomber__BaseScene__) */
