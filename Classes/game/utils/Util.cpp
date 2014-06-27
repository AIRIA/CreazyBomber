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
//        SimpleAudioEngine::getInstance()->playBackgroundMusic(sound.c_str(),repeat);
    }
}

void Util::playEffect(std::string effect,bool repeat)
{
    if(GameConfig::getInstance()->getEffectEnable())
    {
        SimpleAudioEngine::getInstance()->playEffect(effect.c_str(),repeat);
    }
}