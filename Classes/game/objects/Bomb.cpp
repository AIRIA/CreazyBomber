//
//  Bomb.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-3.
//
//

#include "Bomb.h"

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
    runAction(RepeatForever::create(animate));
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

