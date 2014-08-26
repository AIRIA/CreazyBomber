//
//  Util.h
//  CreazyBomber
//
//  Created by AIRIA on 14-6-23.
//
//

#ifndef __CreazyBomber__Util__
#define __CreazyBomber__Util__

#include "common/CommonHeaders.h"

#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

enum MethodType{
    kPPdoSdkShowAds,
    kPPdoSdkHideAds,
    kPPdoSdkShowScoreWall,
    kPPdoSdkLogin,
    kPPdoSdkPay,
    kPPdoSdkToast,
    kPPdoSdkPayConfirm,
    kPPdoSdkAnalyze,
    kPPdoSdkShare,
    kPPdoSdkShowSpotAds,
    kPPdoSdkShowOffersWall
};



class PluginUtil
{
private:
    static JniMethodInfo minfo;
    static jobject obj;
public:
    static void init();
    static void invoke(MethodType key, std::string param="");
    static int getPoint();
};

#endif

enum AnalyzeType{
    kLevelStart = 1,
    kLevelFailed,
    kLevelFinish
};

class Util
{
public:
    /**
     * 显示积分墙
     */
    static void showOffersWall();
    /**
     * 显示插屏广告
     */
    static void showSpotAds();
    static int getPoint();
    static void spendPoint(int point);
    static void rewardPoint(int point);
    static void share();
    static void gameAnalyze(AnalyzeType type);
    static void chargeConfirm();
    static std::string itoa(int i);
    static void charge();
    static void playSound(std::string sound,bool repeat=false);
    static void playEffect(std::string effect,bool repeat=false);
    /**
     * 解析单行动画纹理
     */
    static void addAnimation(std::string fileName,int frameNum,float delay = 0.15f);
    
    static std::vector<std::string> split(std::string _string,std::string delimiter,std::vector<std::string> res);
    
    static void toast(std::string msg);
    static bool adsEnable;
    
};



#endif /* defined(__CreazyBomber__Util__) */
