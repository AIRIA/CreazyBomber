//
//  WelcomeScene.h
//  CreazyBomber
//
//  Created by AIRIA on 14-7-11.
//
//

#ifndef __CreazyBomber__WelcomeScene__
#define __CreazyBomber__WelcomeScene__

#include "base/BaseLayer.h"

class GameConfig;

class WelcomeScene:public BaseLayer
{
public:
    
    enum Role{
        kRoleVampire,
        kRoleZombie,
        kRoleSmurf,
        kRoleViking
    } selectRole;
    
    enum GameMode{
        kGameModeBattle,
        kGameModeStory
    } selectMode;
    
    enum SceneType{
        kSceneCL_Story,
        kSceneMD_Story,
        kSceneBC_Story,
        kSceneCL_Battle,
        kSceneMD_Battle,
        kSceneBC_Battle
    } selectScene;
    
    void onTexturesLoaded();
    virtual bool init();
    CREATE_FUNC(WelcomeScene);
    
protected:
    Node *node1,*node2,*node3,*node4;
    MenuItemSprite *back,*store,*changeRole,*rank;
    GameConfig *config;

    /**
     * 初始化所有的菜单
     */
    void _initMenu();
    

    /**
     * 设置屏蔽层的开启状态
     */
    void _setShieldEnabled(bool val);

    /**
     * 延迟指定的时间 然后调用指定的方法
     */
    void _delayCall(float delta,const std::function<void ()> &func);
    
    void _initBackground();
    /**
     * 显示欢迎的界面 只有在打开程序的时候才会调用该方法 仅会被调用一次
     */
    void _showWelcome();
    /**
     * 显示角色选择列表
     */
    void _showRoles();
    
    /**
     * 显示游戏模式菜单
     */
    void _showGameMode();
    
    void _hideGameMode();
    
    /**
     * 显示关卡选择界面
     */
    void _showLevelSelect();
    
    /**
     * 选择舞台模式 冰川 墓地 丛林
     */
    void _showStages();
    
    /**
     * 上下浮动的Action
     */
    ActionInterval *getFloatAction(float actTime);
    
    /**
     * 使用指定的Action 隐藏元素
     * 隐藏完毕之后调用指定的回调
     */
    void hideElement(ActionInterval *action,CallFunc *callback = nullptr);
    
};


#endif /* defined(__CreazyBomber__WelcomeScene__) */
