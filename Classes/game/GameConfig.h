//
//  GameConfig.h
//  CreazyBomber
//
//  Created by AIRIA on 14-5-21.
//
//

#ifndef __CreazyBomber__GameConfig__
#define __CreazyBomber__GameConfig__

#include "common/CommonHeaders.h"



class GameConfig
{
private:
    
public:
    enum SceneType{
        kClStory,
        kClBattle,
        kMdStory,
        kMdBattle,
        kBcStory,
        kBcBattle
    };
    static std::string selectedRoleName;
    static std::string selectedSceneName;
    static std::string selectedStageName;
    static int selectedLevel;
    static void setSelectScene(SceneType type);
    static std::string getSelectScene();
};

#endif /* defined(__CreazyBomber__GameConfig__) */
