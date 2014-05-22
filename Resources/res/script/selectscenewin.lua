module (..., package.seeall)
require "baseui"
require "uimgr"

local mainscene = MainScene:GetMainScene()
local game_music = GameMusic:GetGameMusic()

local  enter_animate_time = 400
local  leave_animate_time = 400


local scene_names = 
{
    [0]="clstory",
    [1]="mdstory",   
    [2]="bcstory",    
    [3]="cllevel",    
    [4]="mdlevel",    
    [5]="bclevel",    
}

local btn_info =
{
    clstory = {name="clstory",order = 45, file_prefix="clstory",btn=nil, pos={x=326/2,y=139/2}, cbfunc="on_click_clstory"},
    mdstory = {name="mdstory",order = 45,file_prefix="mdstory",btn=nil, pos={x=563/2,y=195/2}, cbfunc="on_click_mdstory"},
    bcstory = {name="bcstory",order = 45,file_prefix="bcstory",btn=nil, pos={x=839/2,y=202/2}, cbfunc="on_click_bcstory"},
    cllevel = {name="cllevel",order = 35,file_prefix="clbattle",btn=nil, pos={x=691/2,y=404/2}, cbfunc="on_click_cllevel"},
    mdlevel = {name="mdlevel",order = 35,file_prefix="mdbattle",btn=nil, pos={x=436/2,y=394/2}, cbfunc="on_click_mdlevel"},
    bclevel = {name="bclevel",order = 35,file_prefix="bcbattle",btn=nil, pos={x=184/2,y=350/2}, cbfunc="on_click_bclevel"},
}

local enter_actions=
{
    {id="clstory", type= "move",starttime=0, parm={duration=200, vibdir=0,offset={x=0,y=200},pos={x=326/2,y=139/2}, maxvibamp=40, maxvibtime=150}},
    {id="clstory", starttime=400,type = "shake",parm={pos={x=326/2,y=139/2},dir=1,time=1111, delta=3, degree=0.01}}, 
   
    {id="mdstory",type= "move", starttime=150, parm={duration=200, vibdir=0, offset={x=0,y=200},pos={x=563/2,y=195/2},maxvibamp=40, maxvibtime=150}},
    {id="mdstory", starttime=400,type = "shake",parm={pos={x=563/2,y=195/2},dir=1,time=1333, delta=3, degree=0.01}}, 

    {id="bcstory", type= "move",starttime=300, parm={ duration=200, vibdir=0, offset={x=0,y=200},pos={x=839/2,y=202/2},maxvibamp=40, maxvibtime=150}},
    {id="bcstory", starttime=400,type = "shake",parm={pos={x=839/2,y=202/2},dir=1,time=1555, delta=3, degree=0.01}}, 

    {id="cllevel", type= "move",starttime=300, parm={ duration=200, vibdir=0, offset={x=0,y=200},pos={x=691/2,y=404/2},maxvibamp=40, maxvibtime=150}},
    {id="cllevel", starttime=400,type = "shake",parm={pos={x=691/2,y=404/2},dir=1,time=1222, delta=3, degree=0.01}}, 

    {id="mdlevel", type= "move",starttime=300, parm={ duration=200, vibdir=0, offset={x=0,y=200},pos={x=436/2,y=394/2},maxvibamp=40, maxvibtime=150}},
    {id="mdlevel", starttime=400,type = "shake",parm={pos={x=436/2,y=394/2},dir=1,time=1444, delta=3, degree=0.01}}, 

    {id="bclevel", type= "move",starttime=300, parm={ duration=200, vibdir=0, offset={x=0,y=200},pos={x=184/2,y=350/2},maxvibamp=40, maxvibtime=150}},
    {id="bclevel", starttime=400,type = "shake",parm={pos={x=184/2,y=350/2},dir=1,time=1666, delta=3, degree=0.01}}, 
}


local leave_actions=
{
    {id="clstory", type= "move",starttime=0, parm={duration=200, vibdir=0,offset={x=0,y=-300},pos={x=102,y=-180}, maxvibamp=40, maxvibtime=150}},
    {id="mdstory",type= "move", starttime=150, parm={duration=200, vibdir=0, offset={x=0,y=-300},pos={x=215,y=-116},maxvibamp=40, maxvibtime=150}},
    {id="bcstory", type= "move",starttime=300, parm={ duration=200, vibdir=0, offset={x=0,y=-300},pos={x=324,y=-125},maxvibamp=40, maxvibtime=150}},
    {id="cllevel", type= "move",starttime=300, parm={ duration=200, vibdir=0, offset={x=0,y=-300},pos={x=150,y=-220},maxvibamp=40, maxvibtime=150}},
    {id="mdlevel", type= "move",starttime=300, parm={ duration=200, vibdir=0, offset={x=0,y=-300},pos={x=265,y=-209},maxvibamp=40, maxvibtime=150}},
    {id="bclevel", type= "move",starttime=300, parm={ duration=200, vibdir=0, offset={x=0,y=-300},pos={x=377,y=-207},maxvibamp=40, maxvibtime=150}},
}


select_scene_ui = {}
function select_scene_ui:new()
    local o = baseui.base_ui:new()

    setmetatable(select_scene_ui, baseui.base_ui)
    setmetatable(o, self)
    self.__index = self;
    o.ui_name="select_scene_ui"
    o.back_btn = nil
    o.cur_select = nil
    self.mode = 0
    self.type= 0
    return o
end



function select_scene_ui:init()
    --创建关卡按钮
    for i,v in pairs(btn_info) do
        local btn = FxLuaButton:new()
        local pos = CCPoint:new(v["pos"]["x"], v["pos"]["y"])
        prefix = ""..v["file_prefix"]
        btn:CreateButton(prefix.."normal.png", prefix.."press.png", prefix.."disable.png",  v["order"], pos, CCRect:new(0,83,84,110),"selectscenewin",v["cbfunc"])
        v["btn"] = btn
        btn:SetVisible(false)
        select_scene_inst:add_node(btn)
    end
    
    for i,v in ipairs(enter_actions) do

        v["sprite"] = btn_info[v["id"]]["btn"]
    end
    for i,v in ipairs(leave_actions) do
        v["sprite"] = btn_info[v["id"]]["btn"]
    end
        
    
    
    local back_pos = CCPoint:new(105/2,86/2)
    local back_btn = FxLuaButton:new()
    back_btn:CreateButton("back_normal.png","back_press.png","back_disable.png",60, back_pos, "selectscenewin","on_click_back")
    back_btn:SetVisible(false)
    select_scene_inst:add_node(back_btn)
    select_scene_inst.back_btn = back_btn
    
end

function select_scene_ui:enter(parm)
    mainscene:SetUILayerTouchable(false)
    game_music:playMusic(music_main_bg)
    game_music:playEffect(ui_item_in)

    mainscene:refresh_scene_ui_data()
    --顺序  故事:丛林/冰川/墓地  关卡:丛林/冰川/墓地
    for i=0,5  do
        isUnLock = mainscene["scene_infos"][i]["isUnLock"]
         --是否加锁
        -- isUnLock = true
        if not isUnLock then
            btn_info[scene_names[i]]["btn"]:SetClickable(false)
        else
            btn_info[scene_names[i]]["btn"]:SetClickable(true)
        end
        --mainscene:AddButton(btn_info[i+1]["btn"], 60)
    end
    --mainscene:AddButton(select_scene_inst.back_btn, 60)
    
      --播放按钮出现动画
    self:play_enter_animate()

    --设置播放结束回调
     local timer = {func=on_enter_animate_over, {}}
     uimgr.ui_mgr_inst:add_timer(enter_animate_time, true, timer)      
    
end

function select_scene_ui:leave(target, parm)
    mainscene:SetUILayerTouchable(false)
    self:play_leave_animate()
     --设置播放结束回调
     local timer = {func=on_leave_animate_over, parm={next_win_name=target}}
     uimgr.ui_mgr_inst:add_timer(leave_animate_time, true, timer)
    --[[ 
     self:show(false)
     self:remove_from_scene()
     uimgr.ui_mgr_inst:goto_next_ui(target,parm)
    --]]
end



function select_scene_ui:play_enter_animate()
    --加入场景
    for i,v in pairs(btn_info) do
        --v["btn"]:SetVisible(true)
        mainscene:AddButton(v["btn"], v["btn"]:GetOrderInParent())
    end
    self.back_btn:SetVisible(false)
    self:do_move(enter_actions)
end


function on_enter_animate_over()
    self = select_scene_inst

    self.back_btn:SetVisible(true)
    mainscene:SetUILayerTouchable(true)
    mainscene:AddButton(self.back_btn, self.back_btn:GetOrderInParent())   
    mainscene:SetUILayerTouchable(true)
end

function select_scene_ui:do_move(action_list)
    for i,action in ipairs(action_list) do 
        
        local timer = {}
        
        local timer = {func=baseui.do_action, parm=action}
        local starttime = action["starttime"]
        uimgr.ui_mgr_inst:add_timer(starttime, true, timer)      
    end 
end

function select_scene_ui:play_leave_animate()
    mainscene:SetUILayerTouchable(false)
    self.back_btn:SetVisible(false)
    mainscene:removeChild(self.back_btn, false)   
 
    self:do_move(leave_actions)
end

function on_leave_animate_over(parm)
    
    --让所有按钮进入不可选择状态,并隐去返回按钮
    self = select_scene_inst
    for i,v in pairs(btn_info) do
            v["btn"]:SetVisible(false)
            mainscene:removeChild(v["btn"], false)
    end
    mainscene:SetUILayerTouchable(true)
    next_ui = parm["next_win_name"]
    uimgr.ui_mgr_inst:goto_next_ui(next_ui,{mode=self.mode,type=self.type})
end



select_scene_inst =select_scene_ui:new()
uimgr.ui_mgr_inst:register(select_scene_inst)


back_btn = nil
cur_select = nil

function SetBtnState(select)
    for i=0,5  do
        if select == i then
            btn_info[i+1]["btn"]:SetSelected(true)
        else
            btn_info[i+1]["btn"]:SetSelected(false)
       end
    end
end


function OnClick(m, t)
    self = select_scene_inst
    self.mode = m
    self.type = t
    select_scene_inst:leave("select_stage_ui",{mode=m, type=t})
end
function on_click_clstory(n,str)
    OnClick(SceneCatlog_Main, SceneArt_CaoDi)
end
function on_click_bcstory(n,str)
    OnClick(SceneCatlog_Main, SceneArt_BingChuan)
end

function on_click_mdstory(n,str)
    OnClick(SceneCatlog_Main, SceneArt_MoDi)
end

function on_click_cllevel(n,str)
     OnClick(SceneCatlog_Branch, SceneArt_CaoDi)
end
function on_click_bclevel(n,str)
    OnClick(SceneCatlog_Branch, SceneArt_BingChuan)
end

function on_click_mdlevel(n,str)
    OnClick(SceneCatlog_Branch, SceneArt_MoDi)
end

function on_click_back(n,str)
    select_scene_inst:leave("main_ui", {})
end

