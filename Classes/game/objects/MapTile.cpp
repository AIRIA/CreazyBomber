//
//  MapTile.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-22.
//
//

#include "MapTile.h"

MapTile *MapTile::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    return createWithSpriteFrame(frame);
}

MapTile *MapTile::createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
{
    MapTile *sprite = new MapTile();
    if (spriteFrame && sprite && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}