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
#include "game/data/PlayerInfoParam.h"

class GameScene:public BaseLayer
{
public:
    virtual void onEnter();
    virtual void onExit();
    virtual void onTexturesLoaded();
    virtual bool init();
    CREATE_FUNC(GameScene);
protected:
    void addUIComponents();
    void normalBombHandler(Ref *pSender);
    void nextLevel(Ref *pSender);
    void retry(Ref *pSender);
    void createPlayerItem(Ref *pSender);
};

#endif /* defined(__CreazyBomber__GameScene__) */
