//
//  BaseScene.h
//  CreazyBomber
//
//  Created by AIRIA on 14-7-14.
//
//

#ifndef __CreazyBomber__BaseScene__
#define __CreazyBomber__BaseScene__

#include "BaseLayer.h"

class BaseScene:public BaseLayer
{
public:
    virtual void onEnter();
    virtual void onExit();
    virtual void onTexturesLoaded();
    virtual bool init();
    CREATE_FUNC(BaseScene);
protected:
    virtual void __loadAssets();
};

#endif /* defined(__CreazyBomber__BaseScene__) */
