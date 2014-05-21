//
//  PerfectMenu.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-21.
//
//

#include "PerfectMenu.h"

bool PerfectMenu::isTouched = false;

PerfectMenu *PerfectMenu::create()
{
    return PerfectMenu::create(nullptr, nullptr);
}

PerfectMenu *PerfectMenu::create(cocos2d::MenuItem *item, ...)
{
    va_list args;
    va_start(args,item);
    
    PerfectMenu *ret = PerfectMenu::createWithItems(item, args);
    
    va_end(args);
    
    return ret;
}

PerfectMenu *PerfectMenu::createWithItems(MenuItem* item, va_list args)
{
    Vector<MenuItem*> items;
    if( item )
    {
        items.pushBack(item);
        MenuItem *i = va_arg(args, MenuItem*);
        while(i)
        {
            items.pushBack(i);
            i = va_arg(args, MenuItem*);
        }
    }
    
    return PerfectMenu::createWithArray(items);
}

PerfectMenu *PerfectMenu::createWithArray(const Vector<MenuItem*>& arrayOfItems)
{
    auto ret = new PerfectMenu();
    if (ret && ret->initWithArray(arrayOfItems))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

PerfectMenu *PerfectMenu::createWithItem(MenuItem* item)
{
    return PerfectMenu::create(item, nullptr);
}



bool PerfectMenu::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (isTouched||_state != Menu::State::WAITING || ! _visible || !_enabled)
    {
        return false;
    }
    
    for (Node *c = this->_parent; c != nullptr; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    
    _selectedItem = this->getItemForTouch(touch);
    if (_selectedItem)
    {
        _state = Menu::State::TRACKING_TOUCH;
        _selectedItem->selected();
        isTouched = true;
        return true;
    }
    
    return false;
}

void PerfectMenu::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    isTouched = false;
    Menu::onTouchEnded(touch, event);
}

void PerfectMenu::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchMoved] -- invalid state");
    MenuItem *currentItem = this->getItemForTouch(touch);
    if(currentItem==nullptr)
    {
        isTouched = false;
    }else{
        isTouched = true;
    }
    if (currentItem != _selectedItem)
    {
        if (_selectedItem)
        {
            _selectedItem->unselected();
        }
        _selectedItem = currentItem;
        if (_selectedItem)
        {
            _selectedItem->selected();
        }
    }
}

void PerfectMenu::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchCancelled] -- invalid state");
    this->retain();
    if (_selectedItem)
    {
        _selectedItem->unselected();
        isTouched = false;
    }
    _state = Menu::State::WAITING;
    this->release();
}

