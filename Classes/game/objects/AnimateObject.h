//
//  AnimateObject.h
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-24.
//
//

#ifndef __CreazyBomber__AnimateObject__
#define __CreazyBomber__AnimateObject__

#include "common/CommonHeaders.h"

class AnimateObject:public Sprite
{
public:
    void cacheAnimate(std::string name,std::string textureName,const Rect &startRect,int frames,float frameTime);
};

#endif /* defined(__CreazyBomber__AnimateObject__) */
