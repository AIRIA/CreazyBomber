//
//  MapObject.cpp
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-25.
//
//

#include "MapObject.h"
#include "game/MapUtil.h"
#include "game/GameManager.h"

void MapObject::onEnter()
{
    Sprite::onEnter();
    if (m_pMapCell==nullptr) {
        return;
    }
    auto mapSizeInPixel = MapUtil::getInstance()->getMapSizeInPixle();
    auto anchor = Point(m_pMapCell->getAnchorX(),1-m_pMapCell->getAnchorY());
    setMapSizeInPixle(mapSizeInPixel);
    setAnchorPoint(anchor);
    setPosition((getCol()+anchor.x)*TILE_WIDTH, mapSizeInPixel.height- (getRow()+1)*TILE_HEIGHT);
    setZOrder(getRow());
    if(getMapCell()->getCellType()!=88)
    {
        scheduleUpdateWithPriority(-1);
    }
    run();
}

void MapObject::run()
{
    doAnimationWithAttack();
}

/**
 * 根据MapCell的数据 来创建指定的tile的行为和显示信息
 */
bool MapObject::initWithMapCell(MapCell *mapCell)
{
    if (!Sprite::init()) {
        return false;
    }
    auto cellFileName = mapCell->getFileName();
    auto cellFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(cellFileName);
    auto cellTexture = cellFrame->getTexture();
    auto cellSourceRect = cellFrame->getRect(); //在PVR文件中的全局位置
    auto cellSourceSize = cellSourceRect.size;
    
    auto animations = mapCell->getAnimations();
    log("ani num:%zd",animations.size());
    auto it = animations.begin();
    auto it_end = animations.end();
    for (auto i=0;it!=it_end;it++,i++) {
        auto cellAnimation = *it;
        createAnimation(mapCell,cellAnimation);
        if(i==0){
            auto frameWidth = cellAnimation->getWidth()*2;
            auto frameHeight = cellAnimation->getHeight()*2;
            auto x = cellSourceRect.origin.x+cellAnimation->getOffsetX()*2+i*frameWidth;
            auto y = cellSourceRect.origin.y+cellAnimation->getOffsetY()*2;
            auto rect = Rect(x,y,frameWidth,frameHeight);
            auto frame = SpriteFrame::createWithTexture(cellTexture, rect);
            setFirstFrame(frame);
            setSpriteFrame(frame);
        }
    }
    return true;
}

void MapObject::createAnimation(MapCell *mapCell,CellAnimation *cellAnimation,std::string suffix)
{
    auto cellFileName = mapCell->getFileName();
    auto cellFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(cellFileName);
    auto cellTexture = cellFrame->getTexture();
    auto cellSourceRect = cellFrame->getRect(); //在PVR文件中的全局位置
    auto cellSourceSize = cellSourceRect.size;
    char animationName[100];
    suffix = suffix==""?cellAnimation->getID():suffix;
    sprintf(animationName, "%s_%s",mapCell->getCellName().c_str(),suffix.c_str());
    
    auto isExist = AnimationCache::getInstance()->getAnimation(animationName);
    if(isExist!=nullptr)
    {
        return;
    }
    log("cached :%s",animationName);
    Vector<SpriteFrame*> frameVec;
    auto frameWidth = cellAnimation->getWidth()*2;
    auto frameHeight = cellAnimation->getHeight()*2;
    for(auto i=0;i<cellAnimation->getFrameNum();i++)
    {
        auto x = cellSourceRect.origin.x+cellAnimation->getOffsetX()*2+i*frameWidth;
        auto y = cellSourceRect.origin.y+cellAnimation->getOffsetY()*2;
        auto rect = Rect(x,y,frameWidth,frameHeight);
        auto frame = SpriteFrame::createWithTexture(cellTexture, rect);
        frameVec.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(frameVec);
    animation->setDelayPerUnit(cellAnimation->getFrameTime());
    AnimationCache::getInstance()->addAnimation(animation, animationName);
}

Animate *MapObject::getDefaultAnimate()
{
    char animationName[50];
    auto mapCell = getMapCell();
    sprintf(animationName, "%s_%s",mapCell->getCellName().c_str(),mapCell->getAnimations().at(0)->getID().c_str());
    auto animation = AnimationCache::getInstance()->getAnimation(animationName);
    auto animate = Animate::create(animation);
    return animate;
}

void MapObject::doTileAnimation()
{
    auto delaytime = rand()%15+5;
    auto delayAct = DelayTime::create(delaytime);
    auto animateCallback = CallFunc::create([&]()->void{
        this->doTileAnimation();
    });
    auto animateSeq = Sequence::create(delayAct,getDefaultAnimate(),animateCallback, NULL);
    runAction(animateSeq);
}

void MapObject::doAnimationWithAttack()
{
    auto animateCallback = CallFunc::create([&]()->void{
        /* 在此处生成随机行为 来判断是否要进行攻击 */
        int randNum = rand()%4;
        if(randNum==0)
        {
            this->doTileAttack();
        }else{
            this->doAnimationWithAttack();
        }
    });
    auto animateSeq = Sequence::create(getDefaultAnimate(),animateCallback, NULL);
    runAction(animateSeq);
}

void MapObject::doTileDestory()
{
    char animationName[50];
    sprintf(animationName,"%s_%d", getMapCell()->getCellName().c_str(),kTileStatusDestory);
    auto animation = AnimationCache::getInstance()->getAnimation(animationName);
    if(animation!=nullptr)
    {
        auto animate = Animate::create(animation);
        auto animateCallback = CallFunc::create([&]()->void{
            this->removeFromParent();
        });
        runAction(Sequence::create(animate,animateCallback, NULL));
    }
}

void MapObject::doTileAttack()
{
    char animationName[50];
    sprintf(animationName,"%s_%d", getMapCell()->getCellName().c_str(),kTileStatusPrepare);
    auto prepareAnimation = AnimationCache::getInstance()->getAnimation(animationName);
    char animationName2[50];
    sprintf(animationName2,"%s_%d", getMapCell()->getCellName().c_str(),kTileStatusAttack);
    auto attackAnimation = AnimationCache::getInstance()->getAnimation(animationName2);
    auto callback = CallFunc::create([&]()->void{
        this->doAnimationWithAttack();
    });
    /* 如果存在攻击前的准备行为 则调用准备行为后调用攻击动作 攻击完成后 继续执行正常的动作 如果没有准备行为 那么准备行为就是攻击行为 如果两者都没有则进行默认的正常动作 */
    if(prepareAnimation)
    {
        if(attackAnimation)
        {
            runAction(Sequence::create(Animate::create(prepareAnimation),Animate::create(attackAnimation),callback, NULL));
        }else{
            runAction(Sequence::create(Animate::create(prepareAnimation),callback, NULL));
        }
    }
    else if(attackAnimation)
    {
        runAction(Sequence::create(Animate::create(attackAnimation),callback, NULL));
    }
    else
    {
        this->doTileAnimation();
    }
}

Point MapObject::getCurrentCoordinate()
{
    auto pos = getPosition();
        
    float col = (pos.x - getAnchorPoint().x*TILE_WIDTH)/TILE_WIDTH;
    float row = (getMapSizeInPixle().height-pos.y-TILE_HEIGHT)/TILE_HEIGHT;
    setZOrder(row);
    return Point(col, row);
}

Point MapObject::convertCoordinate2Point(const cocos2d::Point &coordinate)
{
    auto mapSizeInPixel = MapUtil::getInstance()->getMapSizeInPixle();
    setMapSizeInPixle(mapSizeInPixel);
    return Point((coordinate.x+0.5)*TILE_WIDTH, mapSizeInPixel.height- coordinate.y*TILE_HEIGHT-TILE_HEIGHT);
}

void MapObject::update(float delta)
{
    auto rect = getBoundingBox();
    rect.size = Size(TILE_WIDTH,TILE_HEIGHT-10);
    auto playerRect = GameManager::getInstance()->getPlayer()->getBoundingBox();
    auto isCollision = playerRect.intersectsRect(rect);
    if(isCollision)
    {
        GameManager::getInstance()->setIsCollision(isCollision);
    }
}



#pragma mark----------------通用的tile-------------------------------

void CommonTile::run()
{
    this->doTileAnimation();
}

#pragma mark-----------------传送门----------------------------------
void TransferDoor::onEnter()
{
    MapObject::onEnter();
    setZOrder(-1);
}

void TransferDoor::update(float delta)
{
    
}


#pragma mark ----------------GroundTile-------------------------------------------------

enum AnimationType{
    kTileDoNormal,
    kTileDoDestory,
    kTileDoAttack
};

GroundTile *GroundTile::create(MapCell *mapCell)
{
    auto tile = new GroundTile();
    if(tile&&tile->initWithMapCell(mapCell))
    {
        tile->setMapCell(mapCell);
        tile->autorelease();
        return tile;
    }
    CC_SAFE_DELETE(tile);
    return nullptr;
}


bool GroundTile::initWithFileName(std::string name)
{
    if (!MapObject::init()) {
        return false;
    }
    
    return true;
}



#pragma mark -----------Monster-----------------------------------------------------------


Monster *Monster::create(MapCell *mapCell)
{
    auto monster = new Monster();
    if(monster && monster->initWithMapCell(mapCell))
    {
        monster->autorelease();
        monster->setMapCell(mapCell);
        return monster;
    }
    CC_SAFE_DELETE(monster);
    return nullptr;
}

void Monster::run()
{
    walk(kWalkRight);
}

bool Monster::initWithMapCell(MapCell *mapCell)
{
    if (!MapObject::init()) {
        return false;
    }
    auto cellAnimation = mapCell->getAnimations().at(0);
    auto frameWidth = cellAnimation->getWidth()*2;
    auto frameHeight = cellAnimation->getHeight()*2;
    
    auto fileName = mapCell->getFileName();
    auto monsterFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
    auto texture = monsterFrame->getTexture();
    auto monsterRect = monsterFrame->getRect();
    int frameNum = monsterRect.size.width/frameWidth;
    
    auto createAnimate = [&](std::string suffix,Point &startPos)->void{
        char animationName[50];
        sprintf(animationName, "%s_%s",fileName.c_str(),suffix.c_str());
        auto isExist = AnimationCache::getInstance()->getAnimation(animationName);
        if(isExist!=nullptr)
        {
            return;
        }
        Vector<SpriteFrame*> frameVec;
        for (auto i=0; i<frameNum; i++) {
            auto rect = Rect(monsterRect.origin.x+startPos.x+i*frameWidth,monsterRect.origin.y+startPos.y,frameWidth,frameHeight);
            auto frame = SpriteFrame::createWithTexture(texture, rect);
            frameVec.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(frameVec);
        animation->setDelayPerUnit(cellAnimation->getFrameTime());
        AnimationCache::getInstance()->addAnimation(animation,animationName);
    };
    
    std::vector<std::string> suffixVec = {"up","right","left","down"};
    auto suffixIt = suffixVec.begin();
    auto idx = 0;
    while (suffixIt!=suffixVec.end()) {
        auto pos = Point(0,frameHeight*idx);
        createAnimate(*suffixIt,pos);
        suffixIt++;
        idx++;
                    
    }
    return true;
}

void Monster::walk(Monster::WalkDirection direc)
{
    auto suffix = "up";
    switch (direc) {
        case kWalkUp:
            suffix = "up";
            break;
        case kWalkDown:
            suffix = "down";
            break;
        case kWalkLeft:
            suffix = "left";
            break;
        case kWalkRight:
            suffix = "right";
            break;
        default:
            break;
    }
    char animateName[50];
    sprintf(animateName, "%s_%s",m_pMapCell->getFileName().c_str(),suffix);
    auto animation = AnimationCache::getInstance()->getAnimation(animateName);
    auto walkAct = Animate::create(animation);
    runAction(RepeatForever::create(walkAct));
}

#pragma mark ----------------履带-------------------

void LvDai::doTileAnimation()
{
    runAction(RepeatForever::create(getDefaultAnimate()));
}

void LvDai::onEnter()
{
    MapObject::onEnter();
    setZOrder(-1);
}

#pragma mark ----------------鬼火-------------------
void GuiHuo::run()
{
    runAction(RepeatForever::create(getDefaultAnimate()));
}

#pragma mark ----------------食人类的物种------------------

void ManEater::run()
{
    this->doAnimationWithAttack();
}

void SnowBallMan::run()
{
    this->doAnimationWithAttack();
}