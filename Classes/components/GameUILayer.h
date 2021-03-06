//
//  GameUILayer.h
//  CreazyBomber
//
//  Created by AIRIA on 14-6-3.
//
//

#ifndef __CreazyBomber__GameUILayer__
#define __CreazyBomber__GameUILayer__

#include "base/BaseLayer.h"

class GameUILayer:public BaseLayer
{
public:
    virtual void onEnter();
    virtual void onExit();
    virtual void onTexturesLoaded();
    virtual bool init();
    CREATE_FUNC(GameUILayer);
    
protected:
    void _updateHpHandler(Ref *pSender);
    void _updatePlayerInfoHandler(Ref *pSender);
    void _updateMonsterCount(Ref *pSender);
    void _showBossHp(Ref *pSender);
    void _updateBossHp(Ref *pSender);
    
    ProgressTimer *hpBar;
    ProgressTimer *bossHpBar;
};

#endif /* defined(__CreazyBomber__GameUILayer__) */
