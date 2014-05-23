//
//  BattleScene.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-21.
//
//

#include "BattleScene.h"

#define TILE_WIDTH 80
#define TILE_HEIGHT 80


bool BattleScene::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    Texture2D::PVRImagesHavePremultipliedAlpha(true);
    textureFiles.push_back("textures/monster");
    textureFiles.push_back("textures/scenetex_small-hd");
    textureFiles.push_back("textures/scenetex_medium");
    textureFiles.push_back("textures/scenetex_big");
    return true;
}

void BattleScene::onTexturesLoaded()
{
    
    Data data = FileUtils::getInstance()->getDataFromFile("map/map1101.tmx");
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
    for(auto x=0;x<mapSize.width;x++)
    {
        for(auto y=0;y<mapSize.height;y++)
        {
            auto tilePos = Point(x,y);
            auto layer1 = map->getLayer("块层 1");
            auto layer2 = map->getLayer("块层 2");
            layer1->getTileSet();
            auto gid = layer1->getTileGIDAt(tilePos);
            auto rect = layer1->getTileSet()->getRectForGID(gid);
            log("rect x:%f,y:%f,width:%f,height:%f",rect.origin.x,rect.origin.y,rect.size.width,rect.size.height);
        }
    }
    
    auto backgroundNode = __initBaseTile(mapSize.width, mapSize.height);
    m_pBody->addChild(backgroundNode);
    m_pBody->addChild(map);
    __initTileSet();
    m_pBody->addChild(__getMapLayer());
    
    
}

Node *BattleScene::__initBaseTile(int width,int height)
{
    auto baseTile = Sprite::create("map/clmap_base-hd.png");
    auto node = Node::create();
    for(auto x=0;x<width;x++)
    {
        for(auto y=0;y<height;y++)
        {
            auto rect = Rect(TILE_WIDTH*(x%2), TILE_HEIGHT*(y%2), TILE_WIDTH, TILE_HEIGHT);
            auto tile = Sprite::createWithTexture(baseTile->getTexture(), rect);
            tile->setPosition(Point(TILE_WIDTH*x,TILE_HEIGHT*y));
            node->addChild(tile);
        }
    }
    //node->setContentSize(Size(TILE_WIDTH*width,TILE_HEIGHT*height));
   // node->setAnchorPoint(Point(0.5f,0.5f));
    return node;
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
            tile->setType(type);
            tile->setAnchorPoint(Point(anchorX,1-anchorY));
            tile->getTexture()->setAliasTexParameters();
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
        auto col = cell->IntAttribute("X");
        auto row = cell->IntAttribute("Y");
        if(name!="出生点")
        {
            auto tile = __getTileByName(name);
            tile->setPosition(Point(col*80,DESIGN_HEIGHT-row*80));
            map->addChild(tile);
        }else{
            log("发现出生点");
        }
        cell = cell->NextSiblingElement();
    }
    map->setContentSize(Size(TILE_WIDTH*12,TILE_HEIGHT*8));
    map->setAnchorPoint(Point(0.5f,0.5f));
    map->setPosition(DESIGN_CENTER);
    return map;
}



