//
//  PlayerArrow.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-12.
//
//

#include "PlayerArrow.h"

enum Tags{
    kArrowSprite = 100
};

PlayerArrow *PlayerArrow::getInstance()
{
    static PlayerArrow *arrow = nullptr;
    if(arrow==nullptr)
    {
        arrow = PlayerArrow::create();
    }
    return arrow;
}

bool PlayerArrow::init()
{
    if(!Node::init())
    {
        return false;
    }
    /* 初始化箭头的动画 */
    auto frameNum = 2;
    auto arrowFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("green_head_sign.png");
    auto arrowTexture = arrowFrame->getTexture();
    auto arrowSourceRect = arrowFrame->getRect(); //在PVR文件中的全局位置
    auto arrowSourceSize = arrowSourceRect.size;
    auto frameWidth = arrowSourceSize.width/frameNum;
    //箭头只有两帧
    Vector<SpriteFrame*> frameVec;
    for (auto i=0;i<frameNum;i++) {
        auto rect = Rect(arrowSourceRect.origin.x+i*frameWidth,arrowSourceRect.origin.y,frameWidth,arrowSourceSize.height);
        auto frame = SpriteFrame::createWithTexture(arrowTexture, rect);
        frameVec.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(frameVec);
    animation->setDelayPerUnit(0.3f);
    AnimationCache::getInstance()->addAnimation(animation, "arrow_animation");
    
    return true;
}

void PlayerArrow::onEnter()
{
    Node::onEnter();
    auto arrow = Sprite::create();
    arrow->setTag(kArrowSprite);
    auto animate = Animate::create(AnimationCache::getInstance()->getAnimation("arrow_animation"));
    arrow->runAction(RepeatForever::create(animate));
    addChild(arrow);
}

void PlayerArrow::onExit()
{
    Node::onExit();
    removeChild(getChildByTag(kArrowSprite));
}










