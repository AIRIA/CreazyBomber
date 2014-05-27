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
    
    auto createAnimate = [&](CellAnimation *cellAnimation)->void{
        char animationName[100];
        sprintf(animationName, "%s_%d",mapCell->getCellName().c_str(),cellAnimation->getID());
        
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
    };
    
    auto animations = mapCell->getAnimations();
    log("ani num:%zd",animations.size());
    auto it = animations.begin();
    auto it_end = animations.end();
    for (auto i=0;it!=it_end;it++,i++) {
        
        auto cellAnimation = *it;
        createAnimate(cellAnimation);
    }
    return true;
}


#pragma mark ----------------GroundTile-------------------------------------------------

GroundTile *GroundTile::create(MapCell *mapCell)
{
    auto tile = new GroundTile();
    if(tile&&tile->initWithMapCell(mapCell))
    {
        tile->setMapCell(mapCell);
        char animationName[50];
        sprintf(animationName, "%s_%d",mapCell->getCellName().c_str(),mapCell->getAnimations().at(0)->getID());
        log("run animation :%s",animationName);
        auto animation = AnimationCache::getInstance()->getAnimation(animationName);
        auto animate = Animate::create(animation);
        auto animateRep = RepeatForever::create(animate);
        tile->runAction(animateRep);

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
    if(monster && monster->initWithMonsterName(mapCell->getFileName()))
    {
        monster->autorelease();
        monster->setMapCell(mapCell);
        monster->walk(kWalkRight);
        return monster;
    }
    CC_SAFE_DELETE(monster);
    return nullptr;
}

bool Monster::initWithMonsterName(std::string name)
{
    if (!MapObject::init()) {
        return false;
    }
    m_sName = name;
    int frameNum = 8;
    auto monsterFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
    auto texture = monsterFrame->getTexture();
    auto monsterRect = monsterFrame->getRect();
    auto textureSize = monsterRect.size;
    auto monsterSize = Size(textureSize.width/frameNum,textureSize.height/4);
    auto createAnimate = [&](std::string suffix,Point &startPos)->void{
        char animationName[50];
        sprintf(animationName, "%s_%s",name.c_str(),suffix.c_str());
        auto isExist = AnimationCache::getInstance()->getAnimation(animationName);
        if(isExist!=nullptr)
        {
            return;
        }
        Vector<SpriteFrame*> frameVec;
        for (auto i=0; i<frameNum; i++) {
            auto rect = Rect(monsterRect.origin.x+startPos.x+i*monsterSize.width,monsterRect.origin.y+startPos.y,monsterSize.width,monsterSize.height);
            auto frame = SpriteFrame::createWithTexture(texture, rect);
            frameVec.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(frameVec);
        animation->setDelayPerUnit(0.2f);
        AnimationCache::getInstance()->addAnimation(animation,animationName);
    };
    
    std::vector<std::string> suffixVec = {"up","right","left","down"};
    auto suffixIt = suffixVec.begin();
    auto idx = 0;
    while (suffixIt!=suffixVec.end()) {
        auto pos = Point(0,monsterSize.height*idx);
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
    sprintf(animateName, "%s_%s",m_sName.c_str(),suffix);
    auto animation = AnimationCache::getInstance()->getAnimation(animateName);
    auto walkAct = Animate::create(animation);
    runAction(RepeatForever::create(walkAct));
}