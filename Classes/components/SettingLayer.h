//
//  SettingLayer.h
//  CreazyBomber
//
//  Created by AIRIA on 14-6-20.
//
//

#ifndef __CreazyBomber__SettingLayer__
#define __CreazyBomber__SettingLayer__

#include "common/CommonHeaders.h"

class SettingLayer:public Layer
{
public:
    static SettingLayer *getInstance();
    virtual bool init();
    CREATE_FUNC(SettingLayer);
    virtual void onEnter();
    virtual void onExit();
    void show();
    void showPause(Ref *pSender);
    void showSetting(Ref *pSender);
protected:
    void addBgColor();
    Sprite *background;
};

#endif /* defined(__CreazyBomber__SettingLayer__) */
