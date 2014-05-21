//
//  PerfectMenu.h
//  CreazyBomber
//
//  Created by AIRIA on 14-5-21.
//
//

#ifndef __CreazyBomber__PerfectMenu__
#define __CreazyBomber__PerfectMenu__

#include "common/CommonHeaders.h"

class PerfectMenu:public Menu
{
private:
    static bool isTouched;
public:
    static PerfectMenu *create();
    static PerfectMenu *create(MenuItem* item, ...) CC_REQUIRES_NULL_TERMINATION;
    static PerfectMenu *createWithArray(const Vector<MenuItem*>& arrayOfItems);
    static PerfectMenu *createWithItems(MenuItem *firstItem, va_list args);
    static PerfectMenu *createWithItem(MenuItem* item);
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch,Event* event);
    virtual void onTouchCancelled(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);

};

#endif /* defined(__CreazyBomber__PerfectMenu__) */
