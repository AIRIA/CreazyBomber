//
//  MapCell.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-26.
//
//

#include "MapCell.h"

CellArg *CellArg::create(std::string type, std::string value)
{
    auto arg = new CellArg();
    arg->autorelease();
    arg->setType(type);
    arg->setValue(value);
    return arg;
}


CellAnimation *CellAnimation::create(int id, int width, int height, int frameNum, float frameTime, float offsetX, float offsetY)
{
    auto animation = new CellAnimation();
    animation->autorelease();
    animation->setID(id);
    animation->setWidth(width);
    animation->setHeight(height);
    animation->setFrameNum(frameNum);
    animation->setFrameTime(frameTime);
    animation->setOffsetX(offsetX);
    animation->setOffsetY(offsetY);
    return animation;
}

MapCell *MapCell::create(std::string cellName,std::string fileName,std::string groupId,int cellType,float anchorX,float anchorY)
{
    auto cell = new MapCell();
    cell->autorelease();
    cell->setCellName(cellName);
    cell->setFileName(fileName);
    cell->setGroupId(groupId);
    cell->setCellType(cellType);
    cell->setAnchorX(anchorX);
    cell->setAnchorY(anchorY);
    return cell;
}
