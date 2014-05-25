//
//  Monster.h
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-24.
//
//

#ifndef __CreazyBomber__Monster__
#define __CreazyBomber__Monster__

#include "AnimateObject.h"
#include "MapTile.h"

class Monster:public AnimateObject
{
public:
    static Monster *create(MapTile *tile);
};

#endif /* defined(__CreazyBomber__Monster__) */
