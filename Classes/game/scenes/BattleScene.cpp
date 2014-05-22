//
//  BattleScene.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-21.
//
//

#include "BattleScene.h"


bool BattleScene::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    textureFiles.push_back("textures/monster");
    textureFiles.push_back("textures/scenetex_small");
    textureFiles.push_back("textures/scenetex_medium");
    return true;
}

void BattleScene::onTexturesLoaded()
{
    __initTileSet();
    m_pBody->addChild(__getMapLayer());
}

void BattleScene::__initTileSet()
{
    Data data = FileUtils::getInstance()->getDataFromFile("res/scenetex.cfg");
    doc.Parse((const char*)data.getBytes(),data.getSize());
}

MapTile *BattleScene::__getTileByName(const std::string &name)
{
    auto root = doc.RootElement();
    auto cell = root->FirstChildElement("MapCellSprite");
    while (cell!=nullptr) {
        std::string cellName = cell->Attribute("Name");
        if(cellName == name){
            std::string fileName = cell->Attribute("FileName");
            float anchorX = cell->FloatAttribute("AnchorX");
            float anchorY = cell->FloatAttribute("AnchorY");
            int type = cell->IntAttribute("Type");
            auto tile = MapTile::createWithSpriteFrameName(fileName);
          //  tile->setType(type);
            tile->setAnchorPoint(Point(anchorX,1-anchorY));
            return tile;
        }
        cell = cell->NextSiblingElement();
    }
    return nullptr;
}

Node *BattleScene::__getMapLayer()
{
    GameConfig::selectedStageName = "1101";
    char mapInfoFilePath[100];
    sprintf(mapInfoFilePath, "res/map/map%s.sce",GameConfig::selectedStageName.c_str());
    auto map = Node::create();
    XMLDocument mapDoc;
    Data data = FileUtils::getInstance()->getDataFromFile(mapInfoFilePath);
    mapDoc.Parse((const char *)data.getBytes(),data.getSize());
    XMLElement *root = mapDoc.RootElement();
    XMLElement *cell = root->FirstChildElement("Cell");
    while(cell!=nullptr)
    {
        std::string name = cell->Attribute("Name");
        log(name.c_str());
        auto col = cell->IntAttribute("X");
        auto row = cell->IntAttribute("Y");
        if(name!="出生点")
        {
            auto tile = __getTileByName(name);
            tile->setPosition(Point(col*80,DESIGN_HEIGHT-row*80));
            map->addChild(tile);
            tile->getTexture()->setAntiAliasTexParameters();
        }else{
            log("发现出生点");
        }
        cell = cell->NextSiblingElement();
    }
    
    return map;
}



