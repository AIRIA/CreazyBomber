//
//  CommonHeaders.h
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-18.
//
//

#ifndef CreazyBomber_CommonHeaders_h
#define CreazyBomber_CommonHeaders_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "VisibleRect.h"



using namespace cocos2d;
using namespace cocos2d::extension;
using namespace CocosDenshion;


#include "external/tinyxml2/tinyxml2.h"
#include "external/json/rapidjson.h"
#include "external/json/document.h"
#include "game/utils/Util.h"
#include "game/GameConfig.h"

using namespace tinyxml2;
using namespace rapidjson;

enum WalkDirection{
    kWalkUp = 1,
    kWalkLeft = 2,
    kWalkRight = -2,
    kWalkDown  = -1,
    kWalkStand = 0
};

#define DESIGN_WIDTH 960
#define DESIGN_HEIGHT 640

#define DESIGN_CENTER Point(DESIGN_WIDTH/2,DESIGN_HEIGHT/2)
#define DESIGN_TOP Point(DESIGN_WIDTH/2,DESIGN_HEIGHT)
#define DESIGN_RIGHT_BOTTOM Point(DESIGN_WIDTH,0)

#define SPRITE(name) Sprite::createWithSpriteFrameName(name)

//--------Notifications---------------------------------------------
#define ADD_NORMAL_BOMB      "add_normal_bomb"
#define ADD_TIMER_BOMB       "add_timer_bomb"
#define GAME_PASS            "game_pass"
#define GAME_OVER            "game_over"
#define GAME_RESULT          "game_result"
#define GAME_NEXT            "game_next"
#define GAME_RETRY           "game_retry"
#define GAME_EXIT            "game_exit"

#define UPDATE_HP            "update_hp"
#define MONSTER_DESTROY      "monster_destroy"
#define SHOW_PAUSE           "show_pause"
#define SHOW_SETTING         "show_setting"
#define UPDATE_PLAYER_INFO   "update_player_info"
#define CREATE_PLAYER_ITEM   "create_player_item"  //在木头或者植物被炸掉以后 添加

#define UPDATE_MONSTER_COUNT "update_monster_count"
#define SHOW_BOSS_HP         "show_boss_hp"
#define UPDATE_BOSS_HP       "update_boss_hp"
#define BOSS_DEAD "boss_dead"

//----------sounds---------------

#define SOUND_ITEM_GET_BIG_COIN    "music/soundEffect/item_get_big_coin.mp3"
#define SOUND_ITEM_GET_BOMB        "music/soundEffect/item_get_bombnum.mp3"
#define SOUND_ITEM_GET_LITTLE_COIN "music/soundEffect/item_get_little_coin.mp3"
#define SOUND_ITEM_GET_POWER       "music/soundEffect/item_get_power.mp3"
#define SOUND_ITEM_GET_SHOE        "music/soundEffect/item_get_shoe.mp3"
#define SOUND_ITEM_PUT_BOMB        "music/soundEffect/item_put_bomb.mp3"
#define SOUND_ITEM_USE_HP          "music/soundEffect/item_use_hp.mp3"
#define SOUND_MONSTER_BOMBED_DEAD  "music/soundEffect/monster_bombed_dead.mp3"
#define SOUND_PLAYER_LOW_HP        "music/soundEffect/player_low_hp.mp3"
#define SOUND_INGAME_WIN           "music/soundEffect/ingame_win.mp3"
#define SOUND_INGAME_FAILED        "music/soundEffect/ingame_fail.mp3"
#define SOUND_ITEM_BOMB_EXPLODE    "music/soundEffect/item_bomb_explode.mp3"
#define SOUND_ENTER_TONGGUANDIAN   "music/soundEffect/playe_enter_tongguandian.mp3"
#define SOUND_INGAME_START         "music/soundEffect/ingame_game_start.mp3"
#define SOUND_INGAME_COUNT_NUMBER  "music/soundEffect/ingame_count_number.mp3"

#define SOUND_PLAYER_ATTACKED __String::createWithFormat("music/soundEffect/player_%s_attacked.mp3",GameConfig::getInstance()->getSelectRoleName().c_str())->getCString()
#define SOUND_PLAYER_DEATH    __String::createWithFormat("music/soundEffect/player_%s_death.mp3",GameConfig::getInstance()->getSelectRoleName().c_str())->getCString()
#define SOUND_PLAYER_BIRTH    __String::createWithFormat("music/soundEffect/player_%s_birth.mp3",GameConfig::getInstance()->getSelectRoleName().c_str())->getCString()
#define SOUND_PLAYER_RANDOM   __String::createWithFormat("music/soundEffect/player_%s_random.mp3",GameConfig::getInstance()->getSelectRoleName().c_str())->getCString()
#define SOUND_SCENE_BG        __String::createWithFormat("music/bg/music_game_%s.mp3",GameConfig::getInstance()->getSelectSceneName().c_str())->getCString()

#pragma mark ----------------UserDefault Defines---------------------------------------------------------------------------------------------------

#define KEY_CL "key_cl"
#define KEY_BC "key_bc"
#define KEY_MD "key_md"

#define KEY_COIN_NUM "key_coin_num"

#define KEY_CL_BATTLE "key_cl_battle"
#define KEY_BC_BATTLE "key_bc_battle"
#define KEY_MD_BATTLE "key_md_battle"

#define KEY_CL_LEVEL "key_cl_level"
#define KEY_BC_LEVEL "key_bc_level"
#define KEY_MD_LEVEL "key_md_level"


#define KEY_SOUND_ENABLED "key_sound_enabled"
#define KEY_EFFECT_ENABLED "key_effect_enabled"

#define KEY_BATTLE_MODE "key_battle_mode"

#define __userDefault UserDefault::getInstance()

#endif
