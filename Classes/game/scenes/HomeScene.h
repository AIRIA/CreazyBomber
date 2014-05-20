//
//  HomeScene.h
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-18.
//
//

#ifndef __CreazyBomber__HomeScene__
#define __CreazyBomber__HomeScene__

#include "base/BaseLayer.h"

class HomeScene:public BaseLayer
{
private:
    void __setBackButtonEnable(bool val,float delay=0.0f);
    
    void __delayRun(float dt,std::function<void()> func);
    
    void __showRoleSelectMenu();
    void __showGameModeSelectMenu();
    void __showSceneSelectMenu();
    void __showStageSelectMenu();
    void __showIcons();
    
    void __hideIcons();
    void __hideRoles();
    void __hideModes();
    void __hideScenes();
    void __hideStages();
    void __hideElements(Node *node,float duration);
    
    void __addBackMenu();
public:
    void onTexturesLoaded();
    virtual bool init();
    CREATE_FUNC(HomeScene);
};

#endif /* defined(__CreazyBomber__HomeScene__) */
