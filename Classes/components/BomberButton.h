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
protected:
    void addEventListeners();
};

#endif /* defined(__CreazyBomber__BomberButton__) */
