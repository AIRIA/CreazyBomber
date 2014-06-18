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

void Player::onEnter()
{
    MapObject::onEnter();
    scheduleUpdateWithPriority(10);
}

Player *Player::create(MapCell *mapCell)
{
    auto player = new Player();
    if(player&&player->init())
    {
        player->autorelease();
        player->setMapCell(mapCell);
        player->loadPlayerInfo();
        GameManager::getInstance()->setPlayer(player);
        GameManager::getInstance()->setWalkDirection(kWalkDown);
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
    
    auto playerSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(GameConfig::selectedRoleName+"_chuchang.png");
    auto frameSourceRect = playerSpriteFrame->getRect();
    auto frameHeight = frameSourceRect.size.height;
    auto frameWidth = frameSourceRect.size.width/2;
    
    auto createAnimation = [&](std::string suffix,int frameNum,Point &startPos)->void{
        
        auto playerTexture = playerSpriteFrame->getTexture();
        auto playerSourceRect = playerSpriteFrame->getRect();
        auto animationName = GameConfig::selectedRoleName+"_"+suffix;
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
    if(GameConfig::selectedRoleName=="vampire")
    {
        huxi_frameNum = 4;
    }
    std::vector<RoleProperty> chuchang = {
        {"chuchang",0,2}
    };
    registAnimation(chuchang);
    
    playerSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(GameConfig::selectedRoleName+"_huxi.png");
    /* 角色呼吸的动画 */
    std::vector<RoleProperty> huxiVec = {
        {"huxi_up",0,huxi_frameNum},
        {"huxi_right",frameHeight,huxi_frameNum},
        {"huxi_left",frameHeight*2,huxi_frameNum},
        {"huxi_down",frameHeight*3,huxi_frameNum}
    };
    
    registAnimation(huxiVec);
    
    playerSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(GameConfig::selectedRoleName+".png");
    /* 角色行走的动画 */
    std::vector<RoleProperty> walkVec = {
        {"up",0,8},
        {"right",frameHeight,8},
        {"left",frameHeight*2,8},
        {"down",frameHeight*3,8}
    };
    
    if(GameConfig::selectedRoleName=="vampire")
    {
        walkVec.push_back({"down_1",frameHeight*4,9});
        walkVec.push_back({"down_2",frameHeight*5,9});
        walkVec.push_back({"die_1",frameHeight*6,9});
        walkVec.push_back({"die_2",frameHeight*7,9});
    }
    else if(GameConfig::selectedRoleName=="viking")
    {
        walkVec.push_back({"down_1",frameHeight*4,8});
        walkVec.push_back({"down_2",frameHeight*5,7});
        walkVec.push_back({"die_1",frameHeight*6,8});
        walkVec.push_back({"die_2",frameHeight*7,6});
    }
    else if(GameConfig::selectedRoleName=="zombie")
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
    auto animation1 = AnimationCache::getInstance()->getAnimation(GameConfig::selectedRoleName+"_die_1");
    auto animation2 = AnimationCache::getInstance()->getAnimation(GameConfig::selectedRoleName+"_die_2");
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

void Player::run()
{
    auto getAnimate = [](std::string name)->Animate*{
        auto animationName = GameConfig::selectedRoleName+"_"+name;
        auto animation = AnimationCache::getInstance()->getAnimation(animationName);
        return Animate::create(animation);
    };
    
    auto chuchang = getAnimate("chuchang");
    auto huxiSeq = Sequence::create(Repeat::create(chuchang,3),CallFunc::create([&]()->void{
        auto huxi_down = getAnimate("huxi_down");
        this->runAction(RepeatForever::create(huxi_down));
    }), NULL);
    
    runAction(huxiSeq);
    auto arrow = PlayerArrow::getInstance();
    arrow->retain();
    arrow->setAnchorPoint(Point(0.5,0));
    if(GameConfig::selectedRoleName==std::string("zombie"))
    {
        arrow->setPosition(getWidth(),130);
    }
    else
    {
        arrow->setPosition(getWidth(),getHeight()*2);
    }

    addChild(arrow);
}

std::string Player::getDirectionStr()
{
    Player::WalkDirection direct = GameManager::getInstance()->getPrevWalkDirection();
    std::string directionStr;
    switch (direct) {
        case Player::WalkDirection::kWalkUp:
            directionStr = "up";
            break;
        case Player::WalkDirection::kWalkLeft:
            directionStr = "left";
            break;
        case Player::WalkDirection::kWalkRight:
            directionStr = "right";
            break;
        case Player::WalkDirection::kWalkDown:
            directionStr = "down";
            break;
        default:
            directionStr = "";
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
    rapidjson::Value &player = playerInfoDoc[GameConfig::selectedRoleName.c_str()];
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
    if(GameConfig::selectedRoleName==std::string("zombie"))
    {
        rect.origin = Point(rect.origin.x+20,rect.origin.y)+GameManager::getInstance()->getSpeed();
        rect.size = Size(getWidth(),40);
    }
    else if(GameConfig::selectedRoleName==std::string("viking"))
    {
        rect.origin = Point(rect.origin.x+20,rect.origin.y)+GameManager::getInstance()->getSpeed();
        rect.size = Size(getWidth(),50);
    }
    else
    {
        rect.origin = Point(rect.origin.x+20,rect.origin.y+getFootPos())+GameManager::getInstance()->getSpeed();
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
    if(_isCanBeAttack)
    {
        _isCanBeAttack = false;
        setHP(getHP()-heart);
        if(getHP()<=0)
        {
            unscheduleUpdate();
            stopAllActions();
            die();
            NotificationCenter::getInstance()->postNotification(GAME_OVER);
            return;
        }
        schedule(schedule_selector(Player::blink), 0.1, 11, 0);
        auto delayCall = CallFunc::create([&]()->void{
            this->_isCanBeAttack = true;
        });
        runAction(Sequence::create(DelayTime::create(2),delayCall,NULL));
    }
}

void Player::blink(float delta)
{
    auto visible = isVisible();
    setVisible(!visible);
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
                beAttack(50);
                break;
            }
            it++;
        }
    }
    
    
    /* 判断是否有阻挡 */
    auto isCollision = GameManager::getInstance()->getIsCollision();
    if(isCollision)
    {
        GameManager::getInstance()->setIsCollision(false);
    }
    else
    {
        auto targetPosition = getPosition()+GameManager::getInstance()->getSpeed();
        auto coordinate = getCoordinate();
        setPosition(targetPosition);
    }
}

void Player::stopWalkAction()
{
    
}
