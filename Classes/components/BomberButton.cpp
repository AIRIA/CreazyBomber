//
//  BomberButton.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-30.
//
//

#include "BomberButton.h"
#include "game/GameManager.h"

BomberButton *BomberButton::create()
{
    auto bomb = new BomberButton();
    if(bomb&&bomb->initWithSpriteFrameName("bomb_up.png"))
    {
        bomb->autorelease();
        bomb->addEventListeners();
        bomb->setPosition(VisibleRect::rightBottom());
        bomb->setAnchorPoint(Point(1.0f,0.0f));
        return bomb;
    }
    CC_SAFE_DELETE(bomb);

    return nullptr;
}

void BomberButton::addEventListeners()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [](Touch *touch,Event *event)->bool{
        auto target = static_cast<BomberButton*>(event->getCurrentTarget());
        auto locationInNode = target->convertToNodeSpace(touch->getLocation());
        auto size = target->getContentSize();
        auto rect = Rect(0,0,size.width,size.height);
        if(rect.containsPoint(locationInNode))
        {
            target->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bomb_down.png"));
            return true;
        }
        return false;
    };
    
    touchListener->onTouchEnded = [](Touch *touch,Event *event)->void{
        auto target = static_cast<BomberButton*>(event->getCurrentTarget());
        target->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bomb_up.png"));
        GameManager::getInstance()->getPlayer()->addBomb();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void BomberButton::onEnter()
{
    Sprite::onEnter();
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BomberButton::_disableBomb), DISABLE_BOMB_BUTTON, nullptr);
}

void BomberButton::onExit()
{
    Sprite::onExit();
    NotificationCenter::getInstance()->removeAllObservers(this);
}

void BomberButton::_disableBomb(cocos2d::Ref *pSender)
{
    setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bomb_down.png"));
    getEventDispatcher()->removeEventListenersForTarget(this);
}


