//
//  PlayerItem.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-23.
//
//

#include "PlayerItem.h"
#include "game/GameManager.h"

void PlayerItem::onEnter()
{
    Node::onEnter();
    char buffer[50];
    sprintf(buffer, "%s_%d",getItemType().animationName.c_str(),0);
    auto appearAnimation = AnimationCache::getInstance()->getAnimation(buffer);
    auto appearAnimate = Animate::create(appearAnimation);
    auto itemSequence = Sequence::create(appearAnimate,CallFunc::create([&]()->void{
        char buffer[50];
        sprintf(buffer, "%s_%d",getItemType().animationName.c_str(),1);
        auto normalAnimation = AnimationCache::getInstance()->getAnimation(buffer);
        auto repeateAnimate = RepeatForever::create(Animate::create(normalAnimation));
        this->runAction(repeateAnimate);
    }), NULL);
    runAction(itemSequence);
    setAnchorPoint(Point(0.5f,0.0f));
    scheduleUpdate();
}

PlayerItem *PlayerItem::createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
{
    auto sprite = new PlayerItem();
    if (spriteFrame && sprite && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

PlayerItem* PlayerItem::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    return createWithSpriteFrame(frame);
}

void PlayerItem::update(float delta)
{
    auto rect = getBoundingBox();
    rect.size = Size(TILE_WIDTH,TILE_HEIGHT/2);
    auto playerRect = GameManager::getInstance()->getPlayer()->getBoundingBox();
    if(playerRect.intersectsRect(rect))
    {
        auto manager = GameManager::getInstance();
        auto data = PlayerInfoParam::create();
        data->setType(getItemType().itemType);
        switch (data->getType()) {
            case PlayerInfoParam::kTypeBomb:
                manager->setBombNum(manager->getBombNum()+1);
                data->setValue(manager->getBombNum());
                Util::playEffect(SOUND_ITEM_GET_BOMB);
                break;
            case PlayerInfoParam::kTypeCoin:
                if(getIdx()==0)
                {
                    __userDefault->setIntegerForKey(KEY_COIN_NUM, __userDefault->getIntegerForKey(KEY_COIN_NUM)+50);
                    Util::playEffect(SOUND_ITEM_GET_LITTLE_COIN);
                }else{
                    __userDefault->setIntegerForKey(KEY_COIN_NUM, __userDefault->getIntegerForKey(KEY_COIN_NUM)+100);
                    Util::playEffect(SOUND_ITEM_GET_BIG_COIN);
                }
                data->setValue(__userDefault->getIntegerForKey(KEY_COIN_NUM));
                break;
            case PlayerInfoParam::kTypeShoe:
                Util::playEffect(SOUND_ITEM_GET_SHOE);
                manager->setShoe(manager->getShoe()+1);
                data->setValue(manager->getShoe());
                
                if(_speedUp==false)
                {
                    auto player = manager->getPlayer();
                    player->setSpeed(player->getSpeed()*1.3f);
                    checkShoeNum();
                }
                
                break;
            case PlayerInfoParam::kTypePower:
                Util::playEffect(SOUND_ITEM_GET_POWER);
                manager->setBombPower(manager->getBombPower()+1);
                data->setValue(manager->getBombPower());
                break;
            default:
                break;
        }
        manager->score(20);
        NotificationCenter::getInstance()->postNotification(UPDATE_PLAYER_INFO,data);
        removeFromParent();
    }
}

void PlayerItem::checkShoeNum()
{
    _speedUp = true;
    auto seq = Sequence::create(DelayTime::create(10),CallFunc::create([&]()->void{
        auto manager = GameManager::getInstance();
        manager->setShoe(manager->getShoe()-1);
        auto data = PlayerInfoParam::create();
        data->setType(PlayerInfoParam::TYPE::kTypeShoe);
        data->setValue(manager->getShoe());
        NotificationCenter::getInstance()->postNotification(UPDATE_PLAYER_INFO,data);
        if(manager->getShoe()>0)
        {
            this->checkShoeNum();
        }else{
            auto player = manager->getPlayer();
            player->setSpeed(player->getSpeed()/1.3f);
            _speedUp = false;
        }
        
    }), NULL);
    Director::getInstance()->getRunningScene()->runAction(seq);
}

