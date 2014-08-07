//
//  Util.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-23.
//
//

#include "Util.h"
#include "game/GameConfig.h"

std::string Util::itoa(int i) {
	char buff[50];
	sprintf(buff, "%d", i);
	return buff;
}

void Util::playSound(std::string sound, bool repeat) {
	if (GameConfig::getInstance()->getSoundEnable()) {
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		SimpleAudioEngine::getInstance()->playBackgroundMusic(sound.c_str(),
				repeat);
	}
}

void Util::playEffect(std::string effect, bool repeat) {
	if (GameConfig::getInstance()->getEffectEnable()) {
		SimpleAudioEngine::getInstance()->playEffect(effect.c_str(), repeat);
	}

}

void Util::addAnimation(std::string fileName, int frameNum, float delay) {
	auto animation = AnimationCache::getInstance()->getAnimation(fileName);
	if (animation) {
		return;
	}
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(
			fileName);
	auto textureRect = frame->getRect();
	auto frameWidth = textureRect.size.width / frameNum;
	auto frameHeight = textureRect.size.height;
	Vector<SpriteFrame*> frameVec;
	for (auto i = 0; i < frameNum; i++) {
		auto rect = Rect(textureRect.origin.x + frameWidth * i,
				textureRect.origin.y, frameWidth, frameHeight);
		auto spriteFrame = SpriteFrame::createWithTexture(frame->getTexture(),
				rect);
		frameVec.pushBack(spriteFrame);
	}
	animation = Animation::createWithSpriteFrames(frameVec);
	animation->setDelayPerUnit(delay);
	AnimationCache::getInstance()->addAnimation(animation, fileName);
}

std::vector<std::string> Util::split(std::string _string, std::string delimiter,
		std::vector<std::string> res) {
	std::string strTmp = _string;
	size_t cutAt;
	while ((cutAt = strTmp.find_first_of(delimiter)) != strTmp.npos) {
		if (cutAt > 0) {
			res.push_back(strTmp.substr(0, cutAt));
		}
		strTmp = strTmp.substr(cutAt + 1);
	}

	if (strTmp.length() > 0) {
		res.push_back(strTmp);
	}

	return res;
}

void Util::toast(std::string msg) {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	PluginUtil::invoke(kPPdoSdkToast,msg);
#else

#endif
}

void Util::charge() {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	PluginUtil::invoke(kPPdoSdkPay,"");
#else

#endif
}
void Util::chargeConfirm() {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	PluginUtil::invoke(kPPdoSdkPayConfirm,"");
#else

#endif
}

void Util::gameAnalyze(AnalyzeType type) {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	auto config = GameConfig::getInstance();
	PluginUtil::invoke(kPPdoSdkAnalyze,__String::createWithFormat("{\"key\":\"%d\",\"value\":\"%s_%d\"}",type,config->getSelectSceneName().c_str(),config->getSelectLevel())->getCString());
#else

#endif
}

void Util::share() {
	auto winSize = Director::getInstance()->getWinSize();
	auto render = RenderTexture::create(winSize.width, winSize.height);
	auto runningScene = Director::getInstance()->getRunningScene();
	render->begin();
	runningScene->visit();
	render->end();
	render->saveToFile("record.jpg", Image::Format::JPG);
	auto shareImgPath = FileUtils::getInstance()->getWritablePath()
			+ "record.jpg";
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	PluginUtil::invoke(kPPdoSdkShare,shareImgPath);
#else

#endif
}

void Util::showSpotAds() {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	PluginUtil::invoke(kPPdoSdkShowSpotAds,"");
#else

#endif
}

void Util::showOffersWall() {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	PluginUtil::invoke(kPPdoSdkShowOffersWall,"");
#else

#endif
}

void Util::spendPoint(int point) {

}

void Util::rewardPoint(int point) {

}

int Util::getPoint() {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	return PluginUtil::getPoint();
#else
    return 0;
#endif
}

#pragma mark----------------------plugin util-------------------------------

#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)

#define PP_SDK_CLASS "com/giant/crazy/jni/JniBrige"
#define PP_SDK_INSTANCE_METHOD "instance"

JniMethodInfo PluginUtil::minfo;
jobject PluginUtil::obj;

void PluginUtil::init() {
	bool isHave = JniHelper::getStaticMethodInfo(minfo, PP_SDK_CLASS,
			PP_SDK_INSTANCE_METHOD, "()Ljava/lang/Object;");
	if (isHave) {
		obj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	} else {
		obj = NULL;
	}
}

int PluginUtil::getPoint() {
    std::string methodName = "doSdkGetPoint";
	bool isExist = JniHelper::getMethodInfo(minfo, PP_SDK_CLASS, methodName.c_str(),
			"()I");
    
	if (isExist == false) {
		CCLog("%s not found", methodName.c_str());
		return -1;
	}
	jint result = minfo.env->CallIntMethod(obj, minfo.methodID);
	return result;
}

void PluginUtil::invoke(MethodType key, std::string param) {
	if (obj == NULL) {
		CCLog("Plugin Object is NULL");
		return;
	}
	std::string methodName;
	switch (key) {
		case kPPdoSdkShowAds:
		methodName = "doSdkShowAds";
		break;
		case kPPdoSdkHideAds:
		methodName = "doSdkHideAds";
		break;
		case kPPdoSdkShowScoreWall:
		methodName = "doSdkShowScoreWall";
		break;
		case kPPdoSdkPay:
		methodName = "doSdkPay";
		break;
		case kPPdoSdkLogin:
		methodName = "doSdkLogin";
		break;
		case kPPdoSdkToast:
		methodName = "doSdkToast";
		break;
		case kPPdoSdkPayConfirm:
		methodName = "doSdkPayConfirm";
		break;
		case kPPdoSdkAnalyze:
		methodName = "doSdkAnalyze";
		break;
		case kPPdoSdkShare:
		methodName = "doSdkShare";
		break;
		case kPPdoSdkShowSpotAds:
		methodName = "doSdkShowSpotAds";
		break;
		case kPPdoSdkShowOffersWall:
		methodName = "doSdkShowOffersWall";
		break;
		default:
		break;
	}
	bool isExist = JniHelper::getMethodInfo(minfo, PP_SDK_CLASS, methodName.c_str(),
			"(Ljava/lang/String;)V");
	if (isExist == false) {
		CCLog("%s not found", methodName.c_str());
		return;
	}
	jstring paramStr = minfo.env->NewStringUTF(param.c_str());
	minfo.env->CallVoidMethod(obj, minfo.methodID, paramStr);
}

extern "C"
{
	JNIEXPORT void JNICALL Java_com_giant_crazy_jni_JniBrige_payHandler(JNIEnv *env,jobject thiz)
	{
		__userDefault->setIntegerForKey(KEY_COIN_NUM, __userDefault->getIntegerForKey(KEY_COIN_NUM)+1000);
//		auto msg = __String::createWithFormat("",__userDefault->getIntegerForKey(KEY_COIN_NUM))->getCString();
        NotificationCenter::getInstance()->postNotification(UPDATE_PLAER_COIN);
        NotificationCenter::getInstance()->postNotification(BUY_COIN_SUCCESS);
		Util::toast("金币兑换成功");
	}

}

#endif

