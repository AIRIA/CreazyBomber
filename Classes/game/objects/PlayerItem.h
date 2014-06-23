//
//  PlayerItem.h
//  CreazyBomber
//
//  Created by AIRIA on 14-6-23.
//
//

#ifndef __CreazyBomber__PlayerItem__
#define __CreazyBomber__PlayerItem__

#include "common/CommonHeaders.h"
#include "game/data/PlayerInfoParam.h"
#include "game/objects/MapObject.h"


struct PlayerItemType{
    std::string frameName;
    std::string animationName;
    float perFrameDelay;
    PlayerInfoParam::TYPE itemType;
};

class PlayerItem:public Sprite
{
public:
    virtual void onEnter();
    virtual void update(float delta);
    CREATE_FUNC(PlayerItem);
    static PlayerItem *createWithSpriteFrame(SpriteFrame *frame);
    PlayerItem *createWithSpriteFrameName(const std::string& spriteFrameName);
    CC_SYNTHESIZE(PlayerItemType, m_ItemType, ItemType);
    CC_SYNTHESIZE(int, m_iIdx, Idx);
};

#endif /* defined(__CreazyBomber__PlayerItem__) */
