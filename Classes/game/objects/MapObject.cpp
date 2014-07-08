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
#include "game/objects/MonsterSkills.h"

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
    
    if(getType()!=88&&getType()!=12)
    {
        scheduleUpdateWithPriority(-1);
    }
    if(getType()==12)
    {
        setZOrder((getRow()-20)*10);
    }else{
        setZOrder(getRow()*10);
    }
    run();
}

void MapObject::onExit()
{
    Sprite::onExit();
    NotificationCenter::getInstance()->removeAllObservers(this);
}

void MapObject::run()
{
    doAnimationWithAttack();
}

int MapObject::getArgAt(int idx)
{
    return atoi(m_pMapCell->getArgs().at(idx)->getValue().c_str());
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

Animate *MapObject::getAnimateAt(int idx)
{
    char animationName[50];
    auto mapCell = getMapCell();
    sprintf(animationName, "%s_%s",mapCell->getCellName().c_str(),mapCell->getAnimations().at(idx)->getID().c_str());
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
    auto animateSeq = Sequence::create(delayAct,getAnimateAt(0),animateCallback, NULL);
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
    auto animateSeq = Sequence::create(getAnimateAt(0),animateCallback, NULL);
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



#pragma mark----------------通用的tile-------------------------------

void CommonTile::run()
{
    this->doTileAnimation();
}

void CommonTile::onExit()
{
    MapObject::onExit();
    retain();
    MapUtil::getInstance()->getCommonTiles().eraseObject(this);
    NotificationCenter::getInstance()->postNotification(CREATE_PLAYER_ITEM, this);
}

#pragma mark-----------------传送门----------------------------------
void TransferDoor::onEnter()
{
    MapObject::onEnter();
    setZOrder(-1);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(TransferDoor::_enableTransfor), GAME_PASS, nullptr);
    auto config = GameConfig::getInstance();
    unscheduleUpdate();
    if((config->getSelectSceneName()=="md"||config->getSelectSceneName()=="bc")&&config->getSelectLevel()==4)
    {
        _enableTransfor(NULL);
        NotificationCenter::getInstance()->postNotification(DISABLE_BOMB_BUTTON);
    }
    
}

void TransferDoor::onExit()
{
    MapObject::onExit();
    NotificationCenter::getInstance()->removeAllObservers(this);
}

void TransferDoor::run()
{
    auto animate = getAnimateAt(0);
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
    if(GameManager::getInstance()->getIsGameOver())
    {
        return;
    }
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

void GroundTile::onExit()
{
    MapObject::onExit();
    MapUtil::getInstance()->getCommonTiles().eraseObject(this);
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
 
    auto fileName = getMapCell()->getFileName();
    if(fileName=="md_MB_kulouwang.png"||fileName=="bc_MB_zhangyuguai.png"||fileName=="cl_MB_yuanren.png")
    {
        NotificationCenter::getInstance()->postNotification(SHOW_BOSS_HP);
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Monster::bossDead), BOSS_DEAD, nullptr);
    }
    
    auto id = atoi(getMapCell()->getArgs().at(0)->getValue().c_str());
    auto pro = MapUtil::getInstance()->getMonsterProperyById(id);
    if(pro==nullptr)
    {
        log("can't find target monster property :%s",getMapCell()->getCellName().c_str());
        return;
    }
    setMonsterProperty(pro);
    setIsCollison(true);
    float speed = getMonsterProperty()->getSpeed()/30.0f;
    setSpeedRate(speed);
    MapUtil::getInstance()->getMonsters().pushBack(this);
    auto manager = GameManager::getInstance();
    manager->setMonsterCount(manager->getMonsterCount()+1);
    NotificationCenter::getInstance()->postNotification(UPDATE_MONSTER_COUNT);
    MapObject::onEnter();
}

void Monster::bossDead(Ref *pSender)
{
    stopAllActions();
    auto animationName = __String::createWithFormat("%s_%s",getMapCell()->getFileName().c_str(),"dead")->getCString();
    auto dead = Animate::create(AnimationCache::getInstance()->getAnimation(animationName));
    runAction(Sequence::create(dead,CallFunc::create([&]()->void{
        this->stopAllActions();
        this->unscheduleUpdate();
        this->removeFromParent();
        MapUtil::getInstance()->getMonsters().eraseObject(this);
        NotificationCenter::getInstance()->postNotification(UPDATE_MONSTER_COUNT);
        if(MapUtil::getInstance()->getMonsters().size()==0)
        {
            NotificationCenter::getInstance()->postNotification(GAME_PASS);
        }
    }), NULL));
}

void Monster::run()
{
    walk(kWalkRight);
    doTileAttack();
}

void Monster::doTileAttack()
{
    auto delay = DelayTime::create(rand()%5+5);
    auto skillStr = getMonsterProperty()->getSkills();
    
    auto split = [](const std::string &s, char delim, std::vector<std::string> &elems)->std::vector<std::string>&{
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    };
    std::vector<std::string> skills;
    skills = split(skillStr,',',skills);
    

    auto skillLen = skills.size();
    if(skillLen==0)
    {
        return;
    }
    auto skillIdx = rand()%skillLen;
    auto skill = atoi(skills.at(skillIdx).c_str());
    
    auto start = CallFunc::create([&]()->void{
        this->setIsCollison(false);
        this->setVecSpeed(Point::ZERO);
        walk(kWalkDown);
        this->stopActionByTag(10010);
    });
    
    auto end = CallFunc::create([&,skill]()->void{
        this->setIsCollison(true);
        switch (skill) {
            case 1:
                this->fire(4);
                break;
            case 2:
                this->fire(5);
                break;
            case 3:
                this->callMonster(1);
                break;
            case 4:
                this->callMonster(2);
                break;
            case 5:
                this->bullet(3);
                break;
            case 6:
                this->bullet(4);
                break;
            case 7:
                this->guzhua(2);
                break;
            case 8:
                this->guzhua(3);
                break;
            case 9:
                break;
            case 10:
                break;
            case 11:
                this->bullet(4);
                break;
            case 12:
                this->bullet(5);
                break;
            case 13:
                break;
            case 14:
                break;
            case 15:
                this->callMonster(3);
                break;
            case 16:
                this->callMonster(4);
                break;
            case 17:
                this->bullet(5);
                break;
            case 18:
                this->bullet(6);
                break;
            default:
                break;
        }
    });
    
    std::string animationName;
    
    if(getMapCell()->getFileName()=="cl_MB_yuanren.png"&&(skill==1||skill==2))
    {
        animationName = __String::createWithFormat("%s_%s",getMapCell()->getFileName().c_str(),"prepare_attack")->getCString();
    }else{
        animationName = __String::createWithFormat("%s_%s",getMapCell()->getFileName().c_str(),"attack")->getCString();
    }
    auto attack = Animate::create(AnimationCache::getInstance()->getAnimation(animationName));
    runAction(Sequence::create(delay,start,attack, end,DelayTime::create(0.5f),CallFunc::create([&]()->void{
        this->doTileAttack();
    }), nullptr));
    
}

void Monster::fire(int length)
{
    auto mapUtil = MapUtil::getInstance();
    auto mid = length/2;
    for (auto i=-mid; i<=mid; i++) {
        for (auto j=-mid; j<=mid; j++) {
            if((i==mid||i==-mid||j==mid||j==-mid))
            {
                auto col = getCol()+i;
                auto row = getRow()+j;
                auto coordinate = Point(col,row);
                auto tile = mapUtil->getMapObjectFromMapObjectVector(mapUtil->getCommonTiles(), coordinate);
                if(tile==nullptr&&mapUtil->isBorder(coordinate)==false)
                {
                    auto fire = MonsterFire::create();
                    fire->setCol(col);
                    fire->setRow(row);
                    getParent()->addChild(fire);
                }
            }
        }
    }
}

void Monster::bullet(int length)
{
    
}

void Monster::callMonster(int type)
{
    return;
    auto mapUtil = MapUtil::getInstance();
    std::string monsterName;
    switch (type) {
        case 1:
            monsterName = "猪怪";
            break;
        case 2:
             monsterName = "猴怪";
            break;
        case 3:
             monsterName = "小章鱼(怪)";
            break;
        case 4:
             monsterName = "木偶-bt";
            break;
        case 5:
            monsterName = "小章鱼(怪)-bt";
            break;
        default:
            break;
    }
    auto it = mapUtil->getMapCells().begin();
    while (it!=mapUtil->getMapCells().end()) {
        if((*it)->getCellName()==monsterName)
        {
            break;
        }
        it++;
    }
    auto mapCell = *it;
    for(auto i=0;i<3;i++)
    {
        auto monster = Monster::create(mapCell);
        monster->setCol(getCol());
        monster->setRow(getRow());
        getParent()->addChild(monster);
    }
    
}

void Monster::guzhua(int length)
{
    
}

/* 在这里执行NPC的AI逻辑 */
void Monster::update(float delta)
{
    auto fileName = getMapCell()->getFileName();
    
    /* 监测和炸弹的碰撞 */
    auto monsterRect = getBoundingBox();
    monsterRect.origin = getPosition()-Point(TILE_WIDTH/2,0);
    monsterRect.size = Size(TILE_WIDTH,TILE_HEIGHT);
    if(fileName=="md_MB_kulouwang.png"||fileName=="bc_MB_zhangyuguai.png"||fileName=="cl_MB_yuanren.png")
    {
        monsterRect.size.height = TILE_HEIGHT*2;
    }
    auto util = MapUtil::getInstance();
    auto it = util->getBombFires().begin();
    while(it!=util->getBombFires().end())
    {
        auto fire = *it;
        auto rect = fire->boundingBox();
        rect.origin = Point(fire->getPosition()-Point(TILE_WIDTH,TILE_HEIGHT)/2);
        rect.size = Size(TILE_WIDTH,TILE_HEIGHT);
        if(monsterRect.intersectsRect(rect))
        {
            if(fileName=="md_MB_kulouwang.png"||fileName=="bc_MB_zhangyuguai.png"||fileName=="cl_MB_yuanren.png")
            {
                auto data = Node::create();
                data->setUserData(new int(50));
                NotificationCenter::getInstance()->postNotification(UPDATE_BOSS_HP,data);
            }else{
                doTileDestory();
                return;
            }
            
        }
        it++;
    }
    
    /* 怪物的走路碰撞监测 */
    if(getIsCollison())
    {
        auto coordinate = Point::ZERO;
        auto getRandomDirection = [&](WalkDirection direction=kWalkStand)->MapObject*{
            auto mapUtil = MapUtil::getInstance();
            coordinate = Point(getCol(),getRow());
            while(direction==kWalkStand)
            {
                direction = (WalkDirection)(rand()%5-2);
            }
            m_eDirection = direction;
            
            switch (m_eDirection) {
                case kWalkUp:
                    setVecSpeed(Point(0,1));
                    coordinate -= getVecSpeed();
                    break;
                case kWalkDown:
                    setVecSpeed(Point(0,-1));
                    coordinate -= getVecSpeed();
                    break;
                case kWalkLeft:
                    setVecSpeed(Point(-1,0));
                    coordinate += getVecSpeed();
                    break;
                case kWalkRight:
                    setVecSpeed(Point(1,0));
                    coordinate += getVecSpeed();
                    break;
                default:
                    break;
            }
            setVecSpeed(getVecSpeed()*getSpeedRate());
            auto cornerX = TILE_WIDTH*(coordinate.x+0.5);
            auto cornerY = mapUtil->getMapHeightInPixle()-TILE_HEIGHT*(coordinate.y+0.5);
            setCornerPoint(Point(cornerX,cornerY));
            setNextCoordinate(coordinate);
            auto tile = mapUtil->getMapObjectFromMapObjectVector(mapUtil->getCommonTiles(), coordinate);
            if(tile==nullptr)
            {
                tile = mapUtil->getMapObjectFromBombVector(mapUtil->getBomb(), coordinate);
            }
            return tile;
        };
        
        //随机选择方向以后 需要判断要行走的地方是否有障碍 直到找到一个可以行走的方向
        
        MapObject *tile = nullptr;
        if(getSmartWalk())
        {
            tile = getRandomDirection(getSmartDirection());
        }
        else
        {
            tile = getRandomDirection();
        }
        if((tile!=nullptr&&tile->getType()!=kCellTypeTransfer)||util->isBorder(coordinate))
        {
            return;
        }
        walk(m_eDirection);
        setIsCollison(false);
    }
    
    Point nextPosition = getPosition()+getVecSpeed();
    //需要检测即将到底的位置是不是到达了需要判断方向的地方
    
    auto updateCoordinate = [&]()->void{
        setIsCollison(true);
        auto pos = getPosition();
        float fCol = pos.x / TILE_WIDTH;
        float fRow = (getMapSizeInPixle().height-pos.y-getMonsterProperty()->getFootPos())/TILE_HEIGHT;
        int col = fCol;
        int row = fRow;
       
        if(row<fRow-0.7)
        {
            setZOrder(row*10+2);
        }
        else
        {
            setZOrder(row*10);
        }
        col = col<fCol?col++:col;
        row = row<fRow?row++:row;
        setCol(col);
        setRow(row);
    };
    
    switch (m_eDirection) {
        case kWalkRight:
            if(nextPosition.x>=m_CornerPoint.x)
            {
                nextPosition.x = m_CornerPoint.x;
                updateCoordinate();
            }
            break;
        case kWalkLeft:
            if(nextPosition.x<=m_CornerPoint.x)
            {
                nextPosition.x = m_CornerPoint.x;
                updateCoordinate();
            }
            break;
        case kWalkUp:
            if(nextPosition.y+TILE_HEIGHT/2>=m_CornerPoint.y)
            {
                nextPosition.y = m_CornerPoint.y-TILE_HEIGHT/2;
                updateCoordinate();
            }
            break;
        case kWalkDown:
            if(nextPosition.y+TILE_HEIGHT/2<=m_CornerPoint.y)
            {
                nextPosition.y = m_CornerPoint.y-TILE_HEIGHT/2;
                updateCoordinate();
            }
            break;
            
        default:
            break;
    }
    setPosition(nextPosition);
//    if(nextPosition==getPosition())
//    {
//        setCanMove(false);
//    }
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
    if(fileName=="md_MB_kulouwang.png")
    {
        frameNum = 4;
    }
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
    
    if(fileName=="md_MB_kulouwang.png"||fileName=="bc_MB_zhangyuguai.png")
    {
        suffixVec.push_back("attack");
        suffixVec.push_back("dead");
    }
    
    if(fileName=="cl_MB_yuanren.png")
    {
        suffixVec.push_back("attack");
        suffixVec.push_back("prepare_attack");
    }
    
    auto suffixIt = suffixVec.begin();
    auto idx = 0;
    while (suffixIt!=suffixVec.end()) {
        auto pos = Point(0,frameHeight*idx);
        createAnimate(*suffixIt,pos);
        suffixIt++;
        idx++;
                    
    }
    
    if(fileName=="cl_MB_yuanren.png")
    {
        auto pos = Point(0,0);
        monsterFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("cl_MB_yuanren_siwang.png");
        texture = monsterFrame->getTexture();
        monsterRect = monsterFrame->getRect();
        frameNum = monsterRect.size.width/frameWidth;
        createAnimate("dead",pos);
    }
    
    return true;
}

void Monster::walk(WalkDirection direc)
{
//    stopAllActions();
    stopActionByTag(10010);
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
    auto action = RepeatForever::create(walkAct);
    action->setTag(10010);
    runAction(action);
}

void Monster::doTileDestory()
{
    stopAllActions();
    unscheduleUpdate();
    auto blink = Blink::create(1, 5);
    auto blinkHandler = CallFunc::create([&]()->void{
        this->removeFromParent();
        MapUtil::getInstance()->getMonsters().eraseObject(this);
        NotificationCenter::getInstance()->postNotification(UPDATE_MONSTER_COUNT);
        if(MapUtil::getInstance()->getMonsters().size()==0)
        {
            NotificationCenter::getInstance()->postNotification(GAME_PASS);
        }
    });
    runAction(Sequence::create(blink,blinkHandler, NULL));
    Util::playEffect(SOUND_MONSTER_BOMBED_DEAD);
}

#pragma mark ----------------Monster Boss-------------------

bool MonsterBoss::initWithCell(MapCell *mapCell)
{
    Monster::initWithMapCell(mapCell);
    if(getMapCell()->getFileName()=="cl_MB_yuanren.png")
    {
        auto animationName = __String::createWithFormat("%s_die1",getMapCell()->getFileName().c_str())->getCString();
        if(AnimationCache::getInstance()->getAnimation(animationName))
        {
            return true;
        }
        else
        {
            auto frameWidth = getMapCell()->getAnimations().at(0)->getWidth()*2;
            auto frameHeight = getMapCell()->getAnimations().at(0)->getHeight()*2;
            auto monsterFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("cl_MB_yuanren_siwang.png");
            auto texture = monsterFrame->getTexture();
            auto monsterRect = monsterFrame->getRect();
            int frameNum = monsterRect.size.width/frameWidth;
            Vector<SpriteFrame*> frameVec;
            for (auto i=0; i<frameNum; i++) {
                auto rect = Rect(monsterRect.origin.x+i*frameWidth,monsterRect.origin.y,frameWidth,frameHeight);
                auto frame = SpriteFrame::createWithTexture(texture, rect);
                frameVec.pushBack(frame);
            }
            auto animation = Animation::createWithSpriteFrames(frameVec);
            animation->setDelayPerUnit(getMapCell()->getAnimations().at(0)->getFrameTime());
            AnimationCache::getInstance()->addAnimation(animation,animationName);
        }
    }
    return true;
}

#pragma mark ----------------履带-------------------

void LvDai::doTileAnimation()
{
    runAction(RepeatForever::create(getAnimateAt(0)));
}

void LvDai::onEnter()
{
    MapObject::onEnter();
    setZOrder(-1);
}

#define LV_DAI_SPEED 1

void LvDai::update(float delta)
{
    auto manager = GameManager::getInstance();
    auto rect = getBoundingBox();
    if(getType()!=kCellBorder)
    {
        rect.size = Size(TILE_WIDTH,TILE_HEIGHT-10);
    }
    auto playerRect = manager->getPlayer()->getBoundingBox();
    auto isCollision = playerRect.intersectsRect(rect);
    if(isCollision)
    {
        auto direction = atoi(m_pMapCell->getArgs().at(0)->getValue().c_str());
        
        switch (direction) {
            case 0: //up
                manager->setLvDaiSpeed(Point(0,LV_DAI_SPEED));
                break;
            case 1: //down
                manager->setLvDaiSpeed(Point(0,-LV_DAI_SPEED));
                break;
            case 2: //left
                manager->setLvDaiSpeed(Point(-LV_DAI_SPEED,0));
                break;
            case 3: //right
                manager->setLvDaiSpeed(Point(LV_DAI_SPEED,0));
                break;
            default:
                break;
        }
    }
}

#pragma mark ----------------鬼火-------------------
void GuiHuo::run()
{
    runAction(RepeatForever::create(getAnimateAt(0)));
}

#pragma mark ----------------食人类的物种------------------

void ManEater::run()
{
    auto cd = atoi(getMapCell()->getArgs().at(1)->getValue().c_str());
    auto delay = DelayTime::create(cd);
    auto seq = Sequence::create(delay,CallFunc::create([&]()->void{
        this->stopActionByTag(101);
        this->onAttack();
    }),getAnimateAt(1),getAnimateAt(2),CallFunc::create([&]()->void{
        this->run();
    }), NULL);
    auto stand = RepeatForever::create(getAnimateAt(0));
    stand->setTag(101);
    runAction(stand);
    runAction(seq);
}

void ManEater::update(float delta)
{
    auto status = GameManager::getInstance()->getWalkDirection();
    if(status!=kWalkStand)
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
        GameManager::getInstance()->getPlayer()->beAttack(DAMAGE_MONSTER);
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
}

#pragma mark ----------木箱子--------------------------------------------

void WoodBox::update(float delta)
{
    
    if(_isMoving)
    {
        return;
    }
    auto mapUtil = MapUtil::getInstance();
    auto status = GameManager::getInstance()->getWalkDirection();
    if(status!=kWalkStand)
    {
        auto rect = getBoundingBox();
        rect.size = Size(TILE_WIDTH,TILE_HEIGHT-10);
        auto playerRect = GameManager::getInstance()->getPlayer()->getBoundingBox();
        auto isCollision = playerRect.intersectsRect(rect);
        if(isCollision)
        {
            GameManager::getInstance()->setIsCollision(isCollision);
            if (GameConfig::getInstance()->getSelectLevel()==11&&GameConfig::getInstance()->getSelectSceneName()=="cl") {
                return;
            }
            /* 根据行走碰撞的方向 执行不同的操作 */
            auto direction = GameManager::getInstance()->getWalkDirection();
            setMovingDirection(direction);
            auto nextCoordinate = Point(getCol(),getRow());
            auto offset = Point::ZERO;
            m_Anchor = Point::ZERO;
            m_Scale= Point::ZERO;
            switch (direction) {
                case kWalkUp:
                    offset = Point(0,-1);
                    m_Anchor = Point(0.5f,1.0f);
                    m_Scale = Point(1,0);
                    break;
                case kWalkDown:
                    offset = Point(0,1);
                    m_Anchor = Point(0.5f,0.0f);
                    m_Scale = Point(1,0);
                    break;
                case kWalkLeft:
                    offset = Point(-1,0);
                    m_Anchor = Point(0.0f,0.5f);
                    m_Scale = Point(0,1);
                    break;
                case kWalkRight:
                    offset = Point(1,0);
                    m_Anchor = Point(1.0f,0.5f);
                    m_Scale = Point(0,1);
                    break;
                default:
                    break;
            }
            nextCoordinate += offset;
            if(mapUtil->isBorder(nextCoordinate))
            {
                return;
            }
            offset = Point(offset.x*TILE_WIDTH,offset.y*-1*TILE_HEIGHT);
            auto tile = mapUtil->getMapObjectFromMapObjectVector(mapUtil->getCommonTiles(), nextCoordinate);
            if(tile==nullptr||tile->getType()==kCellTypeMonster)
            {
                _isMoving = true;
                GameManager::getInstance()->getMovingBoxes().pushBack(this);
                auto moveAct = MoveBy::create(0.3,offset);
                auto moveCall = CallFunc::create([&,nextCoordinate]()->void{
                    setCol(nextCoordinate.x);
                    setRow(nextCoordinate.y);
                    setZOrder(getRow()*10);
                    _isMoving = false;
                    GameManager::getInstance()->getMovingBoxes().eraseObject(this);
                });
                runAction(Sequence::create(moveAct,moveCall, NULL));
            }
        }
    }
}

void WoodBox::onExit()
{
    MapObject::onExit();
    retain();
    MapUtil::getInstance()->getCommonTiles().eraseObject(this);
    NotificationCenter::getInstance()->postNotification(CREATE_PLAYER_ITEM, this);
}

#pragma mark -------------------------EmptyObject------------------
auto EmptyObject::createWithSpriteFrame(cocos2d::SpriteFrame *frame) -> EmptyObject*
{
    auto eo = new EmptyObject();
    if(eo&&eo->initWithSpriteFrame(frame))
    {
        eo->autorelease();
        return eo;
    }
    CC_SAFE_FREE(eo);
    return nullptr;
}

EmptyObject* EmptyObject::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    
    return createWithSpriteFrame(frame);
}

void EmptyObject::doTileDestory()
{
    //do nothing
}

void EmptyObject::onEnter()
{
    MapObject::onEnter();
    MapUtil::getInstance()->getCommonTiles().pushBack(this);
}

void EmptyObject::onExit()
{
    MapObject::onExit();
    MapUtil::getInstance()->getCommonTiles().eraseObject(this);
}


#pragma mark-------------地洞------------------------------

void DiDong::update(float delta)
{
    MapObject::update(delta);
    /* 判断玩家和地洞是不是相邻 */
    auto playerCoordiante = GameManager::getInstance()->getPlayer()->getCoordinate();
    if((abs(playerCoordiante.x-getCol())==1&&playerCoordiante.y==getRow())||(abs(playerCoordiante.y-getRow())==1&&playerCoordiante.x==getCol()))
    {
        m_bIsEnable = false;
        m_bIsAttack = true;
        m_pPlayerCoordiante = playerCoordiante;
    }
    else if(m_bIsAttack)
    {
        m_bIsEnable = true;
        m_bIsAttack = false;
        char animationName[50];
        sprintf(animationName,"%s_%d", getMapCell()->getCellName().c_str(),kTileStatusPrepare);
        auto prepareAnimation = AnimationCache::getInstance()->getAnimation(animationName);
        auto attack = Animate::create(prepareAnimation);
        runAction(Sequence::create(attack,CallFunc::create([&]()->void{
            auto mapUtil = MapUtil::getInstance();
            auto it = mapUtil->getMapCells().begin();
            while (it!=mapUtil->getMapCells().end()) {
                if((*it)->getCellName()==std::string("蘑菇怪"))
                {
                    break;
                }
                it++;
            }
            auto mapCell = *it;
            auto monster = Monster::create(mapCell);
            monster->setCol(m_pPlayerCoordiante.x);
            monster->setRow(m_pPlayerCoordiante.y);
            getParent()->addChild(monster);
        }), NULL));
    }
}

#pragma mark ------------树手-----------------------

void ShuShou::update(float delta)
{
    auto player = GameManager::getInstance()->getPlayer();
    auto playerRect = player->getBoundingBox();
    playerRect.size.height = 20;
    auto rect = getBoundingBox();
    rect.origin = getPosition()-Point(TILE_WIDTH/2,0);
    rect.size = Size(TILE_WIDTH,TILE_HEIGHT);
    if(rect.intersectsRect(playerRect))
    {
        player->beAttack(DAMAGE_MONSTER);
    }
}

void ShuShou::run()
{
    unscheduleUpdate();
    /* 攻击的延迟  */
    auto delayTime = getArgAt(2);
    auto delayAct = DelayTime::create(delayTime);
    runAction(Sequence::create(delayAct,CallFunc::create([&]()->void{
        this->doTileAnimation();
    }), NULL));
}

void ShuShou::doTileAnimation()
{
    // 1攻击 2停顿1秒 3执行隐藏 4停顿4秒 5循环执行
    auto startCallback = CallFunc::create([&]()->void{
        this->scheduleUpdate();
    });
    
    auto endCallback = CallFunc::create([&]()->void{
        this->unscheduleUpdate();
    });
    auto animateSeq = Sequence::create(startCallback,getAnimateAt(1),endCallback,getAnimateAt(0),DelayTime::create(4),CallFunc::create([&]()->void{
        this->doTileAnimation();
    }), NULL);
    runAction(animateSeq);
}


#pragma mark ------------地刺-----------------------

void DiCi::update(float delta)
{
    auto player = GameManager::getInstance()->getPlayer();
    auto playerRect = player->getBoundingBox();
    auto rect = getBoundingBox();
    rect.origin = getPosition()-Point(TILE_WIDTH/2,0);
    rect.size = Size(TILE_WIDTH,TILE_HEIGHT);
    if(rect.intersectsRect(playerRect))
    {
        player->beAttack(DAMAGE_MONSTER);
    }
}

void DiCi::run()
{
    unscheduleUpdate();
    doTileAnimation();
}

void DiCi::doTileAnimation()
{
    auto startCallback = CallFunc::create([&]()->void{
        this->scheduleUpdate();
    });
    
    auto endCallback = CallFunc::create([&]()->void{;
        this->unscheduleUpdate();
    });
    auto delay = DelayTime::create(rand()%3+1);
    auto seq = Sequence::create(delay,startCallback,getAnimateAt(1),endCallback,getAnimateAt(0),CallFunc::create([&]()->void{
        this->doTileAnimation();
    }), NULL);
    runAction(seq);
}

#pragma mark -------------火墙------------------

void FireWall::onEnter()
{
    Sprite::onEnter();
    if (m_pMapCell==nullptr) {
        return;
    }
    auto mapSizeInPixel = MapUtil::getInstance()->getMapSizeInPixle();
    setMapSizeInPixle(mapSizeInPixel);
    setAnchorPoint(Point::ANCHOR_BOTTOM_RIGHT);
    setPosition(getCol()*TILE_WIDTH, mapSizeInPixel.height-(getRow()+1)*TILE_HEIGHT);
    scheduleUpdateWithPriority(-1);
    run();
}

void FireWall::run()
{
    runAction(RepeatForever::create(getAnimateAt(0)));
    setZOrder(10000);
    _originPosition = getPosition();
}

void FireWall::update(float delta)
{
    auto rect = getBoundingBox();
    rect.origin = Point(rect.origin.x+TILE_WIDTH,rect.origin.y);
    rect.size = Size(TILE_WIDTH,TILE_HEIGHT);
    auto player = GameManager::getInstance()->getPlayer();
    auto playerRect = player->getBoundingBox();
    if (rect.intersectsRect(playerRect)) {
        player->beAttack(33);
    }
    offsetX++;
    if(offsetX==500)
    {
        auto fire = FireWall::create(getMapCell());
        fire->setCol(0);
        fire->setRow(getRow());
        getParent()->addChild(fire);
    }
    setPositionX(getPositionX()+1);
    if(offsetX>=getMapSizeInPixle().width)
    {
        removeFromParent();
    }
}

#pragma mark -------------雪球和虫子------------------

void SnowBallOrWorm::onEnter()
{
    MapObject::onEnter();
    _originPosition = getPosition();
    /* 根据方向设定偏移量 */
    auto args = getMapCell()->getArgs();
//    auto speed = atoi(args.at(0)->getValue().c_str());
    cdTime = atoi(args.at(1)->getValue().substr(0,1).c_str());
//    auto startTime = atoi(args.at(1)->getValue().substr(2,1).c_str());
    auto direction = atoi(args.at(2)->getValue().substr(0,1).c_str());
    distance = atoi(args.at(2)->getValue().substr(2,1).c_str());
    setOpacity(255);
    switch (direction) {
        case 1: //Down
            this->speed = Point(0,-1);
            break;
        case 2: //left
            this->speed = Point(-1,0);
            break;
        case 3: //right
            this->speed = Point(1,0);
            break;
        case 4:
            break;
            
        default:
            break;
    }
    
}

void SnowBallOrWorm::run()
{
    auto repAct = RepeatForever::create(getAnimateAt(0));
    
    if(getMapCell()->getFileName()=="md_chongzi.png")
    {
        doMove();
    }
    else
    {
        runAction(repAct);
    }
}

void SnowBallOrWorm::doMove()
{
    isCanMove = false;
    auto seq = Sequence::create(getAnimateAt(0),CallFunc::create([this]()->void{
        this->isCanMove = true;
        auto move = RepeatForever::create(getAnimateAt(2));
        move->setTag(1010);
        runAction(move);
        
    }),nullptr);
    runAction(seq);
    setOpacity(255);
}

void SnowBallOrWorm::update(float delta)
{
    if(isCanMove)
    {
        offset++;
        setPosition(getPosition()+speed*1.2);
    }
    
    auto stopRoll = [this]()->void{
        if(getMapCell()->getFileName()!="md_chongzi.png")
        {
            setOpacity(0);
            setPosition(_originPosition);
        }
        else
        {
            this->stopActionByTag(1010);
            isCanMove = false;
            this->runAction(Sequence::create(this->getAnimateAt(1),CallFunc::create([&]()->void{
                setPosition(_originPosition);
                this->setOpacity(0);
            }),DelayTime::create(cdTime),CallFunc::create([&]()->void{
                this->doMove();
            }),NULL) );
        }
        
        offset = 0;
        unscheduleUpdate();
        runAction(Sequence::create(DelayTime::create(cdTime),CallFunc::create([this]()->void{
            this->setOpacity(255);
            this->scheduleUpdateWithPriority(-1);
        }), NULL));
    };
    if(offset>=distance*TILE_WIDTH)
    {
        stopRoll();
        return;
    }
    auto rect = getBoundingBox();
    rect.origin.x += 1;
    rect.origin.y += 1;
    rect.size = Size(TILE_WIDTH-2,TILE_HEIGHT-2);
    auto objs = MapUtil::getInstance()->getCommonTiles();
    for (auto i=0; i<objs.size(); i++) {
        auto obj = objs.at(i);
        auto objRect = Rect(obj->getCol()*TILE_WIDTH,m_MapSizeInPixle.height-(obj->getRow()+1)*TILE_HEIGHT,TILE_WIDTH,TILE_HEIGHT);
        if(rect.intersectsRect(objRect))
        {
            stopRoll();
            return;
        }
    }
    
    auto player = GameManager::getInstance()->getPlayer();
    if(rect.intersectsRect(player->getBoundingBox()))
    {
        player->beAttack(DAMAGE_MONSTER);
    }
    
}

