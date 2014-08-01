#include "AppDelegate.h"
#include "game/scenes/HomeScene.h"
#include "game/scenes/GameScene.h"
#include "game/scenes/WelcomeScene.h"
#include "game/utils/Util.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Crazy Bomber");
        glview->setFrameSize(1280, 800);
        director->setOpenGLView(glview);
    }
    srand(time(NULL));
//    director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    PluginUtil::init();
#endif
    WelcomeScene::create()->run();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
