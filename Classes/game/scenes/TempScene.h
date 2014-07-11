//
//  TempScene.h
//  CreazyBomber
//
//  Created by AIRIA on 14-7-11.
//
//

#ifndef __CreazyBomber__TempScene__
#define __CreazyBomber__TempScene__

#include "common/CommonHeaders.h"

class TempScene:public Layer
{
public:
    virtual bool init();
    static Scene *scene();
    CREATE_FUNC(TempScene);
    virtual void onEnter();
};

#endif /* defined(__CreazyBomber__TempScene__) */
