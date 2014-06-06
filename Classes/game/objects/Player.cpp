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


struct RoleProperty{
    std::string suffix;
    float offsetY;
    int frameNum;
    std::string textureFileName;
};

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
        {"down",frameHeight*3,8},
        
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
    
    scheduleUpdate();
    
    return true;
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
    this->setWidth(player["width"].GetInt()*2);
    this->setHeight(player["height"].GetInt()*2);
    this->setSpeed(player["speed"].GetInt()*2);
    this->setFootPos(player["foot_pos"].GetInt()*2);
}

void Player::update(float delta)
{
    float block = 20;
    //判断两个Rect是否相交
    bool res = false;
    auto rect = getBoundingBox();
    rect.origin = Point(rect.origin.x+20,rect.origin.y+getFootPos()*2);
    rect.size = Size(getWidth()-40,block*2);
    auto convert2Coordinate = [&](Point &pos)->Point{
        float col = pos.x/TILE_WIDTH-1;
        float row = (getMapSizeInPixle().height-pos.y)/TILE_HEIGHT-1;
        setZOrder(row);
        return Point(col, row);
    };
    
    auto targetPos = getPosition()+GameManager::getInstance()->getSpeed();
    auto coordiPos = targetPos + Point(0,this->getFootPos()*2);
    auto coordinate = convert2Coordinate(coordiPos);
    int row = coordinate.y;
    int col = coordinate.x;
    if(col<=coordinate.x-rect.size.width/getWidth()/2)
    {
        col++;
    }
   
    if(row<coordinate.y-block/TILE_HEIGHT)
    {
        row++;
    }
    int neighborRow = row;
    int neighborCol = col;
    
    
    //根据方向进行不同逻辑的碰撞检测
    auto direction = GameManager::getInstance()->getCurrentWalkDirection();
    switch (direction) {
        case kWalkUp:
            neighborCol++;
            neighborRow--;
            row--;
            log("coordinate.x:%f,coordinate.y:%f,row:%d,neighborRow:%d,col:%d,neighborCol:%d",coordinate.x,coordinate.y,row,neighborRow,col,neighborCol);
            break;
        case kWalkDown:
            neighborCol++;
            neighborRow++;
            row++;
            log("coordinate.x:%f,coordinate.y:%f,row:%d,neighborRow:%d,col:%d,neighborCol:%d",coordinate.x,coordinate.y,row,neighborRow,col,neighborCol);
            break;
        case kWalkLeft:
            neighborRow++;
            neighborCol--;
            col--;
            log("coordinate.x:%f,coordinate.y:%f,row:%d,neighborRow:%d,col:%d,neighborCol:%d",coordinate.x,coordinate.y,row,neighborRow,col,neighborCol);
            break;
        case kWalkRight:
            neighborRow++;
            neighborCol++;
            col++;
            log("coordinate.x:%f,coordinate.y:%f,row:%d,neighborRow:%d,col:%d,neighborCol:%d",coordinate.x,coordinate.y,row,neighborRow,col,neighborCol);
            break;
        default:
            break;
    }
    
    
    auto testIntersets = [&](const Point &coordinate)->bool{
        auto mapObj = MapUtil::getInstance()->getMapObjectByCoordinate(coordinate);
        if(mapObj&&mapObj!=this)
        {
            auto cellRect = mapObj->getBoundingBox();
            cellRect.size.height = cellRect.size.height-45;
            return cellRect.intersectsRect(rect);
        }
        return false;
    };
    if(testIntersets(Point(col,row)) || testIntersets(Point(neighborCol,neighborRow)))
    {
        res = true;
    }
    
    if (!res) {
        setPosition(getPosition()+GameManager::getInstance()->getSpeed());
    }
}


