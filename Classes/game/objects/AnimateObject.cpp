//
//  AnimateObject.cpp
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-24.
//
//

#include "AnimateObject.h"

void AnimateObject::cacheAnimate(std::string name, std::string textureName, const cocos2d::Rect &startRect, int frames, float frameTime)
{
    Vector<SpriteFrame*> frameVec;
    auto texture = Director::getInstance()->getTextureCache()->getTextureForKey(textureName);
    for (auto i=0; i<frames; i++) {
        auto rect = Rect(startRect.origin.x*i,startRect.origin.y,startRect.size.width,startRect.size.height);
        auto frame = SpriteFrame::createWithTexture(texture, rect);
        frameVec.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(frameVec);
    AnimationCache::getInstance()->addAnimation(animation, name);
}
