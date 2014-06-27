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
//    static std::string selectedRoleName;
//    static std::string selectedSceneName;
//    static std::string selectedStageName;
    
    bool getSoundEnable();
    void setSoundEnable(bool val);
    
    bool getEffectEnable();
    void setEffectEnable(bool val);
    
    /* 初始化整个程序的关卡信息 */
    void init();
    
    
protected:
    bool m_bSoundEnable = true;
    bool m_bEffectEnable = true;
    CC_SYNTHESIZE(std::string, m_sSelectRoleName, SelectRoleName);
    CC_SYNTHESIZE(std::string, m_sSelectSceneName, SelectSceneName);
    CC_SYNTHESIZE(std::string, m_sSelectStageName, SelectStageName);
    CC_SYNTHESIZE(int,m_iSelectLevel,SelectLevel);
};

#endif /* defined(__CreazyBomber__GameConfig__) */
