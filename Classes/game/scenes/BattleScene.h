//
//  BattleScene.h
//  CreazyBomber
//
//  Created by AIRIA on 14-5-21.
//
//

#ifndef __CreazyBomber__BattleScene__
#define __CreazyBomber__BattleScene__

#include "base/BaseLayer.h"
#include "external/tinyxml2/tinyxml2.h"
#include "game/objects/MapTile.h"

using namespace std;
using namespace tinyxml2;

class BattleScene:public BaseLayer
{
    
private:
    XMLDocument doc;
    ValueMap tiles;
    MapTile *__getTileByName(const std::string &name);
    void __initTileSet();
public:
    virtual void onTexturesLoaded();
    virtual bool init();
    CREATE_FUNC(BattleScene);
    
protected:
    Node *__getMapLayer();
    
};

#endif /* defined(__CreazyBomber__BattleScene__) */
