//
//  Util.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-23.
//
//

#include "Util.h"
#include "game/GameConfig.h"

std::string Util::itoa(int i)
{
    char buff[50];
    sprintf(buff, "%d",i);
    return buff;
}

void Util::playSound(std::string sound,bool repeat)
{
    if(GameConfig::getInstance()->getSoundEnable())
    {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
        SimpleAudioEngine::getInstance()->playBackgroundMusic(sound.c_str(),repeat);
    }
}

void Util::playEffect(std::string effect,bool repeat)
{
    if(GameConfig::getInstance()->getEffectEnable())
    {
        SimpleAudioEngine::getInstance()->playEffect(effect.c_str(),repeat);
    }
}

void Util::addAnimation(std::string fileName, int frameNum)
{
    auto animation = AnimationCache::getInstance()->getAnimation(fileName);
    if(animation)
    {
        return;
    }
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
    auto textureRect = frame->getRect();
    auto frameWidth = textureRect.size.width/frameNum;
    auto frameHeight = textureRect.size.height;
    Vector<SpriteFrame*> frameVec;
    for(auto i=0;i<frameNum;i++)
    {
        auto rect = Rect(textureRect.origin.x+frameWidth*i,textureRect.origin.y,frameWidth,frameHeight);
        auto spriteFrame = SpriteFrame::createWithTexture(frame->getTexture(), rect);
        frameVec.pushBack(spriteFrame);
    }
    animation = Animation::createWithSpriteFrames(frameVec);
    animation->setDelayPerUnit(0.15f);
    AnimationCache::getInstance()->addAnimation(animation,fileName);
}