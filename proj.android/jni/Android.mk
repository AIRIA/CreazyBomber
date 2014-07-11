LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/base/BaseLayer.cpp \
                   ../../Classes/common/VisibleRect.cpp \
                   ../../Classes/components/BomberButton.cpp \
                   ../../Classes/components/DirectionButton.cpp \
                   ../../Classes/components/GameUILayer.cpp \
		   		   ../../Classes/components/PlayerArrow.cpp \
		   		   ../../Classes/components/ResultLayer.cpp \
		   		   ../../Classes/components/SettingLayer.cpp \
                   ../../Classes/game/scenes/HomeScene.cpp \
                   ../../Classes/game/scenes/GameScene.cpp \
                   ../../Classes/game/scenes/TempScene.cpp \
                   ../../Classes/components/PerfectMenu.cpp \
                   ../../Classes/game/objects/MapObject.cpp \
		   		   ../../Classes/game/objects/BigBuilding.cpp \
                   ../../Classes/game/objects/MapCell.cpp \
				   ../../Classes/game/objects/Player.cpp \
				   ../../Classes/game/objects/MonsterSkills.cpp \
				   ../../Classes/game/objects/Bomb.cpp \
		   		   ../../Classes/components/MapLayer.cpp \
                   ../../Classes/game/GameConfig.cpp \
                   ../../Classes/game/MapUtil.cpp \
                   ../../Classes/game/GameManager.cpp \
                   ../../Classes/game/data/PlayerInfoParam.cpp \
                   ../../Classes/game/objects/PlayerItem.cpp \
                   ../../Classes/game/utils/Util.cpp 
                   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,extensions) 
