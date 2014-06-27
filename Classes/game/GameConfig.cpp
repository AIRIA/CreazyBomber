//
//  GameConfig.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-21.
//
//

#include "GameConfig.h"

std::string GameConfig::selectedRoleName = "viking";
std::string GameConfig::selectedSceneName = "cl";
std::string GameConfig::selectedStageName = "";
int GameConfig::selectedLevel = 7;

GameConfig *GameConfig::getInstance()
{
    static GameConfig *_instance = nullptr;
    if (_instance==nullptr) {
        _instance = new GameConfig();
    }
    return _instance;
}

void GameConfig::setSoundEnable(bool val)
{
    if(getSoundEnable()!=val&&val==false)
    {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }else if(SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()==false){
        SimpleAudioEngine::getInstance()->playBackgroundMusic(SOUND_SCENE_BG,true);
    }else{
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    m_bSoundEnable = val;
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
    
}

void GameConfig::setSelectScene(SceneType type)
{
    switch (type) {
        case kClBattle:
            selectedSceneName = "cl_battle";
            break;
        case kClStory:
            selectedSceneName = "cl_story";
            break;
        default:
            break;
    }
}

std::string GameConfig::getSelectScene()
{
    
}