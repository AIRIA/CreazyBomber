//
//  Bomb.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-3.
//
//

#include "Bomb.h"
#include "game/GameManager.h"
#include "game/MapUtil.h"

void Bomb::onEnter()
{
    Sprite::onEnter();
    char animationName[50];
    if (getBombType()==kBombTimer) {
        sprintf(animationName, "%s-timer-zd.png",GameConfig::selectedRoleName.c_str());
    }
    else
    {
        sprintf(animationName, "%s-zd.png",GameConfig::selectedRoleName.c_str());
    }
    auto animate = Animate::create(AnimationCache::getInstance()->getAnimation(animationName));
    auto animateCall = CallFunc::create([&]()->void{
        this->bomb();
    });
    runAction(Sequence::create(Repeat::create(animate, 5),animateCall, NULL));
}

Bomb *Bomb::create(Bomb::BombType type)
{
    auto bomb = new Bomb();
    if(bomb&&bomb->init())
    {
        bomb->autorelease();
        bomb->setBombType(type);
        return bomb;
    }
    CC_SAFE_FREE(bomb);
    return nullptr;
}

bool Bomb::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    initBombAnimations();
    return true;
}

void Bomb::initBombAnimations()
{
    auto createBombAnimation = [](std::string fileName,int frameNum)->void{
        if(AnimationCache::getInstance()->getAnimation(fileName))
        {
            return;
        }
        
        auto bombFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
        auto texture = bombFrame->getTexture();
        auto rect = bombFrame->getRect();
        auto size = rect.size;
        auto frameWidth = size.width/frameNum;
        auto frameHeight = size.height;
        
        Vector<SpriteFrame*> frameVec;
        for(auto i=0;i<frameNum;i++)
        {
            auto frameRect = Rect(rect.origin.x+frameWidth*i,rect.origin.y,frameWidth,frameHeight);
            auto frame = SpriteFrame::createWithTexture(texture, frameRect);
            frameVec.pushBack(frame);
        }
        
        auto animation = Animation::createWithSpriteFrames(frameVec);
        animation->setDelayPerUnit(0.2f);
        AnimationCache::getInstance()->addAnimation(animation, fileName);
    };
    
    createBombAnimation(GameConfig::selectedRoleName+"-timer-zd.png",2);
    createBombAnimation(GameConfig::selectedRoleName+"-zd.png",2);
    for (auto i=1; i<=9; i++) {
        char name[50];
        sprintf(name, "%s-zd0%d.png",GameConfig::selectedRoleName.c_str(),i);
        createBombAnimation(name,9);
    }
}

void Bomb::bomb()
{
    removeFromParent();
    auto getAnimateByName = [](std::string animationName)->Animate*{
        auto animation = AnimationCache::getInstance()->getAnimation(animationName);
        return Animate::create(animation);
    };
    auto removeHandler = CallFuncN::create([](Ref *pSender)->void{
        auto node = static_cast<Node*>(pSender);
        node->removeFromParent();
    });
    auto center = Sprite::create();
    center->runAction(Sequence::create(getAnimateByName((GameConfig::selectedRoleName+"-zd01.png").c_str()),removeHandler,NULL));
    center->setPosition(getPosition());
    center->setAnchorPoint(getAnchorPoint());
    GameManager::getInstance()->getMapTileLayer()->addChild(center);
    
    std::vector<Point> directions = {Point(-1,0),Point(0,-1),Point(+1,0),Point(0,+1)};
    for(auto i=0;i<directions.size();i++)
    {
        for(auto j=1;j<=m_iPower;j++)
        {
            char name[50];
            if(j==m_iPower)
            {
                sprintf(name, "%s-zd0%d.png",GameConfig::selectedRoleName.c_str(),2*(i+1));
            }
            else
            {
                sprintf(name, "%s-zd0%d.png",GameConfig::selectedRoleName.c_str(),2*(i+1)+1);
            }
            
            auto targetCoordiante = Point(getCol(),getRow())+directions[i]*j;
            log("row:%f,col:%f",targetCoordiante.x,targetCoordiante.y);
            auto tile = MapUtil::getInstance()->getMapObjectByCoordinate(targetCoordiante);
            if(tile&&tile!=GameManager::getInstance()->getPlayer())
            {
                tile->doTileDestory();
                break;
            }
            auto bombFire = Sprite::create();
            
            bombFire->setAnchorPoint(getAnchorPoint());
            bombFire->setPosition(GameManager::getInstance()->getPlayer()->convertCoordinate2Point(targetCoordiante));
            bombFire->runAction(Sequence::create(getAnimateByName(name),removeHandler->clone(), NULL));
            bombFire->setZOrder(getZOrder());
            GameManager::getInstance()->getMapTileLayer()->addChild(bombFire);
            
        }
    }
}

