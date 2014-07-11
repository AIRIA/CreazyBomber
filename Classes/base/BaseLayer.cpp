//
//  BaseLayer.cpp
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-18.
//
//

#include "BaseLayer.h"
#include "game/GameManager.h"

#define EVENT_ASSET_LOADED "event_asset_loaded"

enum Tags
{
    kTagWrapper,
    kTagLoading
};

void BaseLayer::onEnter()
{
    Director::getInstance()->getTextureCache()->removeAllTextures();

    log("%s\n", Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
    Layer::onEnter();
    m_pLeft = __createScaleLayer(Point(0.0f,0.5f), VisibleRect::left());
    m_pBody = __createScaleLayer(Point(0.5f,0.5f), VisibleRect::center());
    m_pRight = __createScaleLayer(Point(1.0f,0.5f), VisibleRect::right());
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseLayer::__loadedNotificationHander), EVENT_ASSET_LOADED, nullptr);
    auto loadingNode = Node::create();
    this->addChild(loadingNode);
    __loadAssets();
}

void BaseLayer::onExit()
{
    Layer::onExit();
    NotificationCenter::getInstance()->removeAllObservers(this);
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void BaseLayer::__loadedNotificationHander(cocos2d::Ref *pObj)
{
    loadedNum++;
    __updateLoadingBar();
    if(loadedNum==textureFiles.size())
    {
        auto it = textureFiles.begin();
        while (it!=textureFiles.end()) {
            std::string fileName = *it;
            log("load texture %s",fileName.c_str());
            Texture2D *texture = Director::getInstance()->getTextureCache()->getTextureForKey(fileName+".pvr.ccz");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(fileName+".plist",texture);
            it++;
        }
        runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([&]()->void{
            onTexturesLoaded();
        }), NULL));
        return;
    }
    
}

void BaseLayer::onTexturesLoaded()
{
    textureFiles.clear();
    auto soundIt = soundFiles.begin();
    while (soundIt!=soundFiles.end()) {
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(soundIt->c_str());
        soundIt++;
    }
    
    auto effectIt = effectFiles.begin();
    while (effectIt!=effectFiles.end()) {
        SimpleAudioEngine::getInstance()->preloadEffect(effectIt->c_str());
        effectIt++;
    }
    
    getChildByTag(kTagWrapper)->removeFromParent();
    log("%s\n", Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
}

void BaseLayer::__loadAssets()
{
    if (textureFiles.size()==0) {
        this->onTexturesLoaded();
    }
    auto iterator = textureFiles.begin();
    while (iterator!=textureFiles.end()) {
        std::string fileName = *iterator+".pvr.ccz";
        Director::getInstance()->getTextureCache()->addImageAsync(fileName.c_str(), &BaseLayer::__loadedHandler);
        iterator++;
    }
}

void BaseLayer::__loadedHandler(cocos2d::Texture2D *texture)
{
    NotificationCenter::getInstance()->postNotification(EVENT_ASSET_LOADED);
}

bool BaseLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    manager = GameManager::getInstance();
    m_winSize = Director::getInstance()->getWinSize();
    m_fScaleFactor = m_winSize.height/DESIGN_HEIGHT;
    if(textureFiles.size()==0)
    {
        
    }
    auto wrapper = Layer::create();
    wrapper->ignoreAnchorPointForPosition(false);
    wrapper->setContentSize(Size(DESIGN_WIDTH,DESIGN_HEIGHT));
    wrapper->setPosition(VisibleRect::center());
    wrapper->setAnchorPoint(Point(0.5f,0.5f));
    wrapper->setScale(m_fScaleFactor);
    auto bg = Sprite::create("images/bg.png");
    bg->setPosition(DESIGN_CENTER);
    wrapper->addChild(bg);
    auto logo = Sprite::create("images/logo.png");
    auto copyright = Sprite::create("images/copyright.png");
    logo->setPosition(DESIGN_CENTER);
    logo->setAnchorPoint(Point(0.5f,0.0f));
    
    copyright->setAnchorPoint(Point(0.5f,0.0f));
    copyright->setPosition(Point(DESIGN_WIDTH/2,0)+Point(0,20));
    wrapper->addChild(logo);
    wrapper->addChild(copyright);
    
    auto loadingBg = Sprite::create("images/loading_bg.png");
    loadingBg->setPosition(Point(DESIGN_WIDTH/2,0)+Point(0,80));
    wrapper->addChild(loadingBg);
    
    auto loading = Sprite::create("images/loading.png");
    auto loadingProgress = ProgressTimer::create(loading);
    loadingProgress->setPosition(Point(DESIGN_WIDTH/2,0)+Point(0,80));
    loadingProgress->setType(ProgressTimer::Type::BAR);
    loadingProgress->setMidpoint(Point::ZERO);
    loadingProgress->setBarChangeRate(Point(1,0));
    loadingProgress->setPercentage(0);
    
    wrapper->setTag(kTagWrapper);
    wrapper->addChild(loadingProgress);
    loadingProgress->setTag(kTagLoading);
    addChild(wrapper);
    return true;
}

void BaseLayer::__updateLoadingBar()
{
    float current = float(loadedNum-1)/float(textureFiles.size())*100;
    float next = float(loadedNum)/float(textureFiles.size())*100;
    auto progress = static_cast<ProgressTimer*>(getChildByTag(kTagWrapper)->getChildByTag(kTagLoading));
    auto progressFromTo = ProgressFromTo::create(0.1, current, next);
    progress->runAction(progressFromTo);
}

void BaseLayer::run()
{
    auto scene = Scene::create();
    scene->addChild(this);
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene==nullptr) {
        Director::getInstance()->runWithScene(scene);
        return;
    }
    Director::getInstance()->replaceScene(scene);
}

auto BaseLayer::__createScaleLayer(const cocos2d::Point &anchorPoint, const cocos2d::Point &position) -> Layer*
{
    auto layer = Layer::create();
    layer->setContentSize(Size(DESIGN_WIDTH,DESIGN_HEIGHT));
    layer->setScale(m_fScaleFactor);
    layer->ignoreAnchorPointForPosition(false);
    layer->setAnchorPoint(anchorPoint);
    layer->setPosition(position);
    this->addChild(layer);
    return layer;
}