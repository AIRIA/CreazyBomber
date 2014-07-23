//
//  MonsterSkills.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-7-2.
//
//

#include "MonsterSkills.h"
#include "game/MapUtil.h"
#include "game/GameManager.h"

enum Tag{
    kTagFire
};

bool MonsterFire::init()
{
    if(!Node::init())
    {
        return false;
    }
    setFileName("cl_MBJ_huo.png");
    
    auto animation = AnimationCache::getInstance()->getAnimation(getFileName());
    if(animation==nullptr)
    {
        Util::addAnimation(getFileName(), 7);
    }
    return true;
}

void MonsterFire::onEnter()
{
    Node::onEnter();
    
    auto mapSizeInPixel = MapUtil::getInstance()->getMapSizeInPixle();
    auto anchor = getAnchorPoint();
    setPosition((getCol()+anchor.x)*TILE_WIDTH, mapSizeInPixel.height- (getRow()+1)*TILE_HEIGHT);
    auto sprite = Sprite::create();
    sprite->setAnchorPoint(Point::ZERO);
    auto animate = Animate::create(AnimationCache::getInstance()->getAnimation(getFileName()));
    sprite->runAction(Sequence::create(animate,CallFunc::create([&]()->void{
        this->removeFromParent();
    }), NULL));
    addChild(sprite);
    sprite->setTag(kTagFire);
    scheduleUpdate();
}

void MonsterFire::update(float delta)
{
    auto player = GameManager::getInstance()->getPlayer();
    auto sprite = static_cast<Sprite*>(getChildByTag(kTagFire));
    auto rect = sprite->getBoundingBox();
    rect.origin = getPosition();
    rect.size = Size(TILE_WIDTH,TILE_HEIGHT);
    auto playerRect = player->getBoundingBox();
    if(rect.intersectsRect(playerRect))
    {
        player->beAttack(DAMAGE_MONSTER_FIRE);
    }
}

#pragma mark --------------------monster dici--------------

bool MonsterDiCi::init()
{
    if(!Node::init())
    {
        return false;
    }
    
    setFileName("md_MBJ-guzhua.png");
    auto animation = AnimationCache::getInstance()->getAnimation(getFileName());
    if(animation==nullptr)
    {
        Util::addAnimation(getFileName(), 9);
    }
    return true;
}

#pragma mark------------------monster bomb--------------------

void MonsterBomb::onEnter()
{
    auto anchor = Point(0.5f,0);
    setAnchorPoint(anchor);
    setPosition((getCol()+anchor.x)*TILE_WIDTH,MapUtil::getInstance()->getMapSizeInPixle().height - (getRow()+1-anchor.y)*TILE_HEIGHT);
    Sprite::onEnter();
    const char *animationName = "boss-zd.png";
    auto animate = Animate::create(AnimationCache::getInstance()->getAnimation(animationName));
    auto animateCall = CallFunc::create([&]()->void{
        this->bomb();
    });
    MapUtil::getInstance()->getCommonTiles().pushBack(this);
    MapUtil::getInstance()->getMonsterBombs().pushBack(this);
    runAction(Sequence::create(Repeat::create(animate, 8),animateCall, NULL));
    Util::playEffect(SOUND_ITEM_PUT_BOMB);
}

void MonsterBomb::onExit()
{
    Bomb::onExit();
    MapUtil::getInstance()->getMonsterBombs().eraseObject(this);
}

MonsterBomb *MonsterBomb::create(BombType type)
{
    auto bomb = new MonsterBomb();
    if(bomb&&bomb->init())
    {
        bomb->autorelease();
        bomb->setType(kCellTypeBomb);
        MapUtil::getInstance()->getBomb().pushBack(bomb);
        return bomb;
    }
    CC_SAFE_FREE(bomb);
    return nullptr;
}

bool MonsterBomb::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    _isBombed = false;
    initBombAnimations();
    return true;
}

void MonsterBomb::initBombAnimations()
{
    Util::addAnimation("boss-zd.png", 4,0.1f);
    for (auto i=1; i<=9; i++) {
        char name[50];
        sprintf(name, "boss-zd0%d.png",i);
        Util::addAnimation(name, 9,0.1f);
    }
}

void MonsterBomb::bomb()
{
    Util::playEffect(SOUND_ITEM_BOMB_EXPLODE);
    auto getAnimateByName = [](std::string animationName)->Animate*{
        auto animation = AnimationCache::getInstance()->getAnimation(animationName);
        auto animate = Animate::create(animation);
        return animate;
    };
    auto removeHandler = CallFuncN::create([](Ref *pSender)->void{
        auto node = static_cast<Node*>(pSender);
        node->removeFromParent();
    });
    
    
    auto center = MonsterBombFire::create();
    center->setRow(getRow());
    center->setCol(getCol());
    center->runAction(Sequence::create(getAnimateByName("boss-zd01.png"),removeHandler,NULL));
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
                sprintf(name, "boss-zd0%d.png",2*(i+1));
            }
            else
            {
                sprintf(name, "boss-zd0%d.png",2*(i+1)+1);
            }
            
            auto targetCoordiante = Point(getCol(),getRow())+directions[i]*j;
            if(MapUtil::getInstance()->isBorder(targetCoordiante))
            {
                break;
            }
            auto mapUtil = MapUtil::getInstance();
            auto tile = mapUtil->getMapObjectFromMapObjectVector(mapUtil->getCommonTiles(), targetCoordiante);
            if(tile==nullptr)
            {
                tile = mapUtil->getMapObjectFromBombFireVector(mapUtil->getBombFires(), targetCoordiante);
            }
            if(tile&&tile!=GameManager::getInstance()->getPlayer())
            {
                if(tile->getType()!=kCellTypeBombFire)
                {
                    tile->doTileDestory();
                }
                break;
            }
            
            auto bombFire = MonsterBombFire::create();
            bombFire->setRow(targetCoordiante.y);
            bombFire->setCol(targetCoordiante.x);
            bombFire->setAnchorPoint(getAnchorPoint());
            bombFire->setPosition(GameManager::getInstance()->getPlayer()->convertCoordinate2Point(targetCoordiante));
            bombFire->runAction(Sequence::create(getAnimateByName(name),removeHandler->clone(), NULL));
            GameManager::getInstance()->getMapTileLayer()->addChild(bombFire);
            
        }
    }
    removeFromParent();
    MapUtil::getInstance()->getBomb().eraseObject(this);
}

MonsterBombFire *MonsterBombFire::create()
{
    auto bf = new MonsterBombFire();
    if(bf&&bf->init())
    {
        bf->autorelease();
        bf->setType(kCellTypeBombFire);
        return bf;
    }
    CC_SAFE_DELETE(bf);
    return nullptr;
}

//void MonsterBombFire::onEnter()
//{
//    MapObject::onEnter();
//}
//
//void MonsterBombFire::onExit()
//{
//    MapObject::onExit();
//}
