//
//  HomeScene.cpp
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-18.
//
//

#include "HomeScene.h"
enum{
    kBottomNode = 100,
    kBattleModeNode,
    kStoreModeNode,
    kSelectRoleNode,
    kSelectRoleMenu
};

bool HomeScene::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bg/music_game_bg.mp3");
    m_fScaleFactor = m_winSize.width/DESIGN_WIDTH;
    Texture2D::PVRImagesHavePremultipliedAlpha(true);
    textureFiles.push_back("textures/monster");
    textureFiles.push_back("textures/bg");
    textureFiles.push_back("textures/openning");
    textureFiles.push_back("textures/locale_1");
    textureFiles.push_back("textures/locale_2");
    textureFiles.push_back("textures/locale_3");
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
    
    auto playBtn = MenuItemSprite::create(SPRITE("play_btn_normal.png"),SPRITE("play_btn_press.png"),CC_CALLBACK_1(HomeScene::__startMenuHandler, this));
    playBtn->setScale(0.7f);
    playBtn->setPosition(Point(480,320));
    auto start = Menu::create(playBtn,nullptr);
    start->setPosition(Point::ZERO);
    
    
    m_pBody->addChild(bg);
    m_pBody->addChild(sun);
    bottomNode->addChild(bg0);
    bottomNode->addChild(Node::create(),0,kBattleModeNode);
    bottomNode->addChild(munaiyi,0,tag1++);
    bottomNode->addChild(pinglianren,0,tag1++);
    bottomNode->addChild(bianfu,0,tag1++);
    bottomNode->addChild(pig,0,tag1++);
    bottomNode->addChild(bg1);
    bottomNode->addChild(Node::create(),0,kStoreModeNode);
    bottomNode->addChild(lajitong,0,tag1++);
    bottomNode->addChild(zhizhu,0,tag1++);
    bottomNode->addChild(bg2);
    bottomNode->addChild(deng,0,tag1++);
    m_pBody->addChild(bottomNode);
    bottomNode->addChild(logo,0,tag1++);
    bottomNode->addChild(start,0,tag1++);
    bottomNode->setPosition(Point(0,-50));
    bottomNode->setTag(kBottomNode);
    m_pBody->addChild(cloud);
    
    __addSelectModeUI();
    __addSelectRoleUI();
    
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bg/music_main_bg.mp3",true);

}


void HomeScene::__addSelectModeUI()
{
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto storyNode = bottomNode->getChildByTag(kStoreModeNode);
    auto battleNode = bottomNode->getChildByTag(kBattleModeNode);
    auto storyMode = MenuItemSprite::create(SPRITE("story_normal.png"), SPRITE("story_press.png"));
    auto battleMode = MenuItemSprite::create(SPRITE("battle_normal.png"), SPRITE("battle_press.png"),SPRITE("battle_disable.png"));
    storyMode->setAnchorPoint(Point(1.0f,0.0f));
    battleMode->setAnchorPoint(Point::ZERO);
    auto battleMenu = Menu::create(battleMode,nullptr);
    storyMode->setPosition(950,185);
    storyMode->setTag(0);
    battleMode->setTag(0);
    battleMode->setPosition(30,330);
    battleMenu->setPosition(Point::ZERO);
    battleNode->addChild(battleMenu);
    battleMenu->setTag(0);
    
    auto storyMenu = Menu::create(storyMode,nullptr);
    storyMenu->setPosition(Point::ZERO);
    storyNode->addChild(storyMenu);
    storyMenu->setTag(0);

    battleMode->setScale(0);
    storyMode->setScale(0);
}

void HomeScene::__addSelectRoleUI()
{
    float roleScale = 0.8f;
    auto vampire = MenuItemSprite::create(SPRITE("selectrole_vampire_normal.png"), SPRITE("selectrole_vampire_press.png"),CC_CALLBACK_1(HomeScene::__roleSelectHandler, this));
    auto roleSize = vampire->getContentSize();
    vampire->setPosition(Point(160,320));
    
    vampire->setScale(roleScale);
    roleSize = Size(roleSize.width*roleScale,roleSize.height*roleScale);
    auto zombie = MenuItemSprite::create(SPRITE("selectrole_zombie_normal.png"), SPRITE("selectrole_zombie_press.png"),CC_CALLBACK_1(HomeScene::__roleSelectHandler, this));
    zombie->setPosition(vampire->getPosition()+Point(roleSize.width+20,-70));
    zombie->setScale(roleScale);
    auto smurf = MenuItemSprite::create(SPRITE("selectrole_smurf_normal.png"), SPRITE("selectrole_smurf_press.png"),CC_CALLBACK_1(HomeScene::__roleSelectHandler, this));
    smurf->setPosition(zombie->getPosition()+Point(roleSize.width+20,130));
    smurf->setScale(roleScale);
    auto viking = MenuItemSprite::create(SPRITE("selectrole_viking_normal.png"), SPRITE("selectrole_viking_press.png"),CC_CALLBACK_1(HomeScene::__roleSelectHandler, this));
    viking->setPosition(smurf->getPosition()+Point(roleSize.width+20,-80));
    viking->setScale(roleScale);
    auto selectRole = Menu::create(vampire,zombie,smurf,viking,nullptr);
    selectRole->setPosition(Point::ZERO);
    m_pBody->addChild(selectRole);
    selectRole->setTag(kSelectRoleMenu);
    selectRole->setOpacity(0);
    selectRole->setEnabled(false);
    
    /* change player menu */
    auto changeNormal = SPRITE("change_role_normal.png");
    auto changePress = SPRITE("change_role_press.png");
    auto changePlayerItem = MenuItemSprite::create(changeNormal, changePress);
    auto changeMenu = Menu::create(changePlayerItem,nullptr);
    changePlayerItem->setPosition(Point(DESIGN_WIDTH-200,30));
    changeMenu->setPosition(Point::ZERO);
    m_pBody->addChild(changeMenu);
    
    rolePos.push_back(vampire->getPosition());
    rolePos.push_back(zombie->getPosition());
    rolePos.push_back(smurf->getPosition());
    rolePos.push_back(viking->getPosition());
}

void HomeScene::__startMenuHandler(cocos2d::Ref *pSender)
{
    int tag = tag1;
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto scaleAct = ScaleTo::create(0.2f, 0);
    while (tag-->0) {
        auto child = bottomNode->getChildByTag(tag);
        child->runAction(scaleAct->clone());
    }
    __selectRole();
    auto item = (MenuItemSprite*)pSender;
    auto menu = (Menu*)item->getParent();
    menu->setEnabled(false);
    
}

void HomeScene::__selectRole()
{
    auto roleMenu = (Menu*)m_pBody->getChildByTag(kSelectRoleMenu);
    auto items = roleMenu->getChildren();
    roleMenu->setOpacity(255);
    roleMenu->setEnabled(true);
    int i=0;
    for(auto &item : items)
    {
        auto menuItem = (MenuItemSprite*)item;
        auto targetPosition = menuItem->getPosition();
        menuItem->setPosition(Point(targetPosition.x,1000));

        auto delay = DelayTime::create(i++*0.2f);
        auto moveAct = MoveTo::create(0.4f, targetPosition);
        auto easeInout = EaseBackInOut::create(moveAct);
        auto moveSeq = Sequence::create(delay,easeInout, nullptr);
        menuItem->runAction(moveSeq);
    }
    
}



void HomeScene::__selectMode()
{
    auto bottomNode = m_pBody->getChildByTag(kBottomNode);
    auto storyNode = bottomNode->getChildByTag(kStoreModeNode);
    auto battleNode = bottomNode->getChildByTag(kBattleModeNode);
    auto battleMenu = (Menu*)battleNode->getChildByTag(0);
    auto storyMenu = (Menu*)storyNode->getChildByTag(0);
    auto battleMenuItem = (MenuItemSprite*)battleMenu->getChildByTag(0);
    auto storyMenuItem = (MenuItemSprite*)storyMenu->getChildByTag(0);
    
    battleMenuItem->runAction(EaseBackInOut::create(ScaleTo::create(0.5f, 0.85f)));
    storyMenuItem->runAction(EaseBackInOut::create(ScaleTo::create(0.5f, 0.9f)));
    
    
}

void HomeScene::__roleSelectHandler(cocos2d::Ref *pSender)
{

    auto menuItem = (MenuItemSprite*)pSender;
    auto children = menuItem->getParent()->getChildren();
    for(auto &item : children)
    {
        auto menuItem = (MenuItemSprite*)item;
        menuItem->unselected();
    }
    menuItem->selected();
    __hideRoles();
}

void HomeScene::__hideRoles()
{
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
        auto showMode = CallFunc::create(CC_CALLBACK_0(HomeScene::__selectMode, this));
        item->runAction(Sequence::create(delay,move,showMode,NULL));
    }
}
