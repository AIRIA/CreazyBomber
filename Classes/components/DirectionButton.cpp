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
    kDirectionWrapper
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
    
    auto createDirec = [&](std::string name,const Point &ap,const Point &pos,DirectionButtonTag tag)->Sprite*{
        auto direc = SPRITE(name);
        direc->setPosition(pos);
        direc->setAnchorPoint(ap);
        direc->setTag(tag);
        direc->setOpacity(0);
        m_vArrows.pushBack(direc);
        return direc;
    };
    
    auto up = createDirec("up.png",Point(0.5f,1.0f),Point(0,size.height/2-5),kDirectionUp);
    auto left = createDirec("left.png",Point(0.0f,0.5f),Point(-size.width/2+5,0),kDirectionLeft);
    auto right = createDirec("right.png",Point(1.0f,0.5f),Point(size.width/2-5,0),kDirectionRight);
    auto down = createDirec("down.png",Point(0.5,0.0f),Point(0,-size.height/2+5),kDirectionDown);
    
    wrapper->setPosition(Point(size.width/2,size.height/2));
    wrapper->addChild(bg);
    wrapper->addChild(direc_inner);
    wrapper->addChild(up);
    wrapper->addChild(down);
    wrapper->addChild(left);
    wrapper->addChild(right);
    addChild(wrapper,0,kDirectionWrapper);
    
    direc_inner->getEventDispatcher()->addEventListenerWithSceneGraphPriority(getInnerEventListener(), direc_inner);
    direc_inner->setOpacity(128);
    
    return true;
}

EventListenerTouchOneByOne *DirectionButton::getInnerEventListener()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [](Touch *touch,Event *event)->bool{
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        auto locationInNode = target->convertToNodeSpace(touch->getLocation());
        auto size = target->getContentSize();
        auto rect = Rect(0,0,size.width,size.height);
        if(rect.containsPoint(locationInNode))
        {
            target->setOpacity(255);
            return true;
        }
        return false;
    };
    
    touchListener->onTouchEnded = [&](Touch *touch,Event *event)->void{
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        target->setOpacity(128);
        target->setPosition(Point::ZERO);
        getCurrentDirectionArrow()->setOpacity(128);
        GameManager::getInstance()->setWalkDirection(Player::WalkDirection::kWalkStand);
    };
    
    touchListener->onTouchMoved = [&](Touch *touch,Event *event)->void{
        auto radius = 80;
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        auto offset = target->getParent()->convertToNodeSpace(touch->getLocation());
        
        float angle = offset.getAngle();
        auto limitPos = [&](const Point &pos)->Point{
            
            
            auto x = 0;
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
        };
        
        auto arrowIt = m_vArrows.begin();
        while(arrowIt!=m_vArrows.end())
        {
            (*arrowIt)->setOpacity(128);
            arrowIt++;
        }
        
        auto degree = CC_RADIANS_TO_DEGREES(angle);
        auto wrapper = this->getChildByTag(kDirectionWrapper);
        if(degree>=-135&&degree<-45)
        {
            GameManager::getInstance()->setWalkDirection(Player::WalkDirection::kWalkDown);
            this->setCurrentDirectionArrow(wrapper->getChildByTag(kDirectionDown));
        }
        else if(degree>=-45&&degree<45)
        {
            GameManager::getInstance()->setWalkDirection(Player::WalkDirection::kWalkRight);
            this->setCurrentDirectionArrow(wrapper->getChildByTag(kDirectionRight));
        }
        else if(degree>=45&&degree<135)
        {
            GameManager::getInstance()->setWalkDirection(Player::WalkDirection::kWalkUp);
            this->setCurrentDirectionArrow(wrapper->getChildByTag(kDirectionUp));
        }
        else
        {
            GameManager::getInstance()->setWalkDirection(Player::WalkDirection::kWalkLeft);
            this->setCurrentDirectionArrow(wrapper->getChildByTag(kDirectionLeft));
        }
        this->getCurrentDirectionArrow()->setOpacity(255);
        target->setPosition(limitPos(offset));
    };
    
    return touchListener;
}
