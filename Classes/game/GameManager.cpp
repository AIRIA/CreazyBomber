//
//  GameManager.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-30.
//
//

#include "GameManager.h"

GameManager *GameManager::getInstance()
{
    static GameManager *_instance = nullptr;
    if(_instance==nullptr)
    {
        _instance = new GameManager();
        _instance->setGameScore(345);
    }
    return _instance;
}

void GameManager::initPlayerItemAnimations(std::string name,std::string aniName,float delta)
{
    auto itemFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
    auto texture = itemFrame->getTexture();
    auto rect = itemFrame->getRect();
    
    auto frameWidth = rect.size.width/4;
    auto frameHeight = rect.size.height/2;
    
    for(auto i=0;i<2;i++)
    {
        Vector<SpriteFrame*> frameVec;
        for(auto j=0;j<4;j++)
        {
            auto frameRect = Rect(rect.origin.x+frameWidth*j,rect.origin.y+frameHeight*i,frameWidth,frameHeight);
            auto frame = SpriteFrame::createWithTexture(texture, frameRect);
            frameVec.pushBack(frame);
        }
        char buffer[50];
        sprintf(buffer, "%s_%d",aniName.c_str(),i);
        auto animation = Animation::createWithSpriteFrames(frameVec);
        if(i==0)
            animation->setDelayPerUnit(0.1f);
        else
            animation->setDelayPerUnit(delta);
        
        AnimationCache::getInstance()->addAnimation(animation, buffer);
    }
}