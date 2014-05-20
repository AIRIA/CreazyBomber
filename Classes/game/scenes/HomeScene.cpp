//
//  HomeScene.cpp
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-18.
//
//

#include "HomeScene.h"
enum{
    kStatusSelectRole,
    kStatusSelectMode,
    kStatusSelectScene,
    kStatusSelectStage
} navStatus;

enum{
    kBottomNode = 100,
    kBattleModeNode,
    kStoreModeNode,
    kSelectRoleNode,
    kSelectRoleMenu,
    kBackNode1,
    kBackNode2,
    kBackNode3,
    kBattleMenu,
    kStoryMenu,
    kBackMenu
};

bool HomeScene::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bg/music_game_bg.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("music/soundEffect/ui_click.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("music/soundEffect/ui_item_in.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("music/soundEffect/ui_item_out.mp3");
    SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
    
    m_fScaleFactor = m_winSize.width/DESIGN_WIDTH;
    Texture2D::PVRImagesHavePremultipliedAlpha(true);
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
    
    auto deng = Sprite::createWithSpriteFrameName("deng.png");
    deng->setPosition(Point(200,460));
    auto lajitong = Sprite::createWithSpriteFrameName("lajitong.png");
    lajitong->setPosition(Point(350,180));
    auto munaiyi = Sprite::createWithSpriteFrameName("munaiyi.png");
    munaiyi->setPosition(Point(350,400));
    munaiyi->setScale(0.6);
    auto pinglianren = Sprite::createWithSpriteFrameName("pinglianren.png");
    pinglianren->setPosition(Point(880,290));
    pinglianren->setScale(0.6f);
    auto pig = Sprite::createWithSpriteFrameName("pig.png");
    pig->setScale(0.6f);
    pig->setPosition(Point(700,450));
    
    auto bianfu = Sprite::createWithSpriteFrameName("bianfu.png");
    bianfu->setScale(0.6f);
    bianfu->setPosition(Point(670,550));
    auto zhizhu = Sprite::createWithSpriteFrameName("zhizhu.png");
    zhizhu->setPosition(Point(700,300));
    zhizhu->setScale(0.8f);
    
    auto logo = Sprite::createWithSpriteFrameName("title.png");
    logo->setPosition(Point(500,500));
    logo->setScale(0.8f);
    
    
    auto sun = Sprite::create("textures/sun-hd.png");
    sun->setPosition(Point(300,DESIGN_HEIGHT-60));
    sun->setScale(0.7);
    ActionInterval *rotate = RotateBy::create(0.5f, 5);
    sun->runAction(RepeatForever::create(rotate));
    auto bottomNode = Node::create();
    
    /* start game menu */
    auto playBtn = MenuItemSprite::create(SPRITE("play_btn_normal.png"),SPRITE("play_btn_press.png"),CC_CALLBACK_1(HomeScene::__startMenuHandler, this));
    playBtn->setScale(0.7f);
    playBtn->setPosition(Point(480,320));
    auto start = Menu::create(playBtn,nullptr);
    start->setPosition(Point::ZERO);
    
    
    m_pBody->addChild(bg);
    m_pBody->addChild(sun);
    /* kBackNode1 2 3 用来存放关卡选择的UI */
    bottomNode->addChild(Node::create(),0,kBackNode1);
    bottomNode->addChild(bg0);
    /* 存放挑战模式选择的menu */
    bottomNode->addChild(Node::create(),0,kBattleModeNode);
    bottomNode->addChild(Node::create(),0,kBackNode2);
    bottomNode->addChild(munaiyi,0,tag1++);
    bottomNode->addChild(pinglianren,0,tag1++);
    bottomNode->addChild(bianfu,0,tag1++);
    bottomNode->addChild(pig,0,tag1++);
    bottomNode->addChild(bg1);
    /* 存放故事模式选择的menu */
    bottomNode->addChild(Node::create(),0,kStoreModeNode);
    bottomNode->addChild(lajitong,0,tag1++);
    bottomNode->addChild(zhizhu,0,tag1++);
    bottomNode->addChild(Node::create(),0,kBackNode3);
    bottomNode->addChild(bg2);
    bottomNode->addChild(deng,0,tag1++);
    m_pBody->addChild(bottomNode);
    bottomNode->addChild(logo,0,tag1++);
    bottomNode->addChild(start,0,tag1++);
    bottomNode->setPosition(Point(0,-50));
    bottomNode->setTag(kBottomNode);
    m_pBody->addChild(cloud);
    
    __addBackMenu();
    //循环播放背景音乐
    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bg/music_main_bg.mp3",true);

}

/* 开始按钮的回调 点击后 隐藏部分UI */
void HomeScene::__startMenuHandler(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect("music/soundEffect/ui_click.mp3");
    int tag = tag1;
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto scaleAct = ScaleTo::create(0.2f, 0);
    while (tag-->0) {
        auto child = bottomNode->getChildByTag(tag);
        child->runAction(scaleAct->clone());
    }

    auto item = (MenuItemSprite*)pSender;
    auto menu = (Menu*)item->getParent();
    menu->setEnabled(false);
    navStatus = kStatusSelectRole;
    __showRoleSelectMenu();
    
}

void HomeScene::__addBackMenu()
{
    auto mainMenuItemBack = MenuItemSprite::create(SPRITE("back_normal.png"), SPRITE("back_press.png"));
    mainMenuItemBack->setPosition(Point(80,80));
    auto mainMenu = Menu::create(mainMenuItemBack,nullptr);
    mainMenu->setPosition(Point::ZERO);
    m_pBody->addChild(mainMenu);
    mainMenuItemBack->setCallback([&](Ref *pSender)->void{
        switch (navStatus) {
            case kStatusSelectRole:
                __hideRoles();
                break;
            case kStatusSelectMode:
                __hideModes();
                break;
            case kStatusSelectScene:
                __hideScenes();
                break;
            case kStatusSelectStage:
                break;
            
            default:
                break;
        }
    });
}

void HomeScene::__showRoleSelectMenu()
{
    int i = 0;
    float roleScale = 0.8f;
    auto createRoleItem = [&](std::string normal,std::string press,Point pos)->MenuItemSprite*{
        /*  */
        auto item = MenuItemSprite::create(SPRITE(normal), SPRITE(press));
        item->setCallback([&](Ref *pSender)->void{
            __hideRoles();
            auto delay = DelayTime::create(0.8f);
            auto delayShow = CallFunc::create([&]()->void{
                __showGameModeSelectMenu();
            });
            auto delaySeq = Sequence::create(delay,delayShow, NULL);
            this->runAction(delaySeq);
            
        });
        item->setPosition(pos);
        item->setScale(roleScale);
        
        
        /* 动画方式进入场景 */
        auto delay = DelayTime::create(i++*0.2f);
        auto moveAct = MoveTo::create(0.4f, pos);
        auto easeInout = EaseBackInOut::create(moveAct);
        auto beforeRun = CallFuncN::create([](Ref *pSender)->void{
            auto item = (MenuItemSprite*)pSender;
            item->setPosition(Point(item->getPosition().x,1000));
        });
        auto moveSeq = Sequence::create(beforeRun,delay,easeInout, nullptr);
        item->runAction(moveSeq);
        return item;
    };
    
    
    auto vampire = createRoleItem("selectrole_vampire_normal.png","selectrole_vampire_press.png",Point(160,320));
    auto roleSize = vampire->getContentSize();
    roleSize = Size(roleSize.width*roleScale,roleSize.height*roleScale);
    auto zombie = createRoleItem("selectrole_zombie_normal.png","selectrole_zombie_press.png",vampire->getPosition()+Point(roleSize.width+20,-70));
    auto smurf = createRoleItem("selectrole_smurf_normal.png","selectrole_smurf_press.png",zombie->getPosition()+Point(roleSize.width+20,130));
    auto viking = createRoleItem("selectrole_viking_normal.png","selectrole_viking_press.png",smurf->getPosition()+Point(roleSize.width+20,-80));
    auto selectRole = Menu::create(vampire,zombie,smurf,viking,nullptr);
    selectRole->setPosition(Point::ZERO);
    m_pBody->addChild(selectRole);
    selectRole->setTag(kSelectRoleMenu);
}

void HomeScene::__showGameModeSelectMenu()
{
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto storyNode = bottomNode->getChildByTag(kStoreModeNode);
    auto battleNode = bottomNode->getChildByTag(kBattleModeNode);
    auto createMenuItem = [](std::string normal,std::string press,std::string disable,Point archorPoint,Point pos)->MenuItemSprite*{
        auto easeBackInOut = EaseBackInOut::create(ScaleTo::create(0.5f, 0.85f));
        auto item = MenuItemSprite::create(SPRITE(normal), SPRITE(press),SPRITE(disable));
        item->setAnchorPoint(archorPoint);
        item->setPosition(pos);
        item->setScale(0);
        item->setTag(0);
        item->runAction(easeBackInOut);
        return item;
    };
    
    auto addMenuToNode = [](MenuItemSprite *item,Node *node)->void{
        auto menu = Menu::create(item,nullptr);
        menu->setPosition(Point::ZERO);
        menu->setTag(0);
        node->addChild(menu);
    };
    auto storyMode = createMenuItem("story_normal.png","story_press.png","story_press.png",Point(1.0f,0.0f),Point(950,185));
    auto battleMode = createMenuItem("battle_normal.png","battle_press.png","battle_disable.png",Point::ZERO,Point(30,330));
    addMenuToNode(storyMode,storyNode);
    addMenuToNode(battleMode,battleNode);
    
    storyMode->setCallback([&](Ref *pSender)->void{
        __hideModes();
        __showSceneSelectMenu();
    });
    /* change player menu */
    auto changeNormal = SPRITE("change_role_normal.png");
    auto changePress = SPRITE("change_role_press.png");
    auto changePlayerItem = MenuItemSprite::create(changeNormal, changePress);
    auto changeMenu = Menu::create(changePlayerItem,nullptr);
    changePlayerItem->setPosition(Point(DESIGN_WIDTH-200,80));
    changeMenu->setPosition(Point::ZERO);
    m_pBody->addChild(changeMenu);
    
}

void HomeScene::__showSceneSelectMenu()
{
    navStatus = kStatusSelectScene;
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto back2 = bottomNode->getChildByTag(kBackNode2);
    auto back3 = bottomNode->getChildByTag(kBackNode3);
    
    
    auto createItemFunc = [&](std::string normal,std::string press,std::string disable,Point pos)-> MenuItemSprite*{
        auto easeBackInOut = EaseBackInOut::create(ScaleTo::create(0.3f, 0.8f));
        auto item = MenuItemSprite::create(SPRITE(normal), SPRITE(press),SPRITE(disable));
        item->setCallback([&](Ref *pSender)->void{
            __hideScenes();
            __showStageSelectMenu();
            
        });
        item->setEnabled(false);
        item->setPosition(pos);
        item->setAnchorPoint(Point(0.5f,0.0f));
        item->setScale(0);
        item->runAction(easeBackInOut);
        
        return item;
    };
    
    auto cl_story = createItemFunc("clstorynormal.png", "clstorypress.png", "clstorydisable.png",Point(300,10));
    auto md_story = createItemFunc("mdstorynormal.png", "mdstorypress.png", "mdstorydisable.png",Point(550,100));
    auto bc_story = createItemFunc("bcstorynormal.png", "bcstorypress.png", "bcstorydisable.png",Point(800,100));
    
    auto cl_battle = createItemFunc("clbattlenormal.png", "clbattlepress.png", "clbattledisable.png",Point(200,240));
    auto md_battle = createItemFunc("mdbattlenormal.png", "mdbattlepress.png", "mdbattledisable.png",Point(450,250));
    auto bc_battle = createItemFunc("bcbattlenormal.png", "bcbattlepress.png", "bcbattledisable.png",Point(700,230));
    
    cl_battle->setPosition(Point(200,240));
    md_battle->setPosition(Point(450,250));
    bc_battle->setPosition(Point(700,230));
    
    auto addMenu = [](MenuItemSprite *cl,MenuItemSprite *md,MenuItemSprite *bc,Node *node,int tag)->void{
        auto menu = Menu::create(cl,md,bc,nullptr);
        menu->setPosition(Point::ZERO);
        menu->setTag(tag);
        node->addChild(menu);

    };
    
    addMenu(cl_story,md_story,bc_story,back3,kStoryMenu);
    addMenu(cl_battle,md_battle,bc_battle,back2,kBattleMenu);
    cl_story->setEnabled(true);
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
    
    std::vector<Point> points = {Point(350,70),Point(510,140),Point(670,180),Point(830,250),Point(640,300),Point(440,330),Point(270,340),Point(100,330),Point(400,450),Point(560,460),Point(720,450),Point(870,430)};
    
    auto createMenu = [](Node *parentNode)->Menu*{
        auto menu = Menu::create();
        parentNode->addChild(menu);
        menu->setPosition(Point::ZERO);
        return menu;
    };
    
    int idx = 0;
    auto it = points.begin();
    
    auto row1 = createMenu(back1);
    auto row2 = createMenu(back2);
    auto row3 = createMenu(back3);
    Menu *menu = nullptr;
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
        stage->setPosition(*it);
        stage->setAnchorPoint(Point(0.5,0));
        stage->setScale(0.0f);
        if(idx!=0){
            stage->setEnabled(false);
        }
        auto showSeq = Sequence::create(DelayTime::create(idx*0.15f),easeBackIn->clone(), NULL);
        stage->runAction(showSeq);
        menu->addChild(stage);
        it++;
        idx++;
    }

}


/* 选择角色后 隐藏角色选择的界面 */
void HomeScene::__hideRoles()
{
    SimpleAudioEngine::getInstance()->playEffect("music/soundEffect/ui_item_out.mp3");
    auto roleMenu = (Menu*)m_pBody->getChildByTag(kSelectRoleMenu);
    roleMenu->setEnabled(false);
    auto children = roleMenu->getChildren();
    int i=0;
    for(auto &child : children)
    {
        auto item = (MenuItemSprite*)child;
        if(item->isSelected()==false)
        {
            item->setOpacity(128);
        }
        i++;
        auto delay = DelayTime::create(i*0.2f);
        auto itemPos = item->getPosition();
        auto move = MoveTo::create(0.3f, Point(itemPos.x,1000));
        item->runAction(Sequence::create(delay,move,NULL));
    }
}
/* 选择玩游戏模式后 隐藏模式选择的UI */
void HomeScene::__hideModes()
{
    SimpleAudioEngine::getInstance()->playEffect("music/soundEffect/ui_item_out.mp3");
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto storyNode = bottomNode->getChildByTag(kStoreModeNode);
    auto battleNode = bottomNode->getChildByTag(kBattleModeNode);
    auto battleMenu = (Menu*)battleNode->getChildByTag(0);
    auto storyMenu = (Menu*)storyNode->getChildByTag(0);
    auto battleMenuItem = (MenuItemSprite*)battleMenu->getChildByTag(0);
    auto storyMenuItem = (MenuItemSprite*)storyMenu->getChildByTag(0);
    battleMenu->setEnabled(false);
    storyMenu->setEnabled(false);
    
    battleMenuItem->runAction(EaseBackInOut::create(ScaleTo::create(0.5f, 0.0f)));
    storyMenuItem->runAction(EaseBackInOut::create(ScaleTo::create(0.5f, 0.0f)));
}

void HomeScene::__hideScenes()
{
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto back2 = bottomNode->getChildByTag(kBackNode2);
    auto back3 = bottomNode->getChildByTag(kBackNode3);
    auto storyMenu = back3->getChildByTag(kStoryMenu);
    auto battleMenu = back2->getChildByTag(kBattleMenu);
    __hideElements(storyMenu, 0.3f);
    __hideElements(battleMenu, 0.3f);
}

void HomeScene::__hideStages()
{
    
}

void HomeScene::__hideElements(cocos2d::Node *node,float duration)
{
    auto menu = (Menu*)node;
    if(menu!=nullptr)
    {
        menu->setEnabled(false);
    }
    menu->setEnabled(false);
    auto children = node->getChildren();
    auto it = children.begin();
    while (it!=children.end()) {
        auto easeBackInOut = EaseBackInOut::create(ScaleTo::create(duration, 0.0f));
        (*it)->runAction(easeBackInOut);
        it++;
    }
}




