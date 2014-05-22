module (..., package.seeall)

item_timer_bomb_id = 106
item_hp_id = 101

local constant=
{
    player_base_speed=115,
    speed_per_shoe=15,
    shop_item_timerbomb_price = 50,
    shop_item_hp_price = 50,
    revive_cost = 300,
    revive_hidden_time = 2,
    --kill_monster_score=30,
    destroy_building_score=10,
    obtain_item_score=20,
    lineai_monster_idle_time = 0.5,
    random_gold_precent=0.1,
    obtain_item_max_scale = 1.5,
    obtain_gold_max_scale = 2,
    revive_first_guide_time=0.5,
    revive_first_guide_max_scale=1.5,
    revive_first_guide_max_opacity= 90,
}

function GetConstant(name)
    return constant[name]
end


local pos=
{
		game_ui_role_info_bg = {x=50,y=20},
		game_ui_role_info_speed = {x=77,y=27},
		game_ui_role_info_bombpower = {x=117,y=27},
		game_ui_role_info_bombnum = {x=157,y=27},
		game_ui_role_info_coins = {x=197,y=27},
		game_ui_boss_portrait = {x=370,y=295},
		game_ui_moster_num_icon = {x=300,y=295},				
		game_ui_moster_num = {x=350,y=295},		
		game_ui_role_portrait = {x=30,y=290},				
		
    player_head_sign_pos = {x=23,y=60},	     
    gameover_story_win_newrecord_pos = {x=375,y=207},	             
    gameover_battle_win_newrecord_pos = {x=375,y=207},	 
      
                
}

function  GetConfigPos(name)
	return CCPoint(pos[name].x,pos[name].y)
end

local ingame_ui_pos =
{
        ingame_ui_role_icon_pos={34,605},
        ingame_ui_role_hp_pos={20,517},        
        ingame_ui_boss_icon_pos={674,59},
        ingame_ui_boss_hp_pos={495,55},        
        
        ingame_ui_role_shoes_icon_pos={135,619},
        ingame_ui_role_shoes_num_pos={170,630-5},
        ingame_ui_role_bombnum_icon_pos={209,619},
        ingame_ui_role_bombnum_num_pos={245,630-5},
        ingame_ui_role_bombpower_icon_pos={284,618},
        ingame_ui_role_bombpower_num_pos={323,630-5},
        ingame_ui_role_gold_icon_pos={371,617},
        ingame_ui_role_gold_num_pos={440,630-5},
        
        ingame_ui_monster_num_pos={536,628},
        ingame_ui_map_sign_pos={615,620},
        ingame_ui_map_name_pos={666,628},
        ingame_ui_otherplayer1_icon_pos={731,618},
        ingame_ui_otherplayer2_icon_pos={791,618},
        ingame_ui_otherplayer3_icon_pos={846,618},
        
		    ingame_ui_score_num_pos  = {828,628-5},
		    ingame_ui_score_icon_pos = {748,619-5},
        ingame_ui_otherplayer1_hp_pos={758,619},
        ingame_ui_otherplayer2_hp_pos={817,619},
        ingame_ui_otherplayer3_hp_pos={873,619},
        ingame_ui_pausebtn_pos={928,605},
        ingame_ui_hp_item_pos={928,424},
        ingame_ui_timerbomb_item_pos={927,331},
        ingame_ui_bombbtn_item_pos={850,120},
        
        ingame_ui_role_sign_pos={34,605},
        ingame_ui_otherplayer1_sign_pos={732,619},
        ingame_ui_otherplayer2_sign_pos={789,619},
        ingame_ui_otherplayer3_sign_pos={846,619},
}

function GetInGameUIPos(name)
    return CCPoint(ingame_ui_pos[name][1]/2,ingame_ui_pos[name][2]/2)
end

local loadwin_ui_pos =
{
    loading_str_pos= {240,20},	
}

function get_loadwin_os(name)
    return CCPoint(loadwin_ui_pos[name][1]/2,loadwin_ui_pos[name][2]/2)
end


local  config_string=
{
    loading_str = "11111111111",
    tap_continue_str = "2222222222222",
}

function  GetConfigString(name)
	return config_string[name]
end




local fonts_lib =
{
    font01="font/font01.fnt",
    font02="font/font02.fnt",
    font03="font/font03.fnt",
    font04="font/font04.fnt",
    font05="font/font05.fnt",
    font06="font/font06.fnt",
}

local game_fonts=
{
    in_play_role_info_font = fonts_lib["font01"],
    in_play_role_monster_num_font = fonts_lib["font02"],
    map_name_font = fonts_lib["font02"],
    shop_item_price_font=fonts_lib["font02"],
    shop_item_count_font=fonts_lib["font02"],
    gameover_win_highscore_font=fonts_lib["font04"],
    gameover_win_currentscore_font=fonts_lib["font03"],
    ingame_currentscore_font=fonts_lib["font01"],
    revive_cost_font=fonts_lib["font05"],
    role_coins=fonts_lib["font02"],
    ingame_item_count=fonts_lib["font05"],
    stage_select_num=fonts_lib["font06"],
}

local reset_config=
{
		reset_coins = 300,
		reset_hp_num = 2,
		reset_controlled_bomb = 2,
}

function get_config_num(name)
	return reset_config[name]
end

function get_font(name)
    return game_fonts[name]
end
