//
//  MapCell.h
//  CreazyBomber
//
//  Created by AIRIA on 14-5-26.
//
//

#ifndef __CreazyBomber__MapCell__
#define __CreazyBomber__MapCell__

#include "common/CommonHeaders.h"

class CellArg:public Ref
{
public:
    static CellArg *create(std::string type,std::string value);
    CC_SYNTHESIZE(std::string , m_sType, Type);
    CC_SYNTHESIZE(std::string , m_sValue, Value);
};

class CellAnimation:public Ref
{
public:
    static CellAnimation *create(std::string id,int width,int height,int frameNum,float frameTime,float offsetX,float offsetY);
    CC_SYNTHESIZE(std::string, m_sID, ID);
    CC_SYNTHESIZE(int, m_iWidth, Width);
    CC_SYNTHESIZE(int, m_iHeight, Height);
    CC_SYNTHESIZE(int, m_iFrameNum, FrameNum);
    CC_SYNTHESIZE(float ,m_fFrameTime,FrameTime);
    CC_SYNTHESIZE(float, m_fOffsetX, OffsetX);
    CC_SYNTHESIZE(float, m_fOffsetY, OffsetY);
};

class MapCell:public Ref
{
public:
    static MapCell *create(std::string cellName,std::string fileName,std::string groupId,int cellType,float anchorX,float anchorY);
    CC_SYNTHESIZE(Vector<CellArg*>, m_vArgs, Args);
    CC_SYNTHESIZE(Vector<CellAnimation*>, m_vAnimations, Animations);
    CC_SYNTHESIZE(std::string, m_sFileName, FileName);
    CC_SYNTHESIZE(std::string, m_sCellName, CellName);
    CC_SYNTHESIZE(std::string, m_sGroupId, GroupId);
    CC_SYNTHESIZE(int, m_iCellType, CellType);
    CC_SYNTHESIZE(float, m_fAnchorX, AnchorX);
    CC_SYNTHESIZE(float, m_fAnchorY, AnchorY);
};


#endif /* defined(__CreazyBomber__MapCell__) */
