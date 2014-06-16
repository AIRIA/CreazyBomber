//
//  ResultLayer.h
//  CreazyBomber
//
//  Created by AIRIA on 14-6-16.
//
//

#ifndef __CreazyBomber__ResultLayer__
#define __CreazyBomber__ResultLayer__

#include "common/CommonHeaders.h"

class ResultLayer:public Layer
{
public:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    CREATE_FUNC(ResultLayer);
    
protected:
    void _changeBgColor();
    void _gameOver(Ref *pSender);
    void _showResult(Ref *pSender);
    void _scoreAnimateSelector(float delta);
};

#endif /* defined(__CreazyBomber__ResultLayer__) */
