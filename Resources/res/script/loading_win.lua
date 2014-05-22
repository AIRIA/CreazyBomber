module (..., package.seeall)
require "uimgr"
require "baseui"

local mainscene = MainScene:GetMainScene()
local game_music = GameMusic:GetGameMusic()
--启动游戏，入场动画时间为2秒
local enter_animate_time = 1300
--点击play按钮后，出现mainui之前的动画效果时间为2秒
local leave_animate_time = 500

--爆炸效果时间
local explode_animate_time = 200


local XOffset = 0
local DownOffset = 100
--mainscene的背景，直接加入到mainscene中,不再释放
sprite_list = 
{
    bg={file="bg.png", order = 10,disposable=false,pos={x=240,y=160}},              --没有动作，直接加到位置
    bg0={file="bg0.png", order = 30,disposable=false,pos={x=240,y=86}},          --有动作，先加到屏幕外面，下同
    bg1={file="bg1.png", order = 40,disposable=false,pos={x=240,y=52}},
    bg2={file="bg2.png", order = 50,disposable=false,pos={x=302,y=36}},
    cloud={file="cloud.png", order = 23,disposable=false,pos={x=240,y=283}},    
    sun={file="sun.png", order = 20,disposable=false,pos={x=156,y=286}},
 
    xiaochou={file="xiaochou.png",order = 25,disposable=true,pos={x=184,y=223}},
    bianfu={file="bianfu.png", order=25,disposable=true,pos={x=321,y=262}},
    pig={file="pig.png", order=25,disposable=true,pos={x=392,y=210}},            


    pinglianren={file="pinglianren.png", order=35,disposable=true,pos={x=434,y=116}},
    deng={file="deng.png", order=35,disposable=true,pos={x=72,y=187}},
    munaiyi={file="munaiyi.png", order=35,disposable=true,pos={x=162,y=161}},
    
    zhizhu={file="zhizhu.png", order=45,disposable=true,pos={x=350,y=145}},    
    lajitong={file="lajitong.png", order=45,disposable=true,pos={x=161,y=52}},        
 
    title={file="title.png", order=45,disposable=true,pos={x=1000,y=1000}},    
		logo={file="logo.png", order=60,disposable=true,	pos={x=809/2,y=63/2}},        
 
    bomb0={file="loading_bomb_0.png", order=60,disposable=true,pos={x=281,y=177}},    
    bomb1={file="loading_bomb_1.png", order=61,disposable=true,pos={x=281,y=177}},    
    bomb2={file="loading_bomb_2.png", order=62,disposable=true,pos={x=281,y=177}},    
    bomb3={file="loading_bomb_3.png", order=63,disposable=true,pos={x=281,y=177}},    
}

local enter_action=
{
    {id="sun", starttime=0,type = "move",parm={duration=100,  vibdir=1,offset={x=0,y=-120},pos={x=150,y=286},maxvibamp=33, maxvibtime=490}},   
    {id="sun", starttime=550,type = "rotate",parm={pos={x=150,y=286},time=600}},   
 
    {id="bg0", starttime=0, type = "move",parm={duration=100,  vibdir=1, offset={x=0,y=200},pos={x=240,y=86},maxvibamp=35, maxvibtime=510}},
    {id="bg0", starttime=590,type = "shake",parm={pos={x=240,y=86},time=1377,delta=1,degree=0.01,dir=1}},   


    {id="bg1", starttime=50,type = "move", parm={ duration=100, vibdir=1, offset={x=0,y=100},pos={x=240,y=52},maxvibamp=33, maxvibtime=470}},
    {id="bg1", starttime=600,type = "shake",parm={pos={x=240,y=52},time=1177,delta=1,degree=0.01,dir=1}},   
 
    {id="bg2", starttime=100,type = "move",parm={ duration=100,  vibdir=1,offset={x=0,y=50},pos={x=302,y=36},maxvibamp=29, maxvibtime=530}},
    {id="bg2", starttime=800,type = "shake",parm={pos={x=302,y=36},time=1257,delta=1,degree=0.01,dir=1}},   

    {id="cloud", starttime=100,type = "move",parm={ duration=100,  vibdir=1,offset={x=0,y=50},pos={x=240,y=283},maxvibamp=30, maxvibtime=570}},
    {id="cloud", starttime=800,type = "shake",parm={pos={x=240,y=283},time=1300,delta=3,degree=0.01,dir=1}},   


    
    {id="xiaochou", starttime=150,type = "move",parm={duration=500,  vibdir=1,offset={x=0,y=100},pos={x=184,y=223},maxvibamp=50, maxvibtime=570}},
    {id="xiaochou", starttime=800,type = "shake",parm={pos={x=184,y=223},time=817,delta=1,degree=0.01,dir=1}},   

    {id="bianfu", starttime=200,type = "move",parm={ duration=500,  vibdir=1,offset={x=0,y=100},pos={x=321,y=262},maxvibamp=50, maxvibtime=530}},
    {id="bianfu", starttime=800,type = "shake",parm={pos={x=321,y=262},time=841,delta=1,degree=0.01,dir=1}},   

    {id="pinglianren", starttime=250,type = "move", parm={ duration=500, vibdir=1,offset={x=0,y=100},pos={x=434,y=116},maxvibamp=50, maxvibtime=470}}, 
    {id="pinglianren", starttime=800,type = "shake",parm={pos={x=434,y=116},time=857,delta=1,degree=0.01,dir=1}},   

    {id="deng", starttime=300,type = "move", parm={ duration=500, vibdir=1,offset={x=0,y=100},pos={x=72,y=187},maxvibamp=50, maxvibtime=510}},     
    {id="deng", starttime=900,type = "shake",parm={pos={x=72,y=187},time=870,delta=1,degree=0.01,dir=1}},   

    {id="munaiyi", starttime=350,type = "move", parm={ duration=500, vibdir=1,offset={x=0,y=100},pos={x=162,y=161},maxvibamp=50, maxvibtime=490}},             
    {id="munaiyi", starttime=1000,type = "shake",parm={pos={x=162,y=161},time=910,delta=1,degree=0.01,dir=1}},   

    {id="zhizhu", starttime=400,type = "move", parm={ duration=500, vibdir=1,offset={x=0,y=100},pos={x=350,y=145},maxvibamp=50, maxvibtime=480}},                     
    {id="zhizhu", starttime=1050,type = "shake",parm={pos={x=350,y=145},time=930,delta=1,degree=0.01,dir=1}},   


    {id="lajitong", starttime=450,type = "move", parm={ duration=500, vibdir=1,offset={x=0,y=100},pos={x=161,y=52},maxvibamp=50, maxvibtime=460}},         
    {id="lajitong", starttime=1050,type = "shake",parm={pos={x=161,y=52},time=970,delta=1,degree=0.01,dir=1}},   
   
    {id="pig", starttime=500,type = "move", parm={ duration=500, vibdir=1,offset={x=0,y=100},pos={x=392,y=210},maxvibamp=50, maxvibtime=450}},                 
    {id="pig", starttime=1050,type = "shake",parm={pos={x=392,y=210},time=999,delta=1,degree=0.01,dir=1}},   
    
    {id="title", starttime=850,type = "move",parm={duration=350, vibdir=1, offset={x=0,y=-300},pos={x=240,y=250},maxvibamp=50, maxvibtime=410}},   
    {id="title", starttime=1300,type = "shake",parm={pos={x=240,y=250},time=1100,delta=0,degree=0.01,dir=1}},   
    {id="logo", starttime=850,type = "move",parm={duration=350, vibdir=1, offset={x=0,y=300},pos={x=809/2,y=63/2},maxvibamp=50, maxvibtime=410}},   
    --{id="logo", starttime=1300,type = "shake",parm={pos={x=240,y=250},time=1100,delta=0,degree=0.01,dir=1}},   
    
    
 	   
}

--目前离开动作沿着进来路线退出，计算退出动作
local bomb_action = 
{
    {id="bomb0", starttime=0, type = "bomb",parm={pos={x=240,y=160}, max_scale = 1.2, duration=200}},
    {id="bomb1", starttime=50,type = "bomb",parm={pos={x=240,y=160}, max_scale = 1.2, duration=200}},   
    {id="bomb2", starttime=100,type = "bomb",parm={pos={x=240,y=160}, max_scale = 1.2, duration=200}},   
    {id="bomb3", starttime=150,type = "bomb",parm={pos={x=240,y=160}, max_scale = 1.2, duration=200}},   
    
}
local leave_action = 
{
    {id="title", starttime=150,type = "move",parm={duration=100, vibdir=1, offset={x=0,y=300},pos={x=240,y=550},maxvibamp=0, maxvibtime=0}},           
		{id="logo", starttime=150,type = "move",parm={duration=100, vibdir=1, offset={x=0,y=-300},pos={x=809/2,y=63/2-100},maxvibamp=0, maxvibtime=0}},               
    {id="xiaochou", starttime=150,type = "move",parm={duration=100,  vibdir=1,offset={x=0,y=-100},pos={x=(-286+480)/2,y=(379+320)/2},maxvibamp=0, maxvibtime=0}},
    {id="bianfu", starttime=150,type = "move",parm={ duration=100,  vibdir=1,offset={x=0,y=-100},pos={x=(348+480)/2,y=(357+320)/2},maxvibamp=0, maxvibtime=0}},
    {id="pinglianren", starttime=150,type = "move", parm={ duration=100, vibdir=1,offset={x=0,y=-100},pos={x=(582+480)/2,y=(-91+320)/2},maxvibamp=0, maxvibtime=0}}, 
    {id="deng", starttime=150,type = "move", parm={ duration=100, vibdir=1,offset={x=0,y=-100},pos={x=(-545+480)/2,y=(86+320)/2},maxvibamp=0, maxvibtime=0}},     
    {id="munaiyi", starttime=150,type = "move", parm={ duration=100, vibdir=1,offset={x=0,y=-100},pos={x=(-546+480)/2,y=(344+320)/2},maxvibamp=0, maxvibtime=0}},             
    {id="zhizhu", starttime=150,type = "move", parm={ duration=100, vibdir=1,offset={x=0,y=-200},pos={x=(595+480)/2,y=(-380+320)/2},maxvibamp=0, maxvibtime=0}},                     
    {id="lajitong", starttime=150,type = "move", parm={ duration=100, vibdir=1,offset={x=0,y=-100},pos={x=(-54+480)/2,y=(-422+320)/2},maxvibamp=0, maxvibtime=0}},         
    {id="pig", starttime=150,type = "move", parm={ duration=100, vibdir=1,offset={x=0,y=-100},pos={x=(557+480)/2,y=(147+320)/2},maxvibamp=0, maxvibtime=0}},                 
}

function transfer(enter,leave)
    for id,v in ipairs(enter) do
         --背景不在移走
         if not (v["id"] == "bg0" or v["id"] == "bg1" or v["id"] == "bg2" or v["id"] == "sun") or v["id"] == "cloud" then
         local leave_action={}   
         leave_action["id"] = v["id"]
         leave_action["move"] = {}
         leave_action["move"]["starttime"] = v["move"]["starttime"]
         leave_action["move"]["endtime"] = v["move"]["endtime"]
         leave_action["move"]["startpos"] = {}
         leave_action["move"]["startpos"]["x"] = v["move"]["endpos"]["x"]
         leave_action["move"]["startpos"]["y"] =  v["move"]["endpos"]["y"]
         leave_action["move"]["endpos"] = {} 
         leave_action["move"]["endpos"]["x"] =  v["move"]["startpos"]["x"]
         leave_action["move"]["endpos"]["y"] =  v["move"]["startpos"]["y"]
         table.insert(leave,leave_action)
        end        
    end
end
--transfer(enter_action,leave_action)


loading_ui = {}

function loading_ui:new()
    local o = baseui.base_ui:new()

    setmetatable(loading_ui, baseui.base_ui)
    setmetatable(o, self)
    self.__index = self;
    o.ui_name="loading_ui" 
    o.sprite_list={}
    o.disposable_sprite={}
    self.play_btn = nil
    return o
end

--生成loading_ui 实例，并初始化
loading_ui_inst =loading_ui:new()
uimgr.ui_mgr_inst.active_ui =  loading_ui_inst
uimgr.ui_mgr_inst:register(loading_ui_inst)



function loading_ui:init()
    
    for i,v in pairs(sprite_list) do
        local sprite = FxLuaSprite:new()
        sprite:CreateFromFile(v["file"])
		local pos = CCPoint:new(v["pos"]["x"],v["pos"]["y"])
		sprite:SetPosition(pos)
		if v["anchor"] then
		    sprite:setAnchorPoint(v["anchor"])
        end
        sprite:SetVisible(false)
        mainscene:AddSprite(sprite, v["order"])
        if v["disposable"] == true then
            --加入需要释放列表
            table.insert(self.disposable_sprite,sprite)
        end
        --加入动画列表
        self.sprite_list[i]=sprite
	end
end

function loading_ui:do_move(action_list)
    for i,action in ipairs(action_list) do 
        local timer = {}
        
        local timer = {func=baseui.do_action, parm=action}
        local starttime = action["starttime"]
        local id=action["id"]
        local sprite = self.sprite_list[id]
        action["sprite"] = sprite
        uimgr.ui_mgr_inst:add_timer(starttime, true, timer)
    end 
end

function loading_ui:enter(parm)
    game_music:playMusic(music_main_bg)
    game_music:playEffect(ui_item_in)
    mainscene:SetUILayerTouchable(false)
		self.sprite_list["bg"]:SetVisible(true)
    self:do_move(enter_action)
    local timer={func = show_play_button, parm={}}
    --动画播放时间为2000毫秒
    uimgr.ui_mgr_inst:add_timer(enter_animate_time, false, timer)
end

function loading_ui:play_enter_animate()

   
    self:do_move(enter_actions)
end


function loading_ui:leave(target, parm)
    for i,v in pairs(self.disposable_sprite) do 
        v:SetVisible(false)
        mainscene:removeChild(v, true)
    end
    uimgr.ui_mgr_inst:goto_next_ui(target,parm)
end


function loading_ui:update(elapse)
end


function OnClickPlay(n,str)
    game_music:playEffect(item_bomb_explode)
    mainscene:SetUILayerTouchable(false)
    game_music:playEffect(ui_item_in);
    loading_ui_inst.play_btn:SetVisible(false)
    loading_ui_inst.play_btn:SetClickable(false) 
   
    loading_ui_inst:do_move(bomb_action)
    
    local timer={func=monster_leave, parm={}}
    uimgr.ui_mgr_inst:add_timer(explode_animate_time, true, timer)
end

function leave_loading_ui(parm)
    loading_ui_inst:leave(parm["next_win_name"],{})
    
end

function show_play_button()
    mainscene:SetUILayerTouchable(true)
	local play_btn = FxLuaButton:new()
	local pos = CCPoint:new(240,160)
    play_btn:CreateButton("play_btn_normal.png","play_btn_press.png", 55, pos, "loading_win","OnClickPlay")
	mainscene:AddButton(play_btn, 60)
	loading_ui_inst.play_btn = play_btn
    table.insert(loading_ui_inst.disposable_sprite,play_btn)
end

function monster_leave()
    game_music:playEffect(loading_monster_out)
    for i,v in ipairs(leave_action) do
        v["parm"]["offset"]["x"] = v["parm"]["pos"]["x"] - sprite_list[v["id"]]["pos"]["x"]
        v["parm"]["offset"]["y"] = v["parm"]["pos"]["y"] - sprite_list[v["id"]]["pos"]["y"]
    end
    loading_ui_inst:do_move(leave_action)
    
    local win_name = nil
    if mainscene:IsFirstPlay() then
        win_name = "select_role_ui"     
    else
        win_name = "main_ui"     
    
    end
    local timer={func=leave_loading_ui, parm={next_win_name=win_name}}
    uimgr.ui_mgr_inst:add_timer(leave_animate_time, true, timer)
    
end
