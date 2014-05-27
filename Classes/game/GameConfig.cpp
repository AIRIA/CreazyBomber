//
//  GameConfig.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-21.
//
//

#include "GameConfig.h"

std::string GameConfig::selectedRoleName = "";
std::string GameConfig::selectedSceneName = "cl";
std::string GameConfig::selectedStageName = "";
int GameConfig::selectedLevel = 11;

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