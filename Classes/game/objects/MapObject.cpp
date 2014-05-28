//
//  MapObject.cpp
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-25.
//
//

#include "MapObject.h"
#include "game/MapUtil.h"

void MapObject::onEnter()
{
    Sprite::onEnter();
    if (m_pMapCell==nullptr) {
        return;
    }
    auto mapSizeInPixel = MapUtil::getInstance()->getMapSizeInPixle();
    auto anchor = Point(m_pMapCell->getAnchorX(),1-m_pMapCell->getAnchorY());
    setAnchorPoint(anchor);
    setPosition((getCol()+anchor.x)*80, mapSizeInPixel.height- getRow()*80-80);
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

void MapObject::doTileAnimation()
{
    char animationName[50];
    auto mapCell = getMapCell();
    
    sprintf(animationName, "%s_%s",mapCell->getCellName().c_str(),mapCell->getAnimations().at(0)->getID().c_str());
    auto delaytime = rand()%3+1;
    auto delayAct = DelayTime::create(delaytime);
    auto animation = AnimationCache::getInstance()->getAnimation(animationName);
    auto animate = Animate::create(animation);
    auto animateCallback = CallFunc::create([&]()->void{
        this->doTileAttack();
    });
    auto animateSeq = Sequence::create(delayAct,animate,animateCallback, NULL);
    runAction(animateSeq);
}

void MapObject::doTileDestory()
{
    char animationName[50];
    sprintf(animationName,"%s_%d", getMapCell()->getCellName().c_str(),kTileStatusPrepare);
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
    sprintf(animationName,"%s_%d", getMapCell()->getCellName().c_str(),kTileStatusAttack);
    auto attackAnimation = AnimationCache::getInstance()->getAnimation(animationName);
    auto callback = CallFunc::create([&]()->void{
        this->doTileAttack();
    });
    log("%s",animationName);
    if(prepareAnimation)
    {
        
        if(attackAnimation)
        {
            log("%s","prepare && attack");
            runAction(Sequence::create(Animate::create(prepareAnimation),Animate::create(attackAnimation),callback, NULL));
        }else{
            log("%s","attack");
            runAction(Sequence::create(Animate::create(prepareAnimation),callback, NULL));
        }
    }
    else if(attackAnimation)
    {
        runAction(Sequence::create(Animate::create(attackAnimation),callback, NULL));
    }
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
        tile->doTileAnimation();
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
        monster->walk(kWalkRight);
        return monster;
    }
    CC_SAFE_DELETE(monster);
    return nullptr;
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