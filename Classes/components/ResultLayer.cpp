//
//  ResultLayer.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-16.
//
//

#include "ResultLayer.h"
#include "game/GameManager.h"
#include "game/scenes/WelcomeScene.h"
#include "game/MapUtil.h"

enum Tags{
    kTagWrapper = 100,
    kTagGameScore
};

bool ResultLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    return true;
}

void ResultLayer::onEnter()
{
    Layer::onEnter();
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ResultLayer::_showResult), GAME_RESULT, nullptr);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ResultLayer::_gameOver), GAME_OVER, nullptr);
}

void ResultLayer::onExit()
{
    Layer::onExit();
    NotificationCenter::getInstance()->removeAllObservers(this);
}

void ResultLayer::_changeBgColor()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto winSize = Director::getInstance()->getWinSize();
    auto bg = LayerColor::create(Color4B::BLACK, winSize.width, winSize.height);
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [](Touch *pTouch,Event *pEvent)->bool{
        return true;
    };
    
    bg->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, bg);
    
    bg->setOpacity(0);
    auto fadeAct = FadeTo::create(1.0f, 128);
    bg->runAction(fadeAct);
    addChild(bg);
    Util::showSpotAds();
}

void ResultLayer::_gameOver(cocos2d::Ref *pSender)
{
    Util::gameAnalyze(kLevelFailed);
    Util::playEffect(SOUND_INGAME_FAILED);
    _changeBgColor();
    auto wrapper = Node::create();
    wrapper->setScale(GameManager::getInstance()->getScaleFactor());
    wrapper->setPosition(VisibleRect::center()+VisibleRect::leftTop());
    
    auto bg = SPRITE("story_lost_bg.png");
    wrapper->addChild(bg);
    
    auto charge = MenuItemSprite::create(SPRITE("shop_charge_btn_normal.png"), SPRITE("shop_charge_btn_press.png"));
    auto revive = MenuItemSprite::create(SPRITE("revive_normal.png"), SPRITE("revive_press.png"));
    revive->setCallback([&](Ref *pSender)->void{
        auto coin = __userDefault->getIntegerForKey(KEY_COIN_NUM);
        if(coin>=REVIVE_COIN)
        {
            NotificationCenter::getInstance()->postNotification(PLAYER_REVIVE);
            auto data = PlayerInfoParam::create();
            data->setType(PlayerInfoParam::kTypeCoin);
            __userDefault->setIntegerForKey(KEY_COIN_NUM, __userDefault->getIntegerForKey(KEY_COIN_NUM)-REVIVE_COIN);
            data->setValue(__userDefault->getIntegerForKey(KEY_COIN_NUM));
            NotificationCenter::getInstance()->postNotification(UPDATE_PLAYER_INFO,data);
            this->removeAllChildren();
        }
        else
        {
            Util::toast("没有足够的金币,赚取积分兑换吧!");
            Util::charge();
        }
    });
    revive->setPosition(Point(0,20));
    charge->setPosition(Point(0,-150));
    charge->setCallback([](Ref *pSender)->void{
        Util::charge();
    });
    
    auto menu = Menu::create(revive,charge,nullptr);
    menu->ignoreAnchorPointForPosition(false);
    wrapper->addChild(menu);
    
    
    auto moveAct = MoveTo::create(0.4f, VisibleRect::center());
    auto easeAct = EaseBackOut::create(moveAct);
    
    auto reviveCoin = Label::createWithBMFont("font/font_01.fnt", __String::createWithFormat("%d",REVIVE_COIN)->getCString());
    reviveCoin->setPosition(-30,-45);
    reviveCoin->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    wrapper->addChild(reviveCoin);
    
    auto coin = Label::createWithBMFont("font/font_01.fnt", __String::createWithFormat("%d",__userDefault->getIntegerForKey(KEY_COIN_NUM))->getCString());
    coin->setPosition(-30,-200);
    coin->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    wrapper->addChild(coin);
    wrapper->runAction(Sequence::create(DelayTime::create(0.5f),easeAct,CallFunc::create([]()->void{
        Util::share();
    }),NULL));
    addChild(wrapper);
    auto exit = MenuItemSprite::create(SPRITE("exit_normal.png"), SPRITE("exit_press.png"));
    auto retry = MenuItemSprite::create(SPRITE("restart_normal.png"), SPRITE("restart_press.png"));

    exit->setPosition(Point(-150,-230));
    exit->setCallback([](Ref *pSender)->void{
        //            GameManager::getInstance()->setSpeed(Point::ZERO);
        WelcomeScene::create()->run();
    });
    retry->setPosition(Point(150,-230));
    retry->setCallback([](Ref *pSender)->void{
        NotificationCenter::getInstance()->postNotification(GAME_RETRY);
    });

    
    auto menu2 = Menu::create(exit,retry,nullptr);
    menu2->ignoreAnchorPointForPosition(false);
    wrapper->addChild(menu2);
    
}

void ResultLayer::_showResult(cocos2d::Ref *pSender)
{
    Util::gameAnalyze(kLevelFinish);
    _changeBgColor();
    auto config = GameConfig::getInstance();
    auto scaleFactor = GameManager::getInstance()->getScaleFactor();
    auto key = __String::createWithFormat("key_%s_level",config->getSelectSceneName().c_str())->getCString();
    auto scaleAct = ScaleTo::create(0.3f, scaleFactor);
    auto scaleEase = EaseBackOut::create(scaleAct);
    auto spawn = Spawn::create(FadeTo::create(0.3, 255),scaleEase, NULL);
    auto hideAct = ScaleTo::create(0.3, 2*scaleFactor);
    auto hideEase = EaseBackIn::create(hideAct);
    auto hideSpawn = Spawn::create(FadeOut::create(0.3f),hideEase, NULL);
    
    auto showResultPanel = [&](Node *parent)->void{
        Util::playEffect(SOUND_INGAME_WIN);
        /* 显示结果面板 */
        auto wrapper = Node::create();
        auto winSize = Director::getInstance()->getWinSize();
        wrapper->setScale(winSize.height/DESIGN_HEIGHT);
        wrapper->setPosition(VisibleRect::center()+VisibleRect::leftTop());
        
        auto bg = SPRITE("gameover_win_bg.png");
        wrapper->addChild(bg);
        
        auto infoBg = SPRITE("gameover_win_story_info.png");
        wrapper->addChild(infoBg);
        
        auto star = SPRITE(this->_getStarImg());
        star->setPosition(Point(7,101));
        wrapper->addChild(star);
        
        auto exit = MenuItemSprite::create(SPRITE("exit_normal.png"), SPRITE("exit_press.png"));
        auto retry = MenuItemSprite::create(SPRITE("restart_normal.png"), SPRITE("restart_press.png"));
        auto next = MenuItemSprite::create(SPRITE("next_normal.png"), SPRITE("next_press.png"));
        next->setCallback([](Ref *pSender)->void{
            NotificationCenter::getInstance()->postNotification(GAME_NEXT);
        });
        exit->setPosition(Point(-150,-200));
        exit->setCallback([](Ref *pSender)->void{
//            GameManager::getInstance()->setSpeed(Point::ZERO);
            WelcomeScene::create()->run();
        });
        retry->setPosition(Point(0,-200));
        retry->setCallback([](Ref *pSender)->void{
            NotificationCenter::getInstance()->postNotification(GAME_RETRY);
        });
        next->setPosition(Point(150,-200));
        
        auto menu = Menu::create(exit,retry,next,nullptr);
        menu->ignoreAnchorPointForPosition(false);
        wrapper->addChild(menu);
        
        auto font = Label::createWithBMFont("font/number01.fnt", "0");
        font->setTag(kTagGameScore);
        font->setPosition(0,-15);
        
        auto topScore = Label::createWithBMFont("font/number02.fnt", "0");
        topScore->setPosition(0,-113);
        auto recordScore = __userDefault->getIntegerForKey(MapUtil::getInstance()->getMapName().c_str(), 0);
        topScore->setString(__String::createWithFormat("%d",recordScore)->getCString());
        wrapper->addChild(font);
        wrapper->addChild(topScore);
        wrapper->setTag(kTagWrapper);
        
        auto moveAct = MoveTo::create(0.4f, VisibleRect::center());
        auto easeAct = EaseBackOut::create(moveAct);
        auto showScore = CallFunc::create([&,parent]()->void{
            Util::playEffect(SOUND_INGAME_COUNT_NUMBER);
            parent->schedule(schedule_selector(ResultLayer::_scoreAnimateSelector), 0.07);
        });
        wrapper->runAction(Sequence::create(DelayTime::create(0.5f),easeAct,showScore,CallFunc::create([]()->void{
            Util::share();
        }),NULL));
        parent->addChild(wrapper);
    };
    
    if (config->getSelectLevel()<12&&config->getSelectLevel()==__userDefault->getIntegerForKey(key))
    {
        __userDefault->setIntegerForKey(key,config->getSelectLevel()+1);
    }
    else
    {
        std::string sceneName = GameConfig::getInstance()->getSelectSceneName();
        auto key = "unlock_md.png";
        if(sceneName=="cl")
        {
            
            if(__userDefault->getBoolForKey(KEY_MD))
            {
                showResultPanel(this);
                return;
            }
            __userDefault->setBoolForKey(KEY_MD, true);
        }
        else if(sceneName=="md")
        {
            
            if(__userDefault->getBoolForKey(KEY_MD))
            {
                showResultPanel(this);
                return;
            }
            key = "unlock_bc.png";
            __userDefault->setBoolForKey(KEY_BC, true);
        }
        auto sprite = SPRITE(key);
        sprite->setOpacity(0);
        sprite->setScale(0.5f*scaleFactor);
        sprite->setPosition(VisibleRect::center());
        addChild(sprite);
        auto seq = Sequence::create(spawn,DelayTime::create(2),hideSpawn,CallFunc::create([&]()->void{
            showResultPanel(this);
        }), NULL);
        sprite->runAction(seq);
        return;
    }
    
    /* 设置解锁信息 */
    key = __String::createWithFormat("key_%s_battle",config->getSelectSceneName().c_str())->getCString();
    if(config->getSelectLevel()==8&&__userDefault->getBoolForKey(key)==false)
    {
        __userDefault->setBoolForKey(key, true);
        
        auto unlockSprite = SPRITE(__String::createWithFormat("unlock_%s_challenge.png",config->getSelectSceneName().c_str())->getCString());
        unlockSprite->setPosition(VisibleRect::center());
        unlockSprite->setOpacity(0);
        unlockSprite->setScale(0.5*scaleFactor);
        auto seq = Sequence::create(spawn,DelayTime::create(2),hideSpawn,CallFunc::create([&,this]()->void{
            showResultPanel(this);
        }), NULL);
        unlockSprite->runAction(seq);
        addChild(unlockSprite);
    }
    else
    {
        showResultPanel(this);
    }
    
}

void ResultLayer::_scoreAnimateSelector(float delta)
{
    auto wrapper = getChildByTag(kTagWrapper);
    auto node = wrapper->getChildByTag(kTagGameScore);
    auto score = static_cast<Label*>(node);
    auto times = 3.0/delta;
    int step = GameManager::getInstance()->getGameScore()/times+1;
    auto number = atoi(score->getString().c_str())+step;
    
    if(number>=GameManager::getInstance()->getGameScore())
    {
        number = GameManager::getInstance()->getGameScore();
        unscheduleAllSelectors();
        if(number>__userDefault->getIntegerForKey(MapUtil::getInstance()->getMapName().c_str(), 0))
        {
            __userDefault->setIntegerForKey(MapUtil::getInstance()->getMapName().c_str(), number);
            /* 添加是不是创纪录 */
            auto record = SPRITE("new_record.png");
            wrapper->addChild(record);
            record->setPosition(-125,225);
            record->setScale(5);
            record->setOpacity(0);
            auto scaleAct = ScaleTo::create(0.3f, 1.0f);
            auto fadeAct = FadeTo::create(0.3f, 255);
            auto spawn = Spawn::create(scaleAct,fadeAct, NULL);
            record->runAction(spawn);
            
        }
    }
    char newScore[20];
    sprintf(newScore, "%d",number);
    score->setString(newScore);
}


std::string ResultLayer::_getStarImg()
{
    auto mapUtil = MapUtil::getInstance();
    auto mapId = mapUtil->getMapName();//地图编号
    auto mapVec = mapUtil->getMapVec();
    auto manager = GameManager::getInstance();
    std::string scoreInfo = "";
    for(auto i=0;i<mapVec.size();i++)
    {
        auto map = mapVec.at(i);
        if (map.at(0)==mapId) {
            scoreInfo = map.at(5);
            manager->score(atoi(map.at(4).c_str()));
            break;
        }
    }
    manager->score(manager->getPlayer()->getHP()*2);
    /* compute time reward */
    auto startTime = 20;
    auto config = GameConfig::getInstance();
    auto scenename = config->getSelectSceneName();
    auto timeReward = (startTime+config->getSelectLevel()*20-manager->getBattleTime())*15+random()%10;
    if (timeReward<0) {
        timeReward = 0;
    }
    manager->score(timeReward);
    
    std::vector<std::string> levelVec;
    levelVec = Util::split(scoreInfo, ";", levelVec);
    int doubleStar = atoi(levelVec.at(0).c_str());
    int threeStar = atoi(levelVec.at(1).c_str());
    auto score = manager->getGameScore();
    auto imgName = "";
    auto key = __String::createWithFormat("%s_rate",mapId.c_str())->getCString();
    auto topRate = __userDefault->getIntegerForKey(key, 0);
    auto rate = 0;
    if(score<doubleStar) //★
    {
        rate = 1;
        imgName = "gameover_star1.png";
    }
    else if(score<threeStar)//★★
    {
        rate = 2;
        imgName = "gameover_star2.png";
    }
    else //★★★
    {
        rate = 3;
        imgName = "gameover_star3.png";
    }
    if(rate>topRate)
    {
        __userDefault->setIntegerForKey(key, rate);
    }
    
    return imgName;
}











