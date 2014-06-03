//
//  DirectionButton.h
//  CreazyBomber
//
//  Created by AIRIA on 14-5-30.
//
//

#ifndef __CreazyBomber__DirectionButton__
#define __CreazyBomber__DirectionButton__

#include "common/CommonHeaders.h"

class DirectionButton:public Node
{
public:
    virtual bool init();
    CREATE_FUNC(DirectionButton);
    void walk();
    void stand();
protected:
    Vector<Sprite*> m_vArrows;
    EventListenerTouchOneByOne *getInnerEventListener();
    CC_SYNTHESIZE(Node*, m_pCurrentDirectionArrow, CurrentDirectionArrow);
};

#endif /* defined(__CreazyBomber__DirectionButton__) */
