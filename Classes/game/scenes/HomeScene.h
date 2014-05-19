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
    std::vector<Point> rolePos;
    int tag1 = 0;
    int tag2 = 0;
    int tag3 = 0;
    void __startMenuHandler(Ref *pSender);
    void __selectRole();
    void __selectMode();
    void __roleSelectHandler(Ref *pSender);
    
    
    
    void __hideRoles();
    void __hideModes();
    void __hideScenes();
    void __hideStages();
    void __hideElements(Node *node,float duration);
    
    void __addSelectModeUI();
    void __addSelectRoleUI();
    void __stageSelectHandler(Ref *pSender);
    void __addSelectStageUI();
    void __addSelectSceneUI();
    void __addBackMenu();
public:
    void onTexturesLoaded();
    virtual bool init();
    CREATE_FUNC(HomeScene);
};

#endif /* defined(__CreazyBomber__HomeScene__) */
