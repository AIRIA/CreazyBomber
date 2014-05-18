//
//  BaseLayer.h
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-18.
//
//

#ifndef __CreazyBomber__BaseLayer__
#define __CreazyBomber__BaseLayer__

#include "common/CommonHeaders.h"

/**
 1.屏幕适配 分为三个部分 左 中 右
 **/
class BaseLayer:public Layer
{
public:
    BaseLayer():m_fScaleFactor(0.0f),m_pLeft(nullptr),m_pRight(nullptr),m_pBody(nullptr){};
    /* run self scene */
    void run();
    /* invoke on asset load complete */
    virtual void onTexturesLoaded();
    virtual bool init();
    CREATE_FUNC(BaseLayer);
    virtual void onEnter();
    virtual void onExit();
protected:
    float m_fScaleFactor;
    Size m_winSize;
    /* three part for scale game scene */
    Layer *m_pLeft,*m_pBody,*m_pRight;
    /* textures that will be preload */
    std::vector<std::string> textureFiles;
private:
    Layer *__createScaleLayer(const Point &anchorPoint,const Point &position);
    void __loadAssets();
    static void __loadedHandler(Texture2D *texture);
    int loadedNum = 0;
    void __loadedNotificationHander(Ref *pObj);
};

#endif /* defined(__CreazyBomber__BaseLayer__) */
