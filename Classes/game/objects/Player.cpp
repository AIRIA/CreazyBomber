//
//  Player.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-29.
//
//

#include "Player.h"
#include "game/GameManager.h"
#include "game/MapUtil.h"
#include "components/PlayerArrow.h"


struct RoleProperty{
    std::string suffix;
    float offsetY;
    int frameNum;
    std::string textureFileName;
};

enum ActionTag{
    kTagRoleWalk,
    kTagRoleStand
};

void Player::onEnter()
{
    MapObject::onEnter();
    unscheduleUpdate();
    scheduleUpdateWithPriority(10);
    Util::playEffect(SOUND_PLAYER_BIRTH);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Player::_revive), PLAYER_REVIVE, nullptr);
}


Player *Player::create(MapCell *mapCell)
{
    auto player = new Player();
    if(player&&player->init())
    {
        player->autorelease();
        player->setMapCell(mapCell);
        player->loadPlayerInfo();
        player->setWalkDirection(kWalkEmpty);
        return player;
    }
    CC_SAFE_FREE(player);
    return nullptr;
}

bool Player::init()
{
    if (!Sprite::init()) {
        return false;
    }
    manager = GameManager::getInstance();
    manager->setPlayer(this);
    
    config = GameConfig::getInstance();
    m_sRoleName = config->getSelectRoleName();
    
    mapUtil = MapUtil::getInstance();

    auto playerSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(GameConfig::getInstance()->getSelectRoleName()+"_chuchang.png");
    auto frameSourceRect = playerSpriteFrame->getRect();
    auto frameHeight = frameSourceRect.size.height;
    auto frameWidth = frameSourceRect.size.width/2;
    
    auto createAnimation = [&](std::string suffix,int frameNum,Point &startPos)->void{
        
        auto playerTexture = playerSpriteFrame->getTexture();
        auto playerSourceRect = playerSpriteFrame->getRect();
        auto animationName = GameConfig::getInstance()->getSelectRoleName()+"_"+suffix;
        Vector<SpriteFrame*> frameVec;
        for(auto i=0;i<frameNum;i++)
        {
            auto x = playerSourceRect.origin.x + startPos.x +i*frameWidth;
            auto y = playerSourceRect.origin.y + startPos.y;
            auto rect = Rect(x,y,frameWidth,frameHeight);
            auto frame = SpriteFrame::createWithTexture(playerTexture, rect);
            frameVec.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(frameVec);
        animation->setDelayPerUnit(0.2f);
        AnimationCache::getInstance()->addAnimation(animation, animationName);
    };
    
    /* 把Vec中的动画注册到动画缓存中 */
    auto registAnimation = [&](std::vector<RoleProperty> vec)->void{
        /* 添加选定角色的方向行走动画 上 右 做 下(随机三个) */
        auto it = vec.begin();
        while(it!=vec.end())
        {
            auto startPoint = Point(0,it->offsetY);
            createAnimation(it->suffix,it->frameNum,startPoint);
            it++;
        }

    };
    
    int huxi_frameNum = 2;
    if(GameConfig::getInstance()->getSelectRoleName()=="vampire")
    {
        huxi_frameNum = 4;
    }
    std::vector<RoleProperty> chuchang = {
        {"chuchang",0,2}
    };
    registAnimation(chuchang);
    
    playerSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(GameConfig::getInstance()->getSelectRoleName()+"_huxi.png");
    /* 角色呼吸的动画 */
    std::vector<RoleProperty> huxiVec = {
        {"huxi_up",0,huxi_frameNum},
        {"huxi_right",frameHeight,huxi_frameNum},
        {"huxi_left",frameHeight*2,huxi_frameNum},
        {"huxi_down",frameHeight*3,huxi_frameNum}
    };
    
    registAnimation(huxiVec);
    
    playerSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(GameConfig::getInstance()->getSelectRoleName()+".png");
    /* 角色行走的动画 */
    std::vector<RoleProperty> walkVec = {
        {"up",0,8},
        {"right",frameHeight,8},
        {"left",frameHeight*2,8},
        {"down",frameHeight*3,8}
    };
    
    if(GameConfig::getInstance()->getSelectRoleName()=="vampire")
    {
        walkVec.push_back({"down_1",frameHeight*4,9});
        walkVec.push_back({"down_2",frameHeight*5,9});
        walkVec.push_back({"die_1",frameHeight*6,9});
        walkVec.push_back({"die_2",frameHeight*7,9});
    }
    else if(GameConfig::getInstance()->getSelectRoleName()=="viking")
    {
        walkVec.push_back({"down_1",frameHeight*4,8});
        walkVec.push_back({"down_2",frameHeight*5,7});
        walkVec.push_back({"die_1",frameHeight*6,8});
        walkVec.push_back({"die_2",frameHeight*7,6});
    }
    else if(GameConfig::getInstance()->getSelectRoleName()=="zombie")
    {
        walkVec.push_back({"down_1",frameHeight*4,9});
        walkVec.push_back({"die_1",frameHeight*5,11});
    }
    else{
        walkVec.push_back({"down_1",frameHeight*4,8});
        walkVec.push_back({"down_2",frameHeight*5,8});
        walkVec.push_back({"die_1",frameHeight*6,2});
    }
    
    registAnimation(walkVec);
    
    return true;
}

void Player::die()
{
    stopAllActions();
    unscheduleUpdate();
    unscheduleAllSelectors();
    GameManager::getInstance()->setIsGameOver(true);
    auto animation1 = AnimationCache::getInstance()->getAnimation(GameConfig::getInstance()->getSelectRoleName()+"_die_1");
    auto animation2 = AnimationCache::getInstance()->getAnimation(GameConfig::getInstance()->getSelectRoleName()+"_die_2");
    auto die1 = Animate::create(animation1);
   
    Sequence *dieSeq = nullptr;
    if(animation2)
    {
        auto die2 = Animate::create(animation2);
        dieSeq = Sequence::create(die1,die2, NULL);
    }else{
        dieSeq = Sequence::create(die1, NULL);
    }
    
    runAction(dieSeq);
}

void Player::_revive(cocos2d::Ref *pSender)
{
    stopAllActions();
    scheduleUpdateWithPriority(10);
    _isCanBeAttack = true;
    GameManager::getInstance()->setIsGameOver(false);
    setWalkSpeed(Point::ZERO);
    beAttack(-100);
    m_WalkDirection = kWalkEmpty;
    stand();
    Util::playSound(SOUND_SCENE_BG);
    Util::playEffect(SOUND_PLAYER_BIRTH);
}

void Player::run()
{
    auto getAnimate = [](std::string name)->Animate*{
        auto animationName = GameConfig::getInstance()->getSelectRoleName()+"_"+name;
        auto animation = AnimationCache::getInstance()->getAnimation(animationName);
        return Animate::create(animation);
    };
    
    auto chuchang = getAnimate("chuchang");
    auto huxiSeq = Sequence::create(Repeat::create(chuchang,3),CallFunc::create([&]()->void{
        this->stand();
    }), NULL);
    
    runAction(huxiSeq);
    auto arrow = PlayerArrow::getInstance();
    arrow->retain();
    arrow->setAnchorPoint(Point(0.5,0));
    if(GameConfig::getInstance()->getSelectRoleName()==std::string("zombie"))
    {
        arrow->setPosition(getWidth(),130);
    }
    else
    {
        arrow->setPosition(getWidth(),getHeight()*2);
    }
    if(arrow->getParent())
    {
        return;
    }
    addChild(arrow);
}

std::string Player::getDirectionStr()
{
    std::string directionStr;
    switch (m_WalkDirection) {
        case WalkDirection::kWalkUp:
            directionStr = "up";
            break;
        case WalkDirection::kWalkLeft:
            directionStr = "left";
            break;
        case WalkDirection::kWalkRight:
            directionStr = "right";
            break;
        case WalkDirection::kWalkDown:
            directionStr = "down";
            break;
        default:
            directionStr = "down";
            break;
    }
    return directionStr;
}

void Player::loadPlayerInfo()
{
    auto filePath = FileUtils::getInstance()->fullPathForFilename("res/players.bin");
    auto data = FileUtils::getInstance()->getStringFromFile(filePath);
    rapidjson::Document playerInfoDoc;
    playerInfoDoc.Parse<0>(data.c_str());
    rapidjson::Value &player = playerInfoDoc[GameConfig::getInstance()->getSelectRoleName().c_str()];
    this->setWidth(player["width"].GetInt());
    this->setHeight(player["height"].GetInt());
    this->setSpeed(player["speed"].GetInt()*2);
    this->setFootPos(player["foot_pos"].GetInt()*2);
    this->setHP(99);
}

Rect Player::getBoundingBox() const
{
    //每个角色的碰撞检测区域都有差别
    auto rect = Node::getBoundingBox();
    if(GameConfig::getInstance()->getSelectRoleName()==std::string("zombie"))
    {
        rect.origin = Point(rect.origin.x+20,rect.origin.y)+getWalkSpeed();
        rect.size = Size(getWidth(),40);
    }
    else if(GameConfig::getInstance()->getSelectRoleName()==std::string("viking"))
    {
        rect.origin = Point(rect.origin.x+20,rect.origin.y)+getWalkSpeed();
        rect.size = Size(getWidth(),50);
    }
    else
    {
        rect.origin = Point(rect.origin.x+20,rect.origin.y+getFootPos()*2)+getWalkSpeed();
        rect.size = Size(getWidth(),40);
    }
    
    return rect;
}

const Point &Player::getCoordinate()
{
    auto pos = getBoundingBox().origin+Point(20,getFootPos());
    float fCol = pos.x / TILE_WIDTH;
    float fRow = (getMapSizeInPixle().height-pos.y-getFootPos())/TILE_HEIGHT;
    int col = fCol;
    int row = fRow;
    col = col<fCol?col++:col;
    row = row<fRow?row++:row;
    _coordinate = Point(col, row);
    if(row<fRow-0.7)
    {
        setZOrder(row*10+2);
    }
    else
    {
        setZOrder(row*10);
    }
    
    return _coordinate;
}



void Player::beAttack(float heart)
{
    if(!_isCanBeAttack)
    {
        return;
    }
    auto blinkAction = Blink::create(2, 5);
    if(heart>0)//伤害的时候播放音效 如果伤害值是负数 说明是复活或者是加血
    {
        Util::playEffect(SOUND_PLAYER_ATTACKED);
    }
    _isCanBeAttack = false;
    auto hp = getHP()-heart;
    setHP(hp);
    if (getHP()<0) {
        setHP(0);
    }
    auto data = Node::create();
    data->setUserData(new int(heart));
    NotificationCenter::getInstance()->postNotification(UPDATE_HP,data);
    if(getHP()<=0)
    {
        die();
        Util::playEffect(SOUND_PLAYER_DEATH);
        NotificationCenter::getInstance()->postNotification(GAME_OVER);
        return;
    }
    runAction(Sequence::create(blinkAction,CallFunc::create([&]()->void{
        _isCanBeAttack = true;
    }), NULL));
}

void Player::walk(WalkDirection direction)
{
    /* 如果方向一样 说明正在行走 */
    if(m_WalkDirection==direction||manager->getIsGameOver())
    {
        return;
    }
    m_WalkDirection = direction;
    /* 需要根据行走的方向 设置速度向量 */
    switch (m_WalkDirection) {
        case WalkDirection::kWalkUp:
            m_WalkSpeed = Point(0,m_fSpeed);
            break;
        case WalkDirection::kWalkLeft:
            m_WalkSpeed = Point(-m_fSpeed,0);
            break;
        case WalkDirection::kWalkRight:
            m_WalkSpeed = Point(m_fSpeed,0);
            break;
        case WalkDirection::kWalkDown:
            m_WalkSpeed = Point(0,-m_fSpeed);
            break;
        default:
            break;
    }
    /* 执行行走的动画 同时停止呼吸动画和之前的行走动画 */
    stopActionByTag(kTagRoleStand);
    stopActionByTag(kTagRoleWalk);
    auto animationName = m_sRoleName+"_"+getDirectionStr();
    auto animate = Animate::create(AnimationCache::getInstance()->getAnimation(animationName));
    auto repeatRunAction = RepeatForever::create(animate);
    repeatRunAction->setTag(kTagRoleWalk);
    runAction(repeatRunAction);
}

void Player::stand()
{
    if(GameManager::getInstance()->getIsGameOver()||m_WalkDirection==kWalkStand)
    {
        return;
    }
    /* 速度重置归零 并且停止之前的行走的动作 */
    this->setWalkSpeed(Point::ZERO);
    stopActionByTag(kWalkStand);
    stopActionByTag(kTagRoleWalk);
    /* 呼吸动画 */
    auto animationName = GameConfig::getInstance()->getSelectRoleName()+"_huxi_"+getDirectionStr();
    auto animate = Animate::create(AnimationCache::getInstance()->getAnimation(animationName));
    auto repeatStandAction = RepeatForever::create(animate);
    repeatStandAction->setTag(kTagRoleStand);
    runAction(repeatStandAction);
    m_WalkDirection = kWalkStand;
}


void Player::update(float delta)
{
    /* 判断是否被炸弹伤害到 */
    if(_isCanBeAttack)
    {
        auto fires = MapUtil::getInstance()->getBombFires();
        auto it = fires.begin();
        while (it!=fires.end()) {
            if(getBoundingBox().intersectsRect((*it)->getBoundingBox()))
            {
                beAttack(DAMAGE_BOMB);
                break;
            }
            it++;
        }
        /* 是否和怪物有碰撞 */
        auto monsters = MapUtil::getInstance()->getMonsters();
        auto monsterIt = monsters.begin();
        while (monsterIt!=monsters.end()) {
            auto monster = *monsterIt;
            auto monsterRect = monster->getBoundingBox();
            monsterRect.origin = Point(monster->getPosition()-Point(TILE_WIDTH/2,monster->getMonsterProperty()->getFootPos()*2));
            monsterRect.size = Size(TILE_WIDTH,TILE_HEIGHT-20);
            if(getBoundingBox().intersectsRect(monsterRect))
            {
                beAttack(monster->getMonsterProperty()->getPower());
                break;
            }
            monsterIt++;
        }
    }
    
    
    auto manager = GameManager::getInstance();
    
    /**
     * 判断是否有阻挡
     * 炸弹人在行走的时候 只允许在每一行的底部 或者每一列的中心线走动
     * 如果在改变方向行走的时候 需要判断 距离那一列或者那一行 比较近 自动行走到指定的行 然后再按照指定的方向行走
     * 如果距离最近的行或者列 处于阻挡的状态 那么就单纯的改变行走方向 并不进行位置的移动
     *
     * 1.获取自己的坐标位置 计算出来未来的拐角点
     * 2.获取拐角判断的位置
     */
    auto pos = getPosition();
    float fCol = pos.x / TILE_WIDTH;
    float fRow = (getMapSizeInPixle().height-pos.y)/TILE_HEIGHT-1;
    auto targetPosition = getPosition()+getWalkSpeed()+manager->getLvDaiSpeed();
    auto isCheck = false;
    auto cornerX = 0,cornerY = 0;
    int col = fCol,row = fRow;
    setRow(row);
    setCol(col);
    setZOrder(row*10+5);
    auto cornerPoint = pos;
    
    switch(m_WalkDirection)
    {
        case kWalkUp:
            if(fRow-row<=0.1)
            {
                cornerY = getMapSizeInPixle().height-TILE_HEIGHT*row;
                row -= 1;
                if(cornerY - (pos.y+TILE_HEIGHT) <abs(getWalkSpeed().y))
                {
                    isCheck = true;
                    cornerPoint.y = cornerY-TILE_HEIGHT;
                }
            }
//            else if(fRow-row>=0.9)
//            {
//                row -= 2;
//            }
            
            
//            row += 1;
//            cornerY = getMapSizeInPixle().height-TILE_HEIGHT*row;
//            if (targetPosition.y > cornerY)
//            {
//                isCheck = true;
//            }
            break;
        case kWalkDown:
            if (fCol==col) {
                
            }
            row += 1;
            cornerY = getMapSizeInPixle().height-TILE_HEIGHT*row;
            if (targetPosition.y < cornerY)
            {
                isCheck = true;
            }
            break;
            
        case kWalkLeft:
//            if(fCol-col<0.5)
//            {
//                col -= 1;
//            }
//            cornerX = (col+0.5)*TILE_WIDTH;
//            if ( abs(targetPosition.x-cornerX)<=abs(getWalkSpeed().x))
//            {
//                isCheck = true;
//                
//            }
//            if(abs(targetPosition.x-cornerX)==0)
//            {
//                col -= 1;
//            }
            
            if(fCol-col>=0.5)
            {
                cornerX = (col+0.5)*TILE_WIDTH;
                col -= 1;
                if (abs(pos.x-cornerX)<=abs(getWalkSpeed().x))
                {
                    isCheck = true;
                    cornerPoint.x = cornerX;
                }
            }
            
            
            
            break;
        case kWalkRight:
            if(fCol-col<=0.5)
            {
                col += 1;
                cornerX = (col-0.5)*TILE_WIDTH;
                if (abs(pos.x-cornerX)<=getWalkSpeed().x)
                {
                    isCheck = true;
                    cornerPoint.x = cornerX;
                }
            }
            break;
        default:
            break;
    }
    
    if(isCheck)
    {
        auto corner = Point(col,row);
        auto tile = mapUtil->getMapObjectFromMapObjectVector(mapUtil->getCommonTiles(), corner);
        if(tile==nullptr&&mapUtil->isBorder(corner)==false)
        {
            setPosition(targetPosition);
        }else{
            setPosition(cornerPoint);
        }
    }else{
        setPosition(targetPosition);
    }
    
    
    
    
//    auto isCollision = manager->getIsCollision();
//    if(isCollision)
//    {
//        manager->setIsCollision(false);
//    }
//    else
//    {
//        auto targetPosition = getPosition()+getWalkSpeed()+manager->getLvDaiSpeed();
//        auto coordinate = getCoordinate();
//        setPosition(targetPosition);
//        manager->setLvDaiSpeed(Point::ZERO);
//    }
    
}



