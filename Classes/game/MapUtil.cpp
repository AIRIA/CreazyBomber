//
//  MapUtil.cpp
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-25.
//
//

#include "MapUtil.h"

#define TILE_WIDTH 80
#define TILE_HEIGHT 80

MapUtil *MapUtil::getInstance()
{
    static MapUtil *instance = nullptr;
    if(instance==nullptr)
    {
        instance = new MapUtil();
    }
    return instance;
}

std::string MapUtil::getMapName()
{
    /* 获取地图名称 */
    char mapName[20];
    int prefix = 1100;
    if (GameConfig::selectedSceneName=="cl") {
        prefix = 1100;
    }else if(GameConfig::selectedSceneName=="bc"){
        prefix = 2100;
    }else{
        prefix = 3100;
    }
    prefix += GameConfig::selectedLevel;
    sprintf(mapName, "%d",prefix);
    return mapName;
}

std::string MapUtil::getTmxMapName()
{
    char mapName[20];
    sprintf(mapName, "map/map%s.tmx",getMapName().c_str());
    return mapName;
}

std::string MapUtil::getSecMapName()
{
    char mapName[20];
    sprintf(mapName, "res/map/map%s.sec",getMapName().c_str());
    return mapName;
}

std::string MapUtil::getBaseTileFullName()
{
    char pathName[30];
    sprintf(pathName, "map/%smap_base-hd.png",GameConfig::selectedSceneName.c_str());
    return pathName;
}

const Size &MapUtil::getMapSize()
{
    auto map = TMXTiledMap::create(getTmxMapName());
    return map->getMapSize();
}

/* 需要根据地图的宽高来显示指定数量的背景图 */
Node *MapUtil::getBaseTileLayer()
{
    auto mapSize = getMapSize();
    auto baseTile = Sprite::create(getBaseTileFullName());
    auto node = Node::create();
    for(auto x=0;x<mapSize.width;x++)
    {
        for(auto y=0;y<mapSize.height;y++)
        {
            auto rect = Rect(TILE_WIDTH*(x%2), TILE_HEIGHT*(y%2), TILE_WIDTH, TILE_HEIGHT);
            auto tile = Sprite::createWithTexture(baseTile->getTexture(), rect);
            tile->setPosition(Point(TILE_WIDTH*x,TILE_HEIGHT*y));
            tile->setAnchorPoint(Point::ZERO);
            node->addChild(tile);
        }
    }
    node->setAnchorPoint(Point(0.5f,0.5f));
    node->setContentSize(Size(TILE_WIDTH*mapSize.width,TILE_HEIGHT*mapSize.height));
    node->setPosition(DESIGN_CENTER);
    return node;
    
}

Node *MapUtil::getTmxTileLayer()
{
    if (m_pMap!=nullptr) {
        return m_pMap;
    }
    
    XMLDocument doc;
    Data data = FileUtils::getInstance()->getDataFromFile(getTmxMapName());
    doc.Parse((const char*)data.getBytes(),data.getSize());
    auto root = doc.RootElement();
    
    root->SetAttribute("tilewidth", TILE_WIDTH);
    root->SetAttribute("tileheight", TILE_HEIGHT);
    auto tileSet = root->FirstChildElement("tileset");
    tileSet->SetAttribute("tilewidth", TILE_WIDTH);
    tileSet->SetAttribute("tileheight", TILE_HEIGHT);
    auto sourceImage = tileSet->FirstChildElement();
    sourceImage->SetAttribute("width", sourceImage->IntAttribute("width")*2);
    sourceImage->SetAttribute("height", sourceImage->IntAttribute("height")*2);
    std::string smallTexture = sourceImage->Attribute("source");
    auto dotPos = smallTexture.find_last_of(".");
    smallTexture.insert(dotPos, "-hd");
    sourceImage->SetAttribute("source", smallTexture.c_str());
    XMLPrinter printer;
    doc.Accept(&printer);
    const char *tmxXml = printer.CStr();
    auto map = TMXTiledMap::createWithXML(tmxXml, "map");
    auto mapSize = map->getMapSize();
    map->setAnchorPoint(Point(0.5f,0.5f));
    map->setPosition(DESIGN_CENTER);
    return map;
}


Node *MapUtil::getCommonTileLayer()
{
    auto map = Node::create();
    XMLDocument mapDoc;
    Data data = FileUtils::getInstance()->getDataFromFile(getSecMapName());
    mapDoc.Parse((const char *)data.getBytes(),data.getSize());
    XMLElement *root = mapDoc.RootElement();
    XMLElement *cell = root->FirstChildElement("Cell");
    while(cell!=nullptr)
    {
        std::string name = cell->Attribute("Name");
        auto col = cell->IntAttribute("X");
        auto row = cell->IntAttribute("Y");
//        log(name.c_str());
        if(name!="出生点")
        {
//            auto tile = __getTileByName(name);
//            tile->setPosition(Point(col*80,DESIGN_HEIGHT-row*80));
//            map->addChild(tile);
        }else{
            log("发现出生点");
        }
        cell = cell->NextSiblingElement();
    }

}

MapObject *MapUtil::getMapObject(std::string obj, int type)
{
    
}