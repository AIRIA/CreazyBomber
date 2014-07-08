//
//  BomberButton.h
//  CreazyBomber
//
//  Created by AIRIA on 14-5-30.
//
//

#ifndef __CreazyBomber__BomberButton__
#define __CreazyBomber__BomberButton__

#include "common/CommonHeaders.h"

class BomberButton:public Sprite
{
public:
    static BomberButton *create();
    virtual void onEnter();
    virtual void onExit();
protected:
    void addEventListeners();
    void _disableBomb(Ref *pSender);
};


#endif /* defined(__CreazyBomber__BomberButton__) */
