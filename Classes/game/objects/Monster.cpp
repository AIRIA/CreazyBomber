//
//  Monster.cpp
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-24.
//
//

#include "Monster.h"

//Monster *Monster::_instance = nullptr;

Monster *Monster::create(MapTile *tile)
{
    auto monster = new Monster();
    std::vector<std::string> dircs = {"up","left","right","down"};
    auto wholeSize = tile->getContentSize();
    auto row = 4,col = 8;
    auto tileWidth = wholeSize.width/8;
    auto tileHeight = wholeSize.height/4;
    for (auto i=0; i<row; i++) {
        monster->cacheAnimate(tile->getFrameName()+dircs[i],tile->getFrameName(), Rect(0,tileHeight*i,tileWidth,tileHeight), col, 0.2f);
    }
}