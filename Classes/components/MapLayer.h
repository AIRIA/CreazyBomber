//
//  MapLayer.h
//  CreazyBomber
//
//  Created by AIRIA on 14-6-10.
//
//

#ifndef __CreazyBomber__MapLayer__
#define __CreazyBomber__MapLayer__

#include "common/CommonHeaders.h"

class MapLayer:public Node
{
public:
    virtual void onEnter();
    virtual bool init();
    virtual void update(float delta);
    CREATE_FUNC(MapLayer);

protected:
    float _rangeWidth = 0;
    float _rangeHeight = 0;
    Point _offset;
    Point _originPosition;
};

#endif /* defined(__CreazyBomber__MapLayer__) */
