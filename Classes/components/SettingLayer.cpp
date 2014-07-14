//
//  SettingLayer.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-20.
//
//

#include "SettingLayer.h"
#include "game/GameManager.h"
#include "game/scenes/WelcomeScene.h"
#include "game/MapUtil.h"

SettingLayer *SettingLayer::getInstance()
{
    static SettingLayer *_instance = nullptr;
    if(_instance == nullptr)
    {
        _instance = SettingLayer::create();
    }
    _instance->retain();
    return _instance;
}

void SettingLayer::onEnter()
{
    Layer::onEnter();
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SettingLayer::showPause), SHOW_PAUSE, nullptr);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SettingLayer::showSetting), SHOW_SETTING, nullptr);
}

void SettingLayer::showPause(cocos2d::Ref *pSender)
{
    background = SPRITE("pause_bg.png");
    background->setAnchorPoint(Point(1.0f,1.0f));
    background->setPosition(VisibleRect::rightTop()-Point(100,60));
    show();
}

void SettingLayer::showSetting(cocos2d::Ref *pSender)
{
    background = SPRITE("setting_bg.png");
    show();
}

void SettingLayer::onExit()
{
    Layer::onExit();
    NotificationCenter::getInstance()->removeAllObservers(this);
}

bool SettingLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    return true;
}

void SettingLayer::show()
{
    addBgColor();
    addChild(background);
    background->setScale(GameManager::getInstance()->getScaleFactor());
    auto touchGoon = SPRITE("continue_string.png");
    touchGoon->setScale(background->getScale());
    touchGoon->setAnchorPoint(Point(0.5f,0.0f));
    touchGoon->setPosition(VisibleRect::bottom()+Point(0,50));
    addChild(touchGoon);
    
    auto musicOff = MenuItemSprite::create(SPRITE("music_off_normal.png"), SPRITE("music_off_press.png"));
    auto musicOn = MenuItemSprite::create(SPRITE("music_on_normal.png"), SPRITE("music_on_press.png"));
    auto music = MenuItemToggle::createWithCallback([](Ref *pSender)->void{
    
    }, musicOn,musicOff, nullptr);
    auto replay = MenuItemSprite::create(SPRITE("replay_normal.png"), SPRITE("replay_press.png"));
    auto resume = MenuItemSprite::create(SPRITE("resume_normal.png"), SPRITE("resume_press.png"));
    auto soundOff = MenuItemSprite::create(SPRITE("sound_off_normal.png"), SPRITE("sound_off_press.png"));
    auto soundOn = MenuItemSprite::create(SPRITE("sound_on_normal.png"), SPRITE("sound_on_press.png"));
    auto sound = MenuItemToggle::createWithCallback([](Ref *pSender)->void{
        
    }, soundOn,soundOff,nullptr);
    auto exit = MenuItemSprite::create(SPRITE("white_exit_normal.png"), SPRITE("white_exit_press.png"));
    
    sound->setPosition(Point(100,250));
    music->setPosition(sound->getPosition()+Point(180,0));
    replay->setPosition(Point(100,100));
    resume->setPosition(replay->getPosition()+Point(180,0));
    exit->setPosition(resume->getPosition()+Point(180,0));
    
    auto menu = Menu::create(music,sound,replay,resume,exit,nullptr);
    menu->setPosition(Point::ZERO);
    background->addChild(menu);
    
    music->setCallback([](Ref *pSender)->void{
        auto music = static_cast<MenuItemToggle*>(pSender);
        if(music->getSelectedIndex()==0)
        {
            GameConfig::getInstance()->setSoundEnable(true);
        }else{
            GameConfig::getInstance()->setSoundEnable(false);
        }
    });
    
    sound->setCallback([](Ref *pSender)->void{
        auto sound = static_cast<MenuItemToggle*>(pSender);
        if(sound->getSelectedIndex()==0)
        {
            GameConfig::getInstance()->setEffectEnable(true);
        }else{
            GameConfig::getInstance()->setEffectEnable(false);
        }
    });
    
    replay->setCallback([&](Ref *pSender)->void{
        Director::getInstance()->resume();
        this->removeAllChildren();
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        NotificationCenter::getInstance()->postNotification(GAME_RETRY);
    });
    
    resume->setCallback([&](Ref *pSender)->void{
        
        Director::getInstance()->resume();
        this->removeAllChildren();
    });
    
    exit->setCallback([&](Ref *pSender)->void{
        
        this->removeAllChildren();
        GameManager::getInstance()->setSpeed(Point::ZERO);
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
//        if(getParent())
//        {
//            getParent()->removeFromParent();
//        }
//        Director::getInstance()->getRunningScene()->removeAllChildren();
        Director::getInstance()->purgeCachedData();
        
        WelcomeScene::create()->run();
//        Director::getInstance()->replaceScene(TempScene::scene());
        Director::getInstance()->resume();
    });
    
    if(__userDefault->getBoolForKey(KEY_SOUND_ENABLED)==false)
    {
        music->setSelectedIndex(1);
    }
    
    if(__userDefault->getBoolForKey(KEY_EFFECT_ENABLED)==false)
    {
        sound->setSelectedIndex(1);
    }
    
}
void SettingLayer::addBgColor()
{
    Director::getInstance()->pause();
    auto winSize = Director::getInstance()->getWinSize();
    auto bg = LayerColor::create(Color4B::BLACK, winSize.width, winSize.height);
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch *pTouch,Event *pEvent)->bool{
        Director::getInstance()->resume();
        this->removeAllChildren();
        return true;
    };
    bg->setOpacity(128);
    bg->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, bg);
    addChild(bg);
}
