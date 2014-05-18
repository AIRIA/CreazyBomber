//
//  CommonHeaders.h
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-18.
//
//

#ifndef CreazyBomber_CommonHeaders_h
#define CreazyBomber_CommonHeaders_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "VisibleRect.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace CocosDenshion;

#define DESIGN_WIDTH 960
#define DESIGN_HEIGHT 600

#define DESIGN_CENTER Point(DESIGN_WIDTH/2,DESIGN_HEIGHT/2)
#define DESIGN_TOP Point(DESIGN_WIDTH/2,DESIGN_HEIGHT)
#define DESIGN_RIGHT_BOTTOM Point(DESIGN_WIDTH,0)

#define SPRITE(name) Sprite::createWithSpriteFrameName(name)

#endif
