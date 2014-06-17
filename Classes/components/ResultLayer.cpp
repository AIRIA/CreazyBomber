//
//  ResultLayer.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-16.
//
//

#include "ResultLayer.h"
#include "game/GameManager.h"

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
}

void ResultLayer::_gameOver(cocos2d::Ref *pSender)
{
    _changeBgColor();
}

void ResultLayer::_showResult(cocos2d::Ref *pSender)
{
    _changeBgColor();
    
    auto wrapper = Node::create();
    wrapper->setScale(GameManager::getInstance()->getScaleFactor());
    wrapper->setPosition(VisibleRect::center()+VisibleRect::leftTop());
    
    auto bg = SPRITE("gameover_win_bg.png");
    wrapper->addChild(bg);
    
    auto infoBg = SPRITE("gameover_win_story_info.png");
    wrapper->addChild(infoBg);
    
    auto star = SPRITE("gameover_star2.png");
    star->setPosition(Point(7,101));
    wrapper->addChild(star);
    
    auto exit = MenuItemSprite::create(SPRITE("exit_normal.png"), SPRITE("exit_press.png"));
    auto retry = MenuItemSprite::create(SPRITE("restart_normal.png"), SPRITE("restart_press.png"));
    auto next = MenuItemSprite::create(SPRITE("next_normal.png"), SPRITE("next_press.png"));
    exit->setPosition(Point(-150,-200));
    retry->setPosition(Point(0,-200));
    next->setPosition(Point(150,-200));
    
    auto menu = Menu::create(exit,retry,next,nullptr);
    menu->ignoreAnchorPointForPosition(false);
    wrapper->addChild(menu);
    
    auto font = Label::createWithBMFont("font/number01.fnt", "0");
    font->setTag(kTagGameScore);
    font->setPosition(0,-15);
    
    auto topScore = Label::createWithBMFont("font/number02.fnt", "0");
    topScore->setPosition(0,-113);
    wrapper->addChild(font);
    wrapper->addChild(topScore);
    wrapper->setTag(kTagWrapper);
    
    auto moveAct = MoveTo::create(0.4f, VisibleRect::center());
    auto easeAct = EaseBackOut::create(moveAct);
    auto showScore = CallFunc::create([&]()->void{
        this->schedule(schedule_selector(ResultLayer::_scoreAnimateSelector), 0.05);
    });
    wrapper->runAction(Sequence::create(DelayTime::create(0.5f),easeAct,showScore,NULL));
    addChild(wrapper);
}

void ResultLayer::_scoreAnimateSelector(float delta)
{
    auto wrapper = getChildByTag(kTagWrapper);
    auto node = wrapper->getChildByTag(kTagGameScore);
    auto score = static_cast<Label*>(node);
    auto number = atoi(score->getString().c_str())+15;
    if(number>=GameManager::getInstance()->getGameScore())
    {
        number = GameManager::getInstance()->getGameScore();
        unscheduleAllSelectors();
        /* 添加是不是创纪录 */
        auto record = SPRITE("new_record.png");
        wrapper->addChild(record);
        record->setPosition(-125,225);
        record->setScale(5);
        record->setOpacity(0);
        auto scaleAct = ScaleTo::create(0.3f, 1.0f);
        auto fadeAct = FadeTo::create(0.3f, 255);
        auto easeScale = EaseBackOut::create(scaleAct);
        auto spawn = Spawn::create(scaleAct,fadeAct, NULL);
        record->runAction(spawn);
    }
    char newScore[20];
    sprintf(newScore, "%d",number);
    score->setString(newScore);
}














