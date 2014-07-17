//
//  DirectionButton.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-30.
//
//

#include "DirectionButton.h"
#include "game/GameManager.h"

enum DirectionButtonTag{
    kDirectionUp,
    kDirectionLeft,
    kDirectionRight,
    kDirectionDown,
    kDirectionWrapper,
    kDirectionInnerBall
};

bool DirectionButton::init()
{
    if(!Node::init())
    {
        return false;
    }
    auto wrapper = Node::create();
    auto bg = SPRITE("Direction.png");
    auto size = bg->getContentSize();
    auto direc_inner = SPRITE("Direction_Inner.png");
    
    wrapper->setPosition(Point(size.width/2,size.height/2));
    wrapper->addChild(bg);
    wrapper->addChild(direc_inner,1,kDirectionInnerBall);
    addChild(wrapper,0,kDirectionWrapper);
    
    auto createDirec = [&](std::string name,const Point &ap,const Point &pos,DirectionButtonTag tag)->Sprite*{
        auto direc = SPRITE(name);
        direc->setPosition(pos);
        direc->setAnchorPoint(ap);
        direc->setTag(tag);
        direc->setOpacity(0);
        m_vArrows.pushBack(direc);
        wrapper->addChild(direc);
        return direc;
    };
    
    createDirec("up.png",Point(0.5f,1.0f),Point(0,size.height/2-5),kDirectionUp);
    createDirec("left.png",Point(0.0f,0.5f),Point(-size.width/2+5,0),kDirectionLeft);
    createDirec("right.png",Point(1.0f,0.5f),Point(size.width/2-5,0),kDirectionRight);
    createDirec("down.png",Point(0.5,0.0f),Point(0,-size.height/2+5),kDirectionDown);

    direc_inner->getEventDispatcher()->addEventListenerWithSceneGraphPriority(getInnerEventListener(), direc_inner);
    direc_inner->setOpacity(128);
    
    player = GameManager::getInstance()->getPlayer();
    
    return true;
}

Point DirectionButton::getLimitPos(const cocos2d::Point &pos)
{
    auto x = 0;
    auto angle = pos.getAngle();
    if(pos.x>0)
    {
        auto maxX = radius*cos(angle);
        x = pos.x>maxX?maxX:pos.x;
    }
    else
    {
        auto minX = radius*cos(angle);
        x = pos.x<minX?minX:pos.x;
    }
    
    auto y = 0;
    if(pos.y>0)
    {
        auto maxY = radius*sin(angle);
        y = pos.y>maxY?maxY:pos.y;
    }
    else
    {
        auto minY = radius*sin(angle);
        y = pos.y<minY?minY:pos.y;
    }
    
    return Point(x,y);
}

void DirectionButton::walk(float degree)
{
    /* 设置所有方向键的透明度为50% */
    auto arrowIt = m_vArrows.begin();
    while(arrowIt!=m_vArrows.end())
    {
        (*arrowIt)->setOpacity(128);
        arrowIt++;
    }
    
    auto wrapper = this->getChildByTag(kDirectionWrapper);
    if(degree>=-135&&degree<-45)
    {
        this->setCurrentDirectionArrow(wrapper->getChildByTag(kDirectionDown));
        player->walk(kWalkDown);
    }
    else if(degree>=-45&&degree<45)
    {
        this->setCurrentDirectionArrow(wrapper->getChildByTag(kDirectionRight));
        player->walk(kWalkRight);
    }
    else if(degree>=45&&degree<135)
    {
        this->setCurrentDirectionArrow(wrapper->getChildByTag(kDirectionUp));
        player->walk(kWalkUp);
    }
    else
    {
        this->setCurrentDirectionArrow(wrapper->getChildByTag(kDirectionLeft));
        player->walk(kWalkLeft);
    }
    this->getCurrentDirectionArrow()->setOpacity(255);
}

EventListenerTouchOneByOne *DirectionButton::getInnerEventListener()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [this](Touch *touch,Event *event)->bool{
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        auto locationInNode = target->convertToNodeSpace(touch->getLocation());
        auto size = target->getContentSize();
        auto rect = Rect(0,0,size.width,size.height);
        if(rect.containsPoint(locationInNode))
        {
            target->setOpacity(255);
            auto offset = target->getParent()->convertToNodeSpace(touch->getLocation());
            float angle = offset.getAngle();
            /* 设置方向球的位置 */
            auto pos = getLimitPos(offset);
            float distance = pos.getDistance(Point::ZERO);
            target->setPosition(pos);
            if(distance<=radius&&distance>=30)
            {
                auto degree = CC_RADIANS_TO_DEGREES(angle);
                this->walk(degree);
            }
            
            return true;
        }
        return false;
    };
    
    touchListener->onTouchEnded = [&](Touch *touch,Event *event)->void{
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        target->setOpacity(128);
        target->setPosition(Point::ZERO);
        if(getCurrentDirectionArrow())
        {
            getCurrentDirectionArrow()->setOpacity(128);
            
        }
        player->stand();
    };
    
    touchListener->onTouchMoved = [&](Touch *touch,Event *event)->void{
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        auto offset = target->getParent()->convertToNodeSpace(touch->getLocation());
        float angle = offset.getAngle();
        /* 设置方向球的位置 */
        auto pos = getLimitPos(offset);
        float distance = pos.getDistance(Point::ZERO);
        target->setPosition(pos);
        if(distance<30)
        {
            player->stand();
            return;
        }
        auto degree = CC_RADIANS_TO_DEGREES(angle);
        this->walk(degree);
    };
    
    return touchListener;
}
