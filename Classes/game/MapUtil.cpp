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

enum CellType{
    kCellTypeBornPlace    = 88,//英雄出生的地方
    kCellTypeMonster      = 100,//怪物
    kCellTypeGround       = 3,//普通的tile不能穿越 不能被炸坏
    kCellTypeTransfer     = 36,//传送门
    kCellTypeBigBuilding  = 51,//大型建筑
    kCellTypePlant        = 5,//可以晃动的植物
    kCellTypeBox          = 9,// 木头箱子 可以推动的
    kCellTypeCorpseFlower = 7,//食人花
    kCellTypeWanDou       = 55 ,//豌豆 可以发射炮弹
    kCellTypeDiCi         = 40,//地刺
    kCellTypeLvDai        = 8,//履带 可以让英雄加速或者减速
    kCellTypeShuShou      = 38 ,//瘴气
    kCellTypeMonsterHome  = 11,//怪物帐篷 可以生成怪物
    kCellTypeDiDong       = 44,//地洞 可以生成怪物
    kCellTypeFireWall     = 16,//火墙 可以产生火球
    kCellTypePengHuoKou   = 15,//喷火口 可以喷火
    kCellTypeEvilFire     = 4,// 鬼火 可以移动
    kCellTypeWom          = 42,//虫子
    kCellTypeSnowBall     = 59,//雪球 可以滚动
    kCellTypeIce          = 12,//冰面
};

MapUtil *MapUtil::getInstance()
{
    static MapUtil *instance = nullptr;
    if(instance==nullptr)
    {
        instance = new MapUtil();
        instance->initMapCells();
    }
    return instance;
}

void MapUtil::initMapCells()
{
    XMLDocument doc;
    Data data = FileUtils::getInstance()->getDataFromFile("res/scenetex.cfg");
    doc.Parse((const char*)data.getBytes(),data.getSize());
    
    auto root = doc.RootElement();
    auto cell = root->FirstChildElement("MapCellSprite");
    while (cell!=nullptr) {
        std::string cellName = cell->Attribute("Name");
        std::string fileName = cell->Attribute("FileName");
        std::string groupId = cell->Attribute("GroupID");
        int type = cell->IntAttribute("Type");
        float anchorX = cell->FloatAttribute("AnchorX");
        float anchorY = cell->FloatAttribute("AnchorY");
        auto mapCell = MapCell::create(cellName, fileName, groupId, type,anchorX,anchorY);
        m_vMapCells.pushBack(mapCell);
        /* 解析Animations */
        auto animations = cell->FirstChildElement("Animations");
        auto animation = animations->FirstChildElement("Animation");
        while (animation!=nullptr) {
            auto id = animation->IntAttribute("ID");
            auto width = animation->IntAttribute("Width");
            auto height = animation->IntAttribute("Height");
            auto frameNum = animation->IntAttribute("FrameNum");
            auto frameTime = animation->FloatAttribute("FrameTime");
            auto offsetX = animation->FloatAttribute("offsetX");
            auto offsetY = animation->FloatAttribute("offsetY");
            auto cellAnimation = CellAnimation::create(id, width, height, frameNum, frameTime, offsetX, offsetY);
            mapCell->getAnimations().pushBack(cellAnimation);
            animation = animation->NextSiblingElement();
        }
        /* 解析Args */
        auto args = cell->FirstChildElement("Args");
        auto arg = args->FirstChildElement("Arg");
        while (arg!=nullptr) {
            auto type = arg->Attribute("Type");
            auto val = arg->Attribute("DefaultValue");
            if(val==nullptr)
            {
                val = "";
            }
            auto cellArg = CellArg::create(type, val);
            mapCell->getArgs().pushBack(cellArg);
            arg = arg->NextSiblingElement();
        }
        cell = cell->NextSiblingElement();
    }

    
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

std::string MapUtil::getSceMapName()
{
    char mapName[20];
    sprintf(mapName, "res/map/map%s.sce",getMapName().c_str());
    return mapName;
}

std::string MapUtil::getBaseTileFullName()
{
    char pathName[30];
    sprintf(pathName, "map/%smap_base-hd.png",GameConfig::selectedSceneName.c_str());
    return pathName;
}

const Size MapUtil::getMapSizeInPixle()
{
    auto mapSize = getMapSize();
    auto sizeInPixle = Size(mapSize.width*TILE_WIDTH,mapSize.height*TILE_HEIGHT);
    return sizeInPixle;
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
    node->setContentSize(Size(TILE_WIDTH*mapSize.width,TILE_HEIGHT*mapSize.height));
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
    map->setAnchorPoint(Point::ZERO);
    map->setPosition(Point::ZERO);
    return map;
}


Node *MapUtil::getCommonTileLayer()
{
    auto mapCellNode = Node::create();
    XMLDocument mapDoc;
    Data data = FileUtils::getInstance()->getDataFromFile(getSceMapName());
    mapDoc.Parse((const char *)data.getBytes(),data.getSize());
    XMLElement *root = mapDoc.RootElement();
    XMLElement *cell = root->FirstChildElement("Cell");
    while(cell!=nullptr)
    {
        std::string name = cell->Attribute("Name");
        auto col = cell->IntAttribute("X");
        auto row = cell->IntAttribute("Y");
        cell = cell->NextSiblingElement();
        
        auto mapObj = getMapObject(name);
        if(mapObj==nullptr)
        {
            continue;
        }
        mapObj->setRow(row);
        mapObj->setCol(col);
        mapCellNode->addChild(mapObj);
    }
    return mapCellNode;

}

MapObject *MapUtil::getMapObject(std::string name)
{
    log("%s",name.c_str());
    auto it = m_vMapCells.begin();
    while (it!=m_vMapCells.end()) {
        if((*it)->getCellName()==name)
        {
            break;
        }
        it++;
    }
    
    auto mapCell = *it;
    MapObject *element = nullptr;
    switch (mapCell->getCellType()) {
        case kCellTypeMonster:
            element = Monster::create(mapCell->getFileName());
            break;
        case kCellTypeGround:
            element = GroundTile::create(mapCell->getFileName());
            break;
        case kCellTypeBigBuilding:
            element = BigBuilding::create(mapCell);
            break;
        default:
            break;
    }
    if(element)
    {
        element->setMapCell(mapCell);
    }
    return element;
}


Node *MapUtil::addTileMapBorder()
{
    log("%s","invoke add tileMapBorder");
    auto borderNode = Node::create();
    auto mapSizeInPixle = getMapSizeInPixle();
    borderNode->setContentSize(mapSizeInPixle);
    char dibian_xia[20];
    char dibian_shang[20];
    sprintf(dibian_xia, "%s_dibian_xia.png",GameConfig::selectedSceneName.c_str());
    sprintf(dibian_shang, "%s_dibian_shang.png",GameConfig::selectedSceneName.c_str());
    auto mapSize = getMapSize();
    int endCol = mapSize.width/3;
    if(int(mapSize.width)%3!=0)
    {
        endCol += 1;
    }
    endCol = endCol * 3;
        
    int endRow = mapSize.height/3;
    if(int(mapSize.height)%3!=0)
    {
        endRow += 1;
    }
    endRow = endRow*3;
    
    auto createDibian = [&borderNode](std::string name,Point startPos,int direction)->void{
        auto dibian = Sprite::createWithSpriteFrameName(name);
        auto size = dibian->getContentSize();
        if (direction==0) {
            //横向
            dibian->setAnchorPoint(Point::ZERO);
        }else{
            //纵向
            dibian->setAnchorPoint(Point(1,1));
            dibian->setRotation(90);
        }
        dibian->setPosition(startPos);
        borderNode->addChild(dibian);
    };
    
    for(auto col=0;col<endCol;col+=3)
    {
        createDibian(dibian_shang,Point(TILE_WIDTH*col,mapSizeInPixle.height-TILE_HEIGHT),0);
        createDibian(dibian_xia,Point(TILE_WIDTH*col,0),0);
    }
    
    for (auto row=0; row<endRow; row+=3) {
        createDibian(dibian_xia,Point(TILE_WIDTH,TILE_HEIGHT*row),1);
        createDibian(dibian_shang,Point(mapSizeInPixle.width,TILE_HEIGHT*row),1);
    }
    return borderNode;
}