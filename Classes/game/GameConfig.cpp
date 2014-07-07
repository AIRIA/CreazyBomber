//
//  GameConfig.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-21.
//
//

#include "GameConfig.h"


GameConfig *GameConfig::getInstance()
{
    static GameConfig *_instance = nullptr;
    if (_instance==nullptr) {
        _instance = new GameConfig();
        _instance->init();
        _instance->setSelectRoleName("viking");
        _instance->setSelectSceneName("cl");
        _instance->setSelectLevel(1);
    }
    return _instance;
}

void GameConfig::init()
{
    auto isExist = __userDefault->getBoolForKey("isExist", false);
    if(isExist)
    {

    }
    else
    {
        __userDefault->setBoolForKey("isExist", true);
        __userDefault->setBoolForKey(KEY_CL, true);
        __userDefault->setBoolForKey(KEY_MD, false);
        __userDefault->setBoolForKey(KEY_BC, false);
        __userDefault->setBoolForKey(KEY_CL_BATTLE, false);
        __userDefault->setBoolForKey(KEY_MD_BATTLE, false);
        __userDefault->setBoolForKey(KEY_BC_BATTLE, false);
        
        __userDefault->setBoolForKey(KEY_BATTLE_MODE, false);
        __userDefault->setIntegerForKey(KEY_CL_LEVEL, 1);
        __userDefault->setIntegerForKey(KEY_BC_LEVEL, 1);
        __userDefault->setIntegerForKey(KEY_MD_LEVEL, 1);
        __userDefault->setBoolForKey(KEY_SOUND_ENABLED, true);
        __userDefault->setBoolForKey(KEY_EFFECT_ENABLED, true);
        __userDefault->setIntegerForKey(KEY_COIN_NUM, 30000);
    }  
}

bool GameConfig::getSoundEnable()
{
    return __userDefault->getBoolForKey(KEY_SOUND_ENABLED);
}

bool GameConfig::getEffectEnable()
{
    return __userDefault->getBoolForKey(KEY_EFFECT_ENABLED);
}

void GameConfig::setSoundEnable(bool val)
{
    if(getSoundEnable()!=val&&val==false)
    {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
    else
    {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    m_bSoundEnable = val;
    __userDefault->setBoolForKey(KEY_SOUND_ENABLED, val);
}

void GameConfig::setEffectEnable(bool val)
{
    if(m_bEffectEnable!=val)
    {
        if(val==false)
        {
            SimpleAudioEngine::getInstance()->stopAllEffects();
        }
        m_bEffectEnable = val;
    }
    __userDefault->setBoolForKey(KEY_EFFECT_ENABLED, val);
    
}
