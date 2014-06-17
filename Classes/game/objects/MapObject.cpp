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
    setZOrder(getRow()*10);
    if(getMapCell()->getCellType()!=88)
    {
        scheduleUpdateWithPriority(-1);
    }
    MapUtil::getInstance()->getMapObjects().pushBack(this);
    run();
}

void MapObject::onExit()
{
    Sprite::onExit();
    MapUtil::getInstance()->getMapObjects().eraseObject(this);
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
        stopAllActions();
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
    
    auto attackCallback = CallFunc::create([&]()->void{
        auto delay = DelayTime::create(0.05f);
        auto delayCall = CallFunc::create([&]()->void{
            this->onAttack();
        });
        this->runAction(Sequence::create(delay,delayCall, NULL));
    });
    
    /* 如果存在攻击前的准备行为 则调用准备行为后调用攻击动作 攻击完成后 继续执行正常的动作 如果没有准备行为 那么准备行为就是攻击行为 如果两者都没有则进行默认的正常动作 */
    if(prepareAnimation)
    {
        if(attackAnimation)
        {
            runAction(Sequence::create(Animate::create(prepareAnimation),attackCallback,Animate::create(attackAnimation),callback, NULL));
        }else{
            runAction(Sequence::create(attackCallback,Animate::create(prepareAnimation),callback, NULL));
        }
    }
    else if(attackAnimation)
    {
        runAction(Sequence::create(attackCallback,Animate::create(attackAnimation),callback, NULL));
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
    auto status = GameManager::getInstance()->getWalkDirection();
    if(status!=Player::kWalkStand)
    {
        auto rect = getBoundingBox();
        if(getType()!=kCellBorder)
        {
            rect.size = Size(TILE_WIDTH,TILE_HEIGHT-10);
        }
        auto playerRect = GameManager::getInstance()->getPlayer()->getBoundingBox();
        auto isCollision = playerRect.intersectsRect(rect);
        if(isCollision)
        {
            GameManager::getInstance()->setIsCollision(isCollision);
        }
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
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(TransferDoor::_enableTransfor), GAME_PASS, nullptr);
    unscheduleUpdate();
}

void TransferDoor::onExit()
{
    MapObject::onExit();
    NotificationCenter::getInstance()->removeAllObservers(this);
}

void TransferDoor::run()
{
    auto animate = getDefaultAnimate();
    runAction(RepeatForever::create(animate));
}

void TransferDoor::_enableTransfor(cocos2d::Ref *pSender)
{
    stopAllActions();
    char animationName[50];
    sprintf(animationName,"%s_%d", getMapCell()->getCellName().c_str(),4);
    auto prepareAnimation = AnimationCache::getInstance()->getAnimation(animationName);
    runAction(RepeatForever::create(Animate::create(prepareAnimation)));
    scheduleUpdate();
}

void TransferDoor::update(float delta)
{
    auto rect = getBoundingBox();
    auto playerRect = GameManager::getInstance()->getPlayer()->Node::getBoundingBox();
    playerRect.origin.x = playerRect.origin.x+playerRect.size.width/2;
    playerRect.origin.y = playerRect.origin.y+playerRect.size.height/2;
    playerRect.size = Size(1,1);
    auto isCollision = playerRect.intersectsRect(rect);
    if(isCollision)
    {
        unscheduleUpdate();
        GameManager::getInstance()->setIsGameOver(true);
        GameManager::getInstance()->getPlayer()->unscheduleUpdate();
        auto scaleAct = ScaleTo::create(0.5f, 0.0f);
        auto scaleHandler = CallFunc::create([]()->void{
            NotificationCenter::getInstance()->postNotification(GAME_RESULT);
        });
        GameManager::getInstance()->getPlayer()->runAction(Sequence::create(scaleAct,scaleHandler, NULL));
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

void Monster::onEnter()
{
    MapObject::onEnter();
    auto id = atoi(getMapCell()->getArgs().at(0)->getValue().c_str());
    auto pro = MapUtil::getInstance()->getMonsterProperyById(id);
    if(pro==nullptr)
    {
        log("can't find target monster property :%s",getMapCell()->getCellName().c_str());
        return;
    }
    setMonsterProperty(pro);
    /* 刚开始的时候设定成已经碰撞的状态 开始执行方向的选择操作 */
    setIsCollison(true);
}

void Monster::run()
{
    walk(kWalkRight);
}

/* 在这里执行NPC的AI逻辑 */
void Monster::update(float delta)
{
    /* 监测和炸弹的碰撞 */
    auto monsterRect = getBoundingBox();
    auto it = MapUtil::getInstance()->getBombFires().begin();
    while(it!=MapUtil::getInstance()->getBombFires().end())
    {
        auto rect = (*it)->boundingBox();
        if(monsterRect.intersectsRect(rect))
        {
            doTileDestory();
            return;
        }
        it++;
    }
    if(getIsCollison())
    {
        auto getRandomDirection = [&]()->MapObject*{
            auto coordiante = Point(getCol(),getRow());
            m_eDirection = (WalkDirection)(rand()%4);
            switch (m_eDirection) {
                case kWalkUp:
                    setVecSpeed(Point(0,1));
                    coordiante -= getVecSpeed();
                    break;
                case kWalkDown:
                    setVecSpeed(Point(0,-1));
                    coordiante -= getVecSpeed();
                    break;
                case kWalkLeft:
                    setVecSpeed(Point(-1,0));
                    coordiante += getVecSpeed();
                    break;
                case kWalkRight:
                    setVecSpeed(Point(1,0));
                    coordiante += getVecSpeed();
                    break;
                default:
                    break;
            }
            auto cornerX = TILE_WIDTH*(coordiante.x+0.5);
            auto cornerY = MapUtil::getInstance()->getMapHeightInPixle()-TILE_HEIGHT*(coordiante.y+0.5);
            setCornerPoint(Point(cornerX,cornerY));
            setNextCoordinate(coordiante);
            return MapUtil::getInstance()->getMapObjectByCoordinate(coordiante);
        };
        
        //随机选择方向以后 需要判断要行走的地方是否有障碍 直到找到一个可以行走的方向
        auto tile = getRandomDirection();
        while(tile!=nullptr&&tile->getMapCell()!=nullptr&&tile->getMapCell()->getCellType()!=kCellTypeTransfer)
        {
            tile = getRandomDirection();
        }
        walk(m_eDirection);
        setIsCollison(false);
        
       
    }
    
//    auto speed = getMonsterProperty()->getSpeed();
    setVecSpeed(getVecSpeed()*1);
    auto nextPosition = getPosition()+getVecSpeed();
    //需要检测即将到底的位置是不是到达了需要判断方向的地方
    switch (m_eDirection) {
        case kWalkRight:
            if(nextPosition.x>=m_CornerPoint.x)
            {
                nextPosition.x = m_CornerPoint.x;
                setIsCollison(true);
                setCol(getNextCoordinate().x);
                setRow(getNextCoordinate().y);
                setZOrder(getRow()*10);
            }
            break;
        case kWalkLeft:
            if(nextPosition.x<=m_CornerPoint.x)
            {
                nextPosition.x = m_CornerPoint.x;
                setIsCollison(true);
                setCol(getNextCoordinate().x);
                setRow(getNextCoordinate().y);
                setZOrder(getRow()*10);
            }
            break;
        case kWalkUp:
            if(nextPosition.y+TILE_HEIGHT/2>=m_CornerPoint.y)
            {
                nextPosition.y = m_CornerPoint.y-TILE_HEIGHT/2;
                setIsCollison(true);
                setCol(getNextCoordinate().x);
                setRow(getNextCoordinate().y);
                setZOrder(getRow()*10);
            }
            break;
        case kWalkDown:
            if(nextPosition.y+TILE_HEIGHT/2<=m_CornerPoint.y)
            {
                nextPosition.y = m_CornerPoint.y-TILE_HEIGHT/2;
                setIsCollison(true);
                setCol(getNextCoordinate().x);
                setRow(getNextCoordinate().y);
                setZOrder(getRow()*10);
            }
            break;
            
        default:
            break;
    }
    setPosition(nextPosition);
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
    stopAllActions();
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

void Monster::doTileDestory()
{
    stopAllActions();
    unscheduleUpdate();
    auto blink = Blink::create(1, 5);
    auto blinkHandler = CallFunc::create([&]()->void{
        this->removeFromParent();
        MapUtil::getInstance()->getMonsters().eraseObject(this);
        if(MapUtil::getInstance()->getMonsters().size()==0)
        {
            NotificationCenter::getInstance()->postNotification(GAME_PASS);
        }
    });
    runAction(Sequence::create(blink,blinkHandler, NULL));
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

void ManEater::update(float delta)
{
    auto status = GameManager::getInstance()->getWalkDirection();
    if(status!=Player::kWalkStand)
    {
        auto direc = atoi(getMapCell()->getArgs().at(0)->getValue().c_str());
        auto rect = getBoundingBox();
        /* 根据食人鱼的方向来进行碰撞检测 */
        if(direc==2) //left
        {
            rect.origin.x += TILE_WIDTH;
            rect.size = Size(TILE_WIDTH,TILE_HEIGHT-10);
        }
        else if(direc==3) //right
        {
            rect.size = Size(TILE_WIDTH,TILE_HEIGHT-10);
        }
        
        auto playerRect = GameManager::getInstance()->getPlayer()->getBoundingBox();
        auto isCollision = playerRect.intersectsRect(rect);
        if(isCollision)
        {
            GameManager::getInstance()->setIsCollision(isCollision);
        }
    }
}

void ManEater::onAttack()
{
    auto direc = atoi(getMapCell()->getArgs().at(0)->getValue().c_str());
    auto rect = getBoundingBox();
    /* 根据食人鱼的方向来进行碰撞检测 */
    if(direc==2) //left
    {
        rect.size = Size(TILE_WIDTH,TILE_HEIGHT-30);
    }
    else if(direc==3) //right
    {
        rect.origin.x += TILE_WIDTH;
        rect.size = Size(TILE_WIDTH,TILE_HEIGHT-30);
    }
    
    auto playerRect = GameManager::getInstance()->getPlayer()->getBoundingBox();
    auto isCollision = playerRect.intersectsRect(rect);
    if(isCollision)
    {
        GameManager::getInstance()->getPlayer()->beAttack(33);
    }
}

#pragma mark ----------------吐雪球的建筑------------------

void SnowBallMan::run()
{
    this->doAnimationWithAttack();
}

auto MapBorder::createWithSpriteFrame(cocos2d::SpriteFrame *frame) -> MapBorder*
{
    auto mb = new MapBorder();
    if(mb&&mb->initWithSpriteFrame(frame))
    {
        mb->autorelease();
        mb->setType(kCellBorder);
        return mb;
    }
    CC_SAFE_FREE(mb);
    return nullptr;
}

MapBorder* MapBorder::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    
    return createWithSpriteFrame(frame);
}

void MapBorder::onEnter()
{
    Sprite::onEnter();
    scheduleUpdateWithPriority(-1);
    MapUtil::getInstance()->getMapObjects().pushBack(this);
    log("border show");
}

#pragma mark ----------木箱子--------------------------------------------

void WoodBox::update(float delta)
{
    if(_isMoving)
    {
        return;
    }
    auto status = GameManager::getInstance()->getWalkDirection();
    if(status!=Player::kWalkStand)
    {
        auto rect = getBoundingBox();
        rect.size = Size(TILE_WIDTH,TILE_HEIGHT-10);
        auto playerRect = GameManager::getInstance()->getPlayer()->getBoundingBox();
        auto isCollision = playerRect.intersectsRect(rect);
        if(isCollision)
        {
            GameManager::getInstance()->setIsCollision(isCollision);
            /* 根据行走碰撞的方向 执行不同的操作 */
            auto direction = GameManager::getInstance()->getWalkDirection();
            auto nextCoordinate = Point(getCol(),getRow());
            auto offset = Point::ZERO;
            switch (direction) {
                case Player::kWalkUp:
                    offset = Point(0,-1);
                    break;
                case Player::kWalkDown:
                    offset = Point(0,1);
                    break;
                case Player::kWalkLeft:
                    offset = Point(-1,0);
                    break;
                case Player::kWalkRight:
                    offset = Point(1,0);
                    break;
                default:
                    break;
            }
            nextCoordinate += offset;
            if(nextCoordinate.x==0)
            {
                return;
            }
            offset = Point(offset.x*TILE_WIDTH,offset.y*-1*TILE_HEIGHT);
            auto tile = MapUtil::getInstance()->getMapObjectByCoordinate(nextCoordinate);
            if(tile==nullptr)
            {
                _isMoving = true;
                setCol(nextCoordinate.x);
                setRow(nextCoordinate.y);
                auto moveAct = MoveBy::create(0.4,offset);
                auto moveCall = CallFunc::create([&]()->void{
                    _isMoving = false;
                });
                runAction(Sequence::create(moveAct,moveCall, NULL));
            }
            
        }
    }
}
