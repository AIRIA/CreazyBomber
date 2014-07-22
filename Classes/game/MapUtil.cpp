//
//  MapUtil.cpp
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-25.
//
//

#include "MapUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TILE_WIDTH 80
#define TILE_HEIGHT 80

MapUtil *MapUtil::getInstance()
{
    static MapUtil *instance = nullptr;
    if(instance==nullptr)
    {
        instance = new MapUtil();
    }
    if(instance->getMapCells().size()==0)
    {
        instance->initMapCells();
        instance->getMonsterInfos();
        instance->readMapTxt();
    }
    return instance;
}

void MapUtil::dispose()
{
    m_vMonsters.clear();
    m_vBombs.clear();
    m_vCommonTiles.clear();
    m_vFires.clear();
    m_vMonsterProperteis.clear();
    m_vMapCells.clear();
    m_vMapObjects.clear();
    m_vMapBorders.clear();
    mapVec.clear();
    m_vLvDai.clear();
}

void MapUtil::readMapTxt()
{
    auto data = FileUtils::getInstance()->getStringFromFile("res/map.txt");
    std::string line;
    std::stringstream ss(data);
    char del = '\n';
    while (std::getline(ss, line, del)) {
        std::vector<std::string> elements;
        elements = Util::split(line, "\t", elements);
        mapVec.push_back(elements);
    }
}

void MapUtil::clearMap()
{
    m_vFires.clear();
    m_vMapCells.clear();
    m_vMonsters.clear();
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
        
        /* 解析Animations */
        auto animations = cell->FirstChildElement("Animations");
        auto animation = animations->FirstChildElement("Animation");
        Vector<CellAnimation*> cellAnimationVec;
        while (animation!=nullptr) {
            auto id = animation->Attribute("ID");
            auto width = animation->IntAttribute("Width");
            auto height = animation->IntAttribute("Height");
            auto frameNum = animation->IntAttribute("FrameNum");
            auto frameTime = animation->FloatAttribute("FrameTime");
            auto offsetX = animation->FloatAttribute("OffsetX");
            auto offsetY = animation->FloatAttribute("OffsetY");
            auto cellAnimation = CellAnimation::create(id, width, height, frameNum, frameTime, offsetX, offsetY);
            cellAnimationVec.pushBack(cellAnimation);
            animation = animation->NextSiblingElement();
        }
        
        /* 解析Args */
        auto args = cell->FirstChildElement("Args");
        auto arg = args->FirstChildElement("Arg");
        Vector<CellArg*> cellArgVec;
        while (arg!=nullptr) {
            auto type = arg->Attribute("Type");
            auto val = arg->Attribute("DefaultValue");
            if(val==nullptr)
            {
                val = "";
            }
            auto cellArg = CellArg::create(type, val);
            cellArgVec.pushBack(cellArg);
            arg = arg->NextSiblingElement();
        }
        
        mapCell->setArgs(cellArgVec);
        mapCell->setAnimations(cellAnimationVec);
        m_vMapCells.pushBack(mapCell);
        cell = cell->NextSiblingElement();
    }
}


std::string MapUtil::getMapName()
{
    /* 获取地图名称 */
    char mapName[20];
    int prefix = 1100;
    auto config = GameConfig::getInstance();
    auto name = config->getSelectSceneName();
    if (name=="cl") {
        prefix = 1100;
    }else if(name=="md"){
        prefix = 1200;
    }else if(name=="bc"){
        prefix = 1300;
    }else if(name=="cl_battle"){
        prefix = 2100;
    }else if(name=="md_battle"){
        prefix = 2200;
    }else if(name=="bc_battle"){
        prefix = 2300;
    }
    prefix += GameConfig::getInstance()->getSelectLevel();
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
    sprintf(pathName, "map/%smap_base-hd.png",GameConfig::getInstance()->getSelectSceneName().substr(0,2).c_str());
    return pathName;
}

const Size MapUtil::getMapSizeInPixle()
{
    auto mapSize = getMapSize();
    auto sizeInPixle = Size(mapSize.width*TILE_WIDTH,mapSize.height*TILE_HEIGHT);
    setMapWidthInPixle(sizeInPixle.width);
    setMapHeightInPixle(sizeInPixle.height);
    return sizeInPixle;
}

void MapUtil::initMapSize()
{
    auto map = TMXTiledMap::create(getTmxMapName());
    setMapSize(map->getMapSize());
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
            log("get %s mapObj failed ",name.c_str());
            continue;
        }
        if (mapObj->getType()==kCellTypeEvilFire) {
            continue;
        }
        mapObj->setRow(row);
        mapObj->setCol(col);
        mapCellNode->addChild(mapObj);
        if(mapObj->getType()==kCellTypeBigBuilding)
        {
            auto mapCell = mapObj->getMapCell();
            auto width = atoi(mapCell->getArgs().at(1)->getValue().c_str());
            auto height = atoi(mapCell->getArgs().at(1)->getValue().c_str());
            auto collision = mapCell->getArgs().at(2);
            auto val = collision->getValue();
            
            int ax = width*mapObj->getAnchorPoint().x;
            int startCol = mapObj->getCol()-ax;
            
            auto start = atoi(val.substr(0,1).c_str());
            auto end = atoi(val.substr(2,1).c_str());
            
            for (auto i=0;i<height; i++) {
                for (auto j=start; j<=end; j++) {
                    auto emptyObj = EmptyObject::createWithSpriteFrameName("cl_shui13.png");
                    emptyObj->setCol(startCol+j);
                    emptyObj->setRow(mapObj->getRow()-i);
                    mapCellNode->addChild(emptyObj);
                }
            }
        }
        if(mapObj->getType()==kCellTypeFireWall)
        {
            mapObj->setCol(0);
            for(auto i=mapObj->getRow()-1;i>0;i--)
            {
                auto fireWall = FireWall::create(mapObj->getMapCell());
                fireWall->setRow(i);
                fireWall->setCol(0);
                mapCellNode->addChild(fireWall);
            }
        }
    }
    return mapCellNode;
}

MapObject *MapUtil::getMapObject(std::string name)
{
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
        case kCellTypeBornPlace:
            element = Player::create(mapCell);
            break;
        case kCellTypeMonster:
            element = Monster::create(mapCell);
            break;
//        case kCellTypeGround:
//            element = GroundTile::create(mapCell);
//            break;
        case kCellTypeBigBuilding:
            element = BigBuilding::create(mapCell);
            break;
        case kCellTypePlant:
            element = CommonTile::create(mapCell);
            m_vCommonTiles.pushBack(element);
            break;
        case kCellTypeCorpseFlower:
            element = ManEater::create(mapCell);
            m_vCommonTiles.pushBack(element);
            break;
        case kCellTypeBox:
            element = WoodBox::create(mapCell);
            m_vCommonTiles.pushBack(element);
            break;
//        case kCellTypeWanDou:
//            element = GroundTile::create(mapCell);
//            break;
        case kCellTypeDiCi:
            element = DiCi::create(mapCell);
            break;
        case kCellTypeLvDai:
            element = LvDai::create(mapCell);
            m_vLvDai.pushBack(static_cast<LvDai*>(element));
            break;
        case kCellTypeShuShou:
            element = ShuShou::create(mapCell);
            break;
        case kCellTypeMonsterHome:
            element = MonsterHome::create(mapCell);
            m_vCommonTiles.pushBack(element);
            break;
        case kCellTypeDiDong:
            element = DiDong::create(mapCell);
            m_vCommonTiles.pushBack(element);
            break;
        case kCellTypeFireWall:
            element = FireWall::create(mapCell);
            break;
        case kCellTypePengHuoKou:
            element = SnowBallMan::create(mapCell);
            m_vCommonTiles.pushBack(element);
            break;
        case kCellTypeEvilFire:
            element = GuiHuo::create(mapCell);
            break;
        case kCellTypeWom:
            element = SnowBallOrWorm::create(mapCell);
            break;
        case kCellTypeSnowBall:
            element = SnowBallMan::create(mapCell);
            m_vCommonTiles.pushBack(element);
            break;
        case kCellTypeIce:
            element = GroundTile::create(mapCell);
            break;
        case kCellTypeTransfer:
            element = TransferDoor::create(mapCell);
            break;
        default:
            element = GroundTile::create(mapCell);
            m_vCommonTiles.pushBack(element);
            break;
    }
    element->setType(mapCell->getCellType());
    return element;
}


Node *MapUtil::addTileMapBorder()
{
    auto borderNode = Node::create();
    auto mapSizeInPixle = getMapSizeInPixle();
    borderNode->setContentSize(mapSizeInPixle);
    char dibian_xia[20];
    char dibian_shang[20];
    auto scenename = GameConfig::getInstance()->getSelectSceneName().substr(0,2).c_str();
    sprintf(dibian_xia, "%s_dibian_xia.png",scenename);
    sprintf(dibian_shang, "%s_dibian_shang.png",scenename);
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
    
    auto createDibian = [&](std::string name,Point startPos,int direction)->void{
        auto dibian = MapBorder::createWithSpriteFrameName(name);
        MapUtil::getInstance()->getMapBorders().pushBack(dibian);
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

Monster *MapUtil::getMonsterByCoordinate(const cocos2d::Point &coordinate)
{
    auto it = m_vMonsters.begin();
    while(it!=m_vMonsters.end())
    {
        auto obj = *it;
        if(obj->getRow()==coordinate.y && obj->getCol()==coordinate.x)
        {
            return obj;
        }
        it++;
    }
    return nullptr;
}

void MapUtil::removeMapObject(MapObject *mapObj)
{
    m_vMapObjects.eraseObject(mapObj);
}

void MapUtil::getMonsterInfos()
{
    auto data = FileUtils::getInstance()->getStringFromFile("res/monsters.bin");
    rapidjson::Document monstersInfo;
    if(monstersInfo.Parse<0>(data.c_str()).HasParseError()==false)
    {
        const rapidjson::Value &monsters = monstersInfo["monsters"];
        for(rapidjson::SizeType i=0;i<monsters.Size();i++)
        {
            const rapidjson::Value &monster = monsters[i];
            auto mp = MonsterProperty::create();
            mp->setID(monster["id"].GetInt());
            mp->setMonsterName(monster["name"].GetString());
            mp->setHP(monster["hp"].GetInt());
//            mp->setAi(monster["ai"].GetInt());
            mp->setWuDi(monster["wudi"].GetInt());
            mp->setPower(monster["attack"].GetInt());
            mp->setShiYe(monster["shiye"].GetInt());
            mp->setZhuiZong(monster["zhuizong"].GetInt());
            mp->setSpeed(monster["speed"].GetInt());
            mp->setSkills(monster["skills"].GetString());
            mp->setWidth(monster["width"].GetInt());
            mp->setHeight(monster["height"].GetInt());
            mp->setFootPos(monster["foot_pos"].GetInt());
            mp->setReward(monster["reward"].GetInt());
            m_vMonsterProperteis.pushBack(mp);
        }
    }
}

MonsterProperty *MapUtil::getMonsterProperyById(int id)
{
    auto it = getMonsterProperteis().begin();
    while (it!=getMonsterProperteis().end()) {
        auto pro = *it;
        if(pro->getID()==id)
        {
            return pro;
        }
        it++;
    }
    return nullptr;
}

bool MapUtil::isBorder(const cocos2d::Point &coordinate)
{
    if(coordinate.x==0||coordinate.x==MapUtil::getInstance()->getMapSize().width-1||coordinate.y==0||coordinate.y==MapUtil::getInstance()->getMapSize().height-1)
        return true;
    return false;
}









