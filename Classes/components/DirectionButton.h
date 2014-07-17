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

class Player;

class DirectionButton:public Node
{
public:
    virtual bool init();
    CREATE_FUNC(DirectionButton);
protected:
    Player *player;
    Vector<Sprite*> m_vArrows;
    EventListenerTouchOneByOne *getInnerEventListener();
    
    CC_SYNTHESIZE(Node*, m_pCurrentDirectionArrow, CurrentDirectionArrow);
    /* 方向球的活动半径 */
    float radius = 80;
    /**
     * 获取方向球的有效坐标位置
     */
    Point getLimitPos(const cocos2d::Point &pos);
    /**
     * 判断行走的方向
     */
    void walk(float degree);
};

#endif /* defined(__CreazyBomber__DirectionButton__) */
