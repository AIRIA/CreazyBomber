//
//  Bomb.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-3.
//
//

#include "Bomb.h"
#include "game/GameManager.h"
#include "game/MapUtil.h"

void Bomb::onEnter()
{
    Sprite::onEnter();
    char animationName[50];
    if (getBombType()==kBombTimer) {
        sprintf(animationName, "%s-timer-zd.png",GameConfig::selectedRoleName.c_str());
    }
    else
    {
        sprintf(animationName, "%s-zd.png",GameConfig::selectedRoleName.c_str());
    }
    auto animate = Animate::create(AnimationCache::getInstance()->getAnimation(animationName));
    auto animateCall = CallFunc::create([&]()->void{
        auto start = time(nullptr);

        this->bomb();
        auto end = time(nullptr);
        log("bomb time:%ld",end-start);
    });
    //scheduleUpdateWithPriority(-1);
    MapUtil::getInstance()->getMapObjects().pushBack(this);
    runAction(Sequence::create(Repeat::create(animate, 8),animateCall, NULL));
}


Bomb *Bomb::create(Bomb::BombType type)
{
    auto bomb = new Bomb();
    if(bomb&&bomb->init())
    {
        bomb->autorelease();
        bomb->setBombType(type);
        return bomb;
    }
    CC_SAFE_FREE(bomb);
    return nullptr;
}

bool Bomb::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    _isBombed = false;
    initBombAnimations();
    return true;
}

void Bomb::doTileDestory()
{
    if (!_isBombed) {
        _isBombed = true;
        bomb();
    }
}

void Bomb::initBombAnimations()
{
    auto createBombAnimation = [](std::string fileName,int frameNum,float delay=0.2f)->void{
        if(AnimationCache::getInstance()->getAnimation(fileName))
        {
            return;
        }
        
        auto bombFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
        auto texture = bombFrame->getTexture();
        auto rect = bombFrame->getRect();
        auto size = rect.size;
        auto frameWidth = size.width/frameNum;
        auto frameHeight = size.height;
        frameNum = frameNum>5?5:frameNum;
        Vector<SpriteFrame*> frameVec;
        for(auto i=0;i<frameNum;i++)
        {
            auto frameRect = Rect(rect.origin.x+frameWidth*i,rect.origin.y,frameWidth,frameHeight);
            auto frame = SpriteFrame::createWithTexture(texture, frameRect);
            frameVec.pushBack(frame);
        }
        
        auto animation = Animation::createWithSpriteFrames(frameVec);
        animation->setDelayPerUnit(delay);
        AnimationCache::getInstance()->addAnimation(animation, fileName);
    };
    
    createBombAnimation(GameConfig::selectedRoleName+"-timer-zd.png",2);
    createBombAnimation(GameConfig::selectedRoleName+"-zd.png",2);
    for (auto i=1; i<=9; i++) {
        char name[50];
        sprintf(name, "%s-zd0%d.png",GameConfig::selectedRoleName.c_str(),i);
        if(GameConfig::selectedRoleName==std::string("viking"))
        {
            createBombAnimation(name,8,0.1f);
        }
        else
        {
            createBombAnimation(name,9,0.1f);
        }
        
    }
}

void Bomb::bomb()
{
    
    auto getAnimateByName = [](std::string animationName)->Animate*{
        auto animation = AnimationCache::getInstance()->getAnimation(animationName);
        auto animate = Animate::create(animation);
        return animate;
    };
    auto removeHandler = CallFuncN::create([](Ref *pSender)->void{
        auto node = static_cast<Node*>(pSender);
        node->removeFromParent();
    });
    
    
    auto center = BombFire::create();
    center->setRow(getRow());
    center->setCol(getCol());
    center->runAction(Sequence::create(getAnimateByName((GameConfig::selectedRoleName+"-zd01.png").c_str()),removeHandler,NULL));
    center->setPosition(getPosition());
    center->setAnchorPoint(getAnchorPoint());
    GameManager::getInstance()->getMapTileLayer()->addChild(center);
    
    std::vector<Point> directions = {Point(-1,0),Point(0,-1),Point(+1,0),Point(0,+1)};
    for(auto i=0;i<directions.size();i++)
    {
        for(auto j=1;j<=m_iPower;j++)
        {
            char name[50];
            if(j==m_iPower)
            {
                sprintf(name, "%s-zd0%d.png",GameConfig::selectedRoleName.c_str(),2*(i+1));
            }
            else
            {
                sprintf(name, "%s-zd0%d.png",GameConfig::selectedRoleName.c_str(),2*(i+1)+1);
            }
            
            auto targetCoordiante = Point(getCol(),getRow())+directions[i]*j;
            auto tile = MapUtil::getInstance()->getMapObjectByCoordinate(targetCoordiante);
            if(tile&&tile!=GameManager::getInstance()->getPlayer())
            {
                tile->doTileDestory();
                if(tile->getMapCell()->getCellType()!=kCellTypeMonster)
                {
                    break;
                }
            }
 
            auto bombFire = BombFire::create();
            bombFire->setRow(targetCoordiante.y);
            bombFire->setCol(targetCoordiante.x);
            bombFire->setAnchorPoint(getAnchorPoint());
            bombFire->setPosition(GameManager::getInstance()->getPlayer()->convertCoordinate2Point(targetCoordiante));
            bombFire->runAction(Sequence::create(getAnimateByName(name),removeHandler->clone(), NULL));
            bombFire->setZOrder(getZOrder()-2);
            GameManager::getInstance()->getMapTileLayer()->addChild(bombFire);
            
        }
    }
    removeFromParent();
}

#pragma mark ------------------------BombFire---------------------------

BombFire *BombFire::create()
{
    auto bf = new BombFire();
    if(bf&&bf->init())
    {
        bf->autorelease();
        return bf;
    }
    CC_SAFE_DELETE(bf);
    return nullptr;
}

void BombFire::onEnter()
{
    MapObject::onEnter();
    MapUtil::getInstance()->getBombFires().pushBack(this);
}

void BombFire::onExit()
{
    MapObject::onExit();
    MapUtil::getInstance()->getBombFires().eraseObject(this);
}





