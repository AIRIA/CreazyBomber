//
//  MapTile.h
//  CreazyBomber
//
//  Created by AIRIA on 14-5-22.
//
//

#ifndef __CreazyBomber__MapTile__
#define __CreazyBomber__MapTile__

#include "common/CommonHeaders.h"

class MapTile:public Sprite
{
private:
    int _type;
public:
    int getType(){return _type;};
    void setType(int type){
        _type = type;
    };
    static MapTile *createWithSpriteFrame(SpriteFrame *spriteFrame);
    
    static MapTile *createWithSpriteFrameName(const std::string& spriteFrameName);
};

#endif /* defined(__CreazyBomber__MapTile__) */
