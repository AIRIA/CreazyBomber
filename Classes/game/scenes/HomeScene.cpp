//
//  HomeScene.cpp
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-18.
//
//

#include "HomeScene.h"
#include "components/PerfectMenu.h"
#include "game/objects/PlayerRole.h"

#define ICON_ANIMATE_TIME 0.3f

enum{
    kStatusSelectRole,
    kStatusSelectMode,
    kStatusSelectScene,
    kStatusSelectStage,
    kStatusStart
} navStatus;

enum{
    kBottomNode = 100,
    kStartMenu,
    kBattleModeNode,
    kStoreModeNode,
    kSelectRoleNode,
    kSelectRoleMenu,
    kBackNode1,
    kBackNode2,
    kBackNode3,
    kBackNode4,
    kBattleMenu,
    kStoryMenu,
    kBackMenu,
    kStageFront,
    kStageMid,
    kStageBack,
    kChangePlayerMenu,
    kBackButtonMenu
};

enum GameMode{
    kGameModeBattle,
    kGameModeStory
} selectMode;

enum Role{
    kRoleVampire,
    kRoleZumbie,
    kRoleSmurf,
    kRoleViking
} selectRole;

enum SceneType{
    kSceneCL_Story,
    kSceneMD_Story,
    kSceneBC_Story,
    kSceneCL_Battle,
    kSceneMD_Battle,
    kSceneBC_Battle
} selectScene;

bool HomeScene::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    /* 预加载音频文件 */
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bg/music_game_bg.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("music/soundEffect/ui_click.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("music/soundEffect/ui_item_in.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("music/soundEffect/ui_item_out.mp3");
    SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
    
    m_fScaleFactor = m_winSize.width/DESIGN_WIDTH;
    Texture2D::PVRImagesHavePremultipliedAlpha(true);
    /* 预加载的PVR纹理 */
    textureFiles.push_back("textures/monster");
    textureFiles.push_back("textures/bg");
    textureFiles.push_back("textures/openning");
    textureFiles.push_back("textures/locale_1");
    textureFiles.push_back("textures/locale_2");
    textureFiles.push_back("textures/locale_3");
    textureFiles.push_back("textures/selectstage");
    textureFiles.push_back("textures/button");
    return true;
}

void HomeScene::onTexturesLoaded()
{
    log("texture loaded");
    auto bg = Sprite::createWithSpriteFrameName("bg.png");
    bg->setPosition(DESIGN_CENTER);
    
    auto bg0 = Sprite::createWithSpriteFrameName("bg0.png");
    bg0->setAnchorPoint(Point::ZERO);
    auto bg1 = Sprite::createWithSpriteFrameName("bg1.png");
    bg1->setAnchorPoint(Point::ZERO);
    auto bg2 = Sprite::createWithSpriteFrameName("bg2.png");
    bg2->setAnchorPoint(Point(1.0f,0.0f));
    bg2->setPosition(DESIGN_RIGHT_BOTTOM);
    
    auto cloud = Sprite::createWithSpriteFrameName("cloud.png");
    cloud->setAnchorPoint(Point(0.5,1.0));
    cloud->setPosition(DESIGN_TOP);

    BlendFunc blend = {GL_DST_COLOR, GL_ONE};
    cloud->setBlendFunc(blend);
    
    auto sun = Sprite::create("textures/sun-hd.png");
    sun->setPosition(Point(300,DESIGN_HEIGHT-60));
    sun->setScale(0.7);
    ActionInterval *rotate = RotateBy::create(0.5f, 5);
    sun->runAction(RepeatForever::create(rotate));
    auto bottomNode = Node::create();
    
    m_pBody->addChild(bg);
    m_pBody->addChild(sun);
    /* kBackNode1 2 3 用来存放关卡选择的UI */
    bottomNode->addChild(Node::create(),0,kBackNode1);
    bottomNode->addChild(bg0);
    /* 存放挑战模式选择的menu */
    bottomNode->addChild(Node::create(),0,kBattleModeNode);
    bottomNode->addChild(Node::create(),0,kBackNode2);
    bottomNode->addChild(bg1);
    /* 存放故事模式选择的menu */
    bottomNode->addChild(Node::create(),0,kStoreModeNode);
    bottomNode->addChild(Node::create(),0,kBackNode3);
    bottomNode->addChild(bg2);
    bottomNode->addChild(Node::create(),0,kBackNode4);
    m_pBody->addChild(bottomNode);
    bottomNode->setPosition(Point(0,-50));
    bottomNode->setTag(kBottomNode);
    m_pBody->addChild(cloud);
    this->__showIcons();
    __addBackMenu();
    //循环播放背景音乐
    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bg/music_main_bg.mp3",true);

}

void HomeScene::__delayRun(float dt,const std::function<void ()> &func)
{
    auto delayTime = DelayTime::create(dt);
    auto delayCall = CallFunc::create(func);
    runAction(Sequence::create(delayTime,delayCall, NULL));
}

void HomeScene::__addBackMenu()
{
    auto mainMenuItemBack = MenuItemSprite::create(SPRITE("back_normal.png"), SPRITE("back_press.png"),SPRITE("back_press.png"));
    mainMenuItemBack->setPosition(Point(80,80));
    auto mainMenu = PerfectMenu::create(mainMenuItemBack,nullptr);
    mainMenu->setPosition(Point::ZERO);
    mainMenu->setTag(kBackButtonMenu);
    m_pBody->addChild(mainMenu);
    mainMenuItemBack->setCallback([&](Ref *pSender)->void{
        this->__setBackButtonEnable(false);
        switch (navStatus) {
            case kStatusSelectRole:
                log("hide roles");
                this->__hideRoles([&]()->void{
                    this->__showIcons();
                });
                break;
            case kStatusSelectMode:
                log("hide modes");
                this->__hideModes([&]()->void{
                    this->__showRoleSelectMenu();
                });
                break;
            case kStatusSelectScene:
                log("hide scenes");
                this->__hideScenes([&]()->void{
                    this->__showGameModeSelectMenu();
                });
                
                break;
            case kStatusSelectStage:
                log("hide stages");
                this->__hideStages([&]()->void{
                    this->__showSceneSelectMenu();
                });
                break;
            default:
                break;
        }
    });
}

void HomeScene::__setBackButtonEnable(bool val,float delay)
{
    runAction(Sequence::create(DelayTime::create(delay),CallFunc::create([&,val]()->void{
        if(val)
        {
            log("enable");
        }else{
            log("disable");
        }
        auto backMenu = (PerfectMenu*)m_pBody->getChildByTag(kBackButtonMenu);
        auto item = (MenuItemSprite*)(*backMenu->getChildren().begin());
        item->setEnabled(val);
    }), NULL));
    
}

/* 显示首页的小图标以及开始游戏的按钮 */
void HomeScene::__showIcons()
{
    navStatus = kStatusStart;
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto back2 = bottomNode->getChildByTag(kBackNode2);
    auto back3 = bottomNode->getChildByTag(kBackNode3);
    auto back4 = bottomNode->getChildByTag(kBackNode4);
    auto createSprite = [](std::string frameName,Point pos,float scale,Node *parentNode)->Sprite*{
        auto sprite = SPRITE(frameName);
        sprite->setScale(scale);
        sprite->setPosition(pos);
        parentNode->addChild(sprite);
        return sprite;
    };
    
    
    createSprite("lajitong.png",Point(350,180),1.0f,back3);
    createSprite("munaiyi.png",Point(350,400),0.6f,back2);
    createSprite("pinglianren.png",Point(880,290),0.6f,back2);
    createSprite("pig.png",Point(700,450),0.6f,back3);
    createSprite("bianfu.png",Point(670,550),0.6f,back2);
    createSprite("zhizhu.png",Point(700,300),0.9f,back3);
    createSprite("deng.png",Point(200,460),1.0f,back4);
    createSprite("title.png",Point(500,500),0.8f,back4);
    
    /* start game menu */
    auto playBtn = MenuItemSprite::create(SPRITE("play_btn_normal.png"),SPRITE("play_btn_press.png"));
    playBtn->setScale(0.7f);
    playBtn->setPosition(Point(480,320));
    playBtn->setCallback([&](Ref *pSender)->void{
        SimpleAudioEngine::getInstance()->playEffect("music/soundEffect/ui_click.mp3");
        this->__hideIcons([&]()->void{
            this->__showRoleSelectMenu();
        });
        
    });
    auto start = PerfectMenu::create(playBtn,nullptr);
    start->setPosition(Point::ZERO);
    start->setTag(kStartMenu);
    bottomNode->addChild(start);
}

void HomeScene::__showRoleSelectMenu()
{
    log("show role select");
    navStatus = kStatusSelectRole;
    int i = 0;
    float roleScale = 0.8f;
    
    auto createRoleItem = [&](Role role,std::string normal,std::string press,Point pos)->MenuItemSprite*{

        auto item = MenuItemSprite::create(SPRITE(normal), SPRITE(press));
        /* callback lambda  */
        item->setCallback([&,role](Ref *pSender)->void{
            selectRole = role;
            auto menuItem = (MenuItemSprite*)pSender;
            auto menu = (PerfectMenu*)menuItem->getParent();
            auto children = menu->getChildren();
            auto it = children.begin();
            while (it!=children.end()) {
                if(*it!=menuItem)
                {
                    auto move = MoveTo::create(0.2f,menuItem->getPosition());
                    auto fadeOut = FadeOut::create(0.2f);
                    auto removeSeq = Sequence::create(Spawn::create(move,fadeOut, NULL),
                                                      CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, *it)), NULL);
                    (*it)->runAction(removeSeq);
                }
                it++;
            }
            this->__delayRun(0.2f, [&,menuItem]()->void{
                menuItem->selected();
                auto flip = OrbitCamera::create(0.8f, 0.5, 0, 0, 360,0, 0);
                menuItem->runAction(flip);
                this->__delayRun(0.9f, [&]()->void{
                    __hideRoles([&]()->void{
                        __showGameModeSelectMenu();
                    });
                });
            });
        });
        item->setPosition(pos);
        item->setScale(roleScale);
        
        
        /* 动画方式进入场景 */
        auto delay = DelayTime::create(i++*0.2f);
        auto moveAct = MoveTo::create(0.4f, pos);
        auto easeInout = EaseBackInOut::create(moveAct);
        /* pos(x,1000)---->pos(x,y) */
        auto beforeRun = CallFuncN::create([](Ref *pSender)->void{
            auto item = (MenuItemSprite*)pSender;
            item->setPosition(Point(item->getPosition().x,1000));
        });
        auto moveSeq = Sequence::create(beforeRun,delay,easeInout, nullptr);
        item->runAction(moveSeq);
        return item;
    };
    
    
    auto vampire = createRoleItem(kRoleVampire,"selectrole_vampire_normal.png","selectrole_vampire_press.png",Point(160,320));
    auto roleSize = vampire->getContentSize();
    roleSize = Size(roleSize.width*roleScale,roleSize.height*roleScale);
    auto zombie = createRoleItem(kRoleZumbie,"selectrole_zombie_normal.png","selectrole_zombie_press.png",vampire->getPosition()+Point(roleSize.width+20,-70));
    auto smurf = createRoleItem(kRoleSmurf,"selectrole_smurf_normal.png","selectrole_smurf_press.png",zombie->getPosition()+Point(roleSize.width+20,130));
    auto viking = createRoleItem(kRoleViking,"selectrole_viking_normal.png","selectrole_viking_press.png",smurf->getPosition()+Point(roleSize.width+20,-80));
    auto selectRole = PerfectMenu::create(vampire,zombie,smurf,viking,nullptr);
    selectRole->setPosition(Point::ZERO);
    m_pBody->addChild(selectRole);
    selectRole->setTag(kSelectRoleMenu);
    selectRole->setEnabled(false);
    this->__delayRun(1.2f, [selectRole]()->void{
        selectRole->setEnabled(true);
    });
    this->__setBackButtonEnable(true,1.2f);
}

void HomeScene::__showGameModeSelectMenu()
{
    navStatus = kStatusSelectMode;
    
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto storyNode = bottomNode->getChildByTag(kStoreModeNode);
    auto battleNode = bottomNode->getChildByTag(kBattleModeNode);
    auto createMenuItem = [](GameMode mode,std::string normal,std::string press,std::string disable,Point archorPoint,Point pos)->MenuItemSprite*{
        auto easeBackInOut = EaseBackInOut::create(ScaleTo::create(0.5f, 0.85f));
        auto item = MenuItemSprite::create(SPRITE(normal), SPRITE(press),SPRITE(disable));
        item->setAnchorPoint(archorPoint);
        item->setPosition(pos);
        item->setScale(0);
        item->setTag(0);
        item->runAction(Sequence::create(easeBackInOut,CallFunc::create([item]()->void{
            auto menu = (Menu*)item->getParent();
            menu->setEnabled(true);
        }), NULL));
        return item;
    };
    
    auto addMenuToNode = [](MenuItemSprite *item,Node *node)->void{
        auto menu = PerfectMenu::create(item,nullptr);
        menu->setPosition(Point::ZERO);
        menu->setEnabled(false);
        menu->setTag(0);
        node->addChild(menu);
    };
    auto storyMode = createMenuItem(kGameModeStory,"story_normal.png","story_press.png","story_press.png",Point(1.0f,0.0f),Point(950,185));
    auto battleMode = createMenuItem(kGameModeBattle,"battle_normal.png","battle_press.png","battle_disable.png",Point::ZERO,Point(30,330));
    battleMode->setEnabled(false);
    addMenuToNode(storyMode,storyNode);
    addMenuToNode(battleMode,battleNode);
    
    storyMode->setCallback([&](Ref *pSender)->void{
        __hideModes([&]()->void{
            __showSceneSelectMenu();
        });
    });
    /* change player menu */
    auto changeNormal = SPRITE("change_role_normal.png");
    auto changePress = SPRITE("change_role_press.png");
    auto changePlayerItem = MenuItemSprite::create(changeNormal, changePress);
    auto changeMenu = PerfectMenu::create(changePlayerItem,nullptr);
    changePlayerItem->setPosition(Point(DESIGN_WIDTH-200,80));
    changeMenu->setPosition(Point::ZERO);
    changeMenu->setTag(kChangePlayerMenu);
    m_pBody->addChild(changeMenu);
    this->__setBackButtonEnable(true,0.5f);
}

void HomeScene::__showSceneSelectMenu()
{
    navStatus = kStatusSelectScene;
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto back2 = bottomNode->getChildByTag(kBackNode2);
    auto back3 = bottomNode->getChildByTag(kBackNode3);
    
    
    auto createItemFunc = [&](SceneType scene, std::string normal,std::string press,std::string disable,Point pos)-> MenuItemSprite*{
        
        auto easeBackInOut = EaseBackInOut::create(ScaleTo::create(0.5f, 0.8f));
        auto item = MenuItemSprite::create(SPRITE(normal), SPRITE(press),SPRITE(disable));
        item->setCallback([&,scene](Ref *pSender)->void{
            selectScene = scene;
            __hideScenes([&]()->void{
                __showStageSelectMenu();
            });
            
            
        });
        item->setEnabled(false);
        item->setPosition(pos);
        item->setAnchorPoint(Point(0.5f,0.0f));
        item->setScale(0);
        item->runAction(easeBackInOut);
        
        return item;
    };
    
    auto cl_story = createItemFunc(kSceneCL_Story,"clstorynormal.png", "clstorypress.png", "clstorydisable.png",Point(300,10));
    auto md_story = createItemFunc(kSceneMD_Story,"mdstorynormal.png", "mdstorypress.png", "mdstorydisable.png",Point(550,100));
    auto bc_story = createItemFunc(kSceneBC_Story,"bcstorynormal.png", "bcstorypress.png", "bcstorydisable.png",Point(800,100));
    
    auto cl_battle = createItemFunc(kSceneCL_Battle,"clbattlenormal.png", "clbattlepress.png", "clbattledisable.png",Point(200,240));
    auto md_battle = createItemFunc(kSceneMD_Battle,"mdbattlenormal.png", "mdbattlepress.png", "mdbattledisable.png",Point(450,250));
    auto bc_battle = createItemFunc(kSceneBC_Battle,"bcbattlenormal.png", "bcbattlepress.png", "bcbattledisable.png",Point(700,230));
    
    cl_battle->setPosition(Point(200,240));
    md_battle->setPosition(Point(450,250));
    bc_battle->setPosition(Point(700,230));
    
    auto addMenu = [this](MenuItemSprite *cl,MenuItemSprite *md,MenuItemSprite *bc,Node *node,int tag)->void{
        auto menu = PerfectMenu::create(cl,md,bc,nullptr);
        menu->setPosition(Point::ZERO);
        menu->setTag(tag);
        menu->setEnabled(false);
        node->addChild(menu);
        this->__delayRun(0.5f, [menu]()->void{
            menu->setEnabled(true);
        });
    };
    
    addMenu(cl_story,md_story,bc_story,back3,kStoryMenu);
    addMenu(cl_battle,md_battle,bc_battle,back2,kBattleMenu);
    cl_story->setEnabled(true);
    this->__setBackButtonEnable(true,0.5f);
}

void HomeScene::__showStageSelectMenu()
{
    navStatus = kStatusSelectStage;
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    /* 获取三个节点 */
    auto back1 = bottomNode->getChildByTag(kBackNode1);
    auto back2 = bottomNode->getChildByTag(kBackNode2);
    auto back3 = bottomNode->getChildByTag(kBackNode3);
    
    std::string type = "bc";
    switch (selectScene) {
        case kSceneCL_Story:
            type = "cl";
            break;
        case kSceneBC_Story:
            type = "bc";
            break;
        case kSceneMD_Story:
            type = "md";
            break;
        default:
            break;
    }
    
    
    
    std::vector<Point> points = {Point(350,70),Point(510,140),Point(670,180),Point(830,250),Point(640,300),Point(440,330),Point(270,340),Point(100,330),Point(400,450),Point(560,460),Point(720,450),Point(870,430)};
    
    auto createMenu = [](Node *parentNode,int tag)->PerfectMenu*{
        auto menu = PerfectMenu::create();
        parentNode->addChild(menu);
        menu->setPosition(Point::ZERO);
        menu->setTag(tag);
        return menu;
    };
    
    int idx = 0;
    auto it = points.begin();
    
    auto row1 = createMenu(back1,kStageBack);
    auto row2 = createMenu(back2,kStageMid);
    auto row3 = createMenu(back3,kStageFront);
    PerfectMenu *menu = nullptr;
    MenuItemSprite *stage = nullptr;
    auto showAct = ScaleTo::create(0.2, 0.8f);
    auto easeBackIn = EaseBackInOut::create(showAct);
    while (it!=points.end()) {
        if(idx<3)
        {
            menu = row3;
            stage = MenuItemSprite::create(SPRITE(type+"_stage1_normal.png"), SPRITE(type+"_stage1_press.png"),SPRITE(type+"_stage1_disable.png"));
        }
        else if(idx<8)
        {
            menu = row2;
            stage = MenuItemSprite::create(SPRITE(type+"_stage2_normal.png"), SPRITE(type+"_stage2_press.png"),SPRITE(type+"_stage2_disable.png"));
        }
        else
        {
            menu = row1;
            stage = MenuItemSprite::create(SPRITE(type+"_stage1_normal.png"), SPRITE(type+"_stage1_press.png"),SPRITE(type+"_stage1_disable.png"));
        }
        stage->setUserData(&idx);
        stage->setPosition(*it);
        stage->setAnchorPoint(Point(0.5,0));
        stage->setScale(0.0f);
        if(idx!=0){
            stage->setEnabled(false);
        }
        auto showSeq = Sequence::create(DelayTime::create(idx*0.15f),easeBackIn->clone(), NULL);
        stage->runAction(showSeq);
        stage->setCallback([](Ref *pSender)->void{
            auto item = (MenuItemSprite*)pSender;
            void *userData = item->getUserData();
            int idx = *((int*)(userData));
            log("idx %d",idx);
            
            
        });
        menu->addChild(stage);
        it++;
        idx++;
    }
    this->__setBackButtonEnable(true,1.0f);
    
}

/* 隐藏首页的小图标 */
void HomeScene::__hideIcons(const std::function<void()> &func)
{
    this->__setBackButtonEnable(false);
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto removeIcon = [&](int nodeTag)->void{
        auto node = bottomNode->getChildByTag(nodeTag);
        auto children = node->getChildren();
        auto it = children.begin();
        while(it!=children.end())
        {
            auto removeCallback = CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, *it));
            auto scaleAct = ScaleTo::create(ICON_ANIMATE_TIME, 0);
            auto hideSeq = Sequence::create(scaleAct,removeCallback, NULL);
            (*it)->runAction(hideSeq);
            it++;
        }
    };
    removeIcon(kBackNode2);
    removeIcon(kBackNode3);
    removeIcon(kBackNode4);
    auto startMenu = (PerfectMenu*)bottomNode->getChildByTag(kStartMenu);
    __hideElements(startMenu, ICON_ANIMATE_TIME);
    this->__delayRun(0, func);
}


/* 选择角色后 隐藏角色选择的界面 */
void HomeScene::__hideRoles(const std::function<void()> &func)
{
    this->__setBackButtonEnable(false);
    SimpleAudioEngine::getInstance()->playEffect("music/soundEffect/ui_item_out.mp3");
    auto roleMenu = (PerfectMenu*)m_pBody->getChildByTag(kSelectRoleMenu);
    roleMenu->setEnabled(false);
    auto children = roleMenu->getChildren();
    int i=0;
    float delayTime = 0.0f;
    float aniTime = 0.3f;
    for(auto &child : children)
    {
        auto item = (MenuItemSprite*)child;
        i++;
        delayTime = i*0.2f;
        auto delay = DelayTime::create(delayTime);
        auto itemPos = item->getPosition();
        auto move = MoveTo::create(aniTime, Point(itemPos.x,1000));
        item->runAction(Sequence::create(delay,move,NULL));
    }
    auto delay = DelayTime::create(delayTime+aniTime);
    auto delayCall = CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, roleMenu));
    roleMenu->runAction(Sequence::create(delay,delayCall,nullptr));
    this->__delayRun(i*0.2f, func);
    
}
/* 选择玩游戏模式后 隐藏模式选择的UI */
void HomeScene::__hideModes(const std::function<void()> &func)
{
    log("hide modes");
    this->__setBackButtonEnable(false);
    SimpleAudioEngine::getInstance()->playEffect("music/soundEffect/ui_item_out.mp3");
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto storyNode = bottomNode->getChildByTag(kStoreModeNode);
    auto battleNode = bottomNode->getChildByTag(kBattleModeNode);
    auto battleMenu = (PerfectMenu*)battleNode->getChildByTag(0);
    auto storyMenu = (PerfectMenu*)storyNode->getChildByTag(0);

    m_pBody->getChildByTag(kChangePlayerMenu)->removeFromParent();
    __hideElements(battleMenu, 0.5f);
    __hideElements(storyMenu, 0.5f);
    this->__delayRun(0.5f, func);
}

void HomeScene::__hideScenes(const std::function<void()> &func)
{
    this->__setBackButtonEnable(false);
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto back2 = bottomNode->getChildByTag(kBackNode2);
    auto back3 = bottomNode->getChildByTag(kBackNode3);
    auto storyMenu = back3->getChildByTag(kStoryMenu);
    auto battleMenu = back2->getChildByTag(kBattleMenu);
    __hideElements(storyMenu, 0.3f);
    __hideElements(battleMenu, 0.3f);
    this->__delayRun(0.5f, func);
}

void HomeScene::__hideStages(const std::function<void()> &func)
{
    this->__setBackButtonEnable(false);
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    
    auto hideMenuItem = [&](int nodeTag,int menuTag)->void{
        auto wrapperNode = bottomNode->getChildByTag(nodeTag);
        auto menu = (PerfectMenu*)wrapperNode->getChildByTag(menuTag);
        __hideElements(menu,0.3f);
    };
    
    hideMenuItem(kBackNode1,kStageBack);
    hideMenuItem(kBackNode2,kStageMid);
    hideMenuItem(kBackNode3,kStageFront);
    this->__delayRun(0.3f, func);
}

void HomeScene::__hideElements(cocos2d::Node *node,float duration)
{
    auto menu = (PerfectMenu*)node;
    if(menu!=nullptr)
    {
        menu->setEnabled(false);
    }
    auto children = node->getChildren();
    auto it = children.begin();
    while (it!=children.end()) {
        auto easeBackInOut = EaseBackInOut::create(ScaleTo::create(duration, 0.0f));
        (*it)->runAction(easeBackInOut);
        it++;
    }
    auto delay = DelayTime::create(duration);
    auto delayCall = CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, node));
    node->runAction(Sequence::create(delay,delayCall,nullptr));
}

