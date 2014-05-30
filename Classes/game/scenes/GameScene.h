//
//  GameScene.h
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-25.
//
//

#ifndef __CreazyBomber__GameScene__
#define __CreazyBomber__GameScene__

#include "base/BaseLayer.h"

class GameScene:public BaseLayer
{
public:
    virtual void onTexturesLoaded();
    virtual bool init();
    CREATE_FUNC(GameScene);
protected:
    void addUIComponents();
};

#endif /* defined(__CreazyBomber__GameScene__) */
