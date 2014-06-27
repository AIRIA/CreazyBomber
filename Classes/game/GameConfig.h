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
public:
    static GameConfig *getInstance();
    
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
    
    bool getSoundEnable(){
        return m_bSoundEnable;
    };
    
    void setSoundEnable(bool val);
    
    bool getEffectEnable(){
        return m_bEffectEnable;
    };
    
    void setEffectEnable(bool val);
protected:
    bool m_bSoundEnable = true;
    bool m_bEffectEnable = true;
    
};

#endif /* defined(__CreazyBomber__GameConfig__) */
