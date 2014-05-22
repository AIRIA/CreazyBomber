module (..., package.seeall)
require "baseui"
require "uimgr"

local mainscene = MainScene:GetMainScene()
local game_music = GameMusic:GetGameMusic()

local  enter_animate_time = 1000
local  leave_animate_time = 1000

local mode_info =
{
   mode_1v1 = {normal="vs_player_1v1_normal.png",press="vs_player_1v1_press.png",disable="vs_player_1v1_press.png",order=35, pos={x=131/2,y=359/2}, cbfunc="on_click_mode", val=BattleModeTag_1V1},
   mode_1v2 = {normal="vs_player_1v2_normal.png",press="vs_player_1v2_press.png",disable="vs_player_1v2_press.png",order=35, pos={x=328/2,y=385/2}, cbfunc="on_click_mode", val=BattleModeTag_1V2},
   mode_1v3 = {normal="vs_player_1v3_normal.png",press="vs_player_1v3_press.png",disable="vs_player_1v3_press.png",order=35, pos={x=489/2,y=357/2}, cbfunc="on_click_mode", val=BattleModeTag_1V3},
   mode_2v2 = {normal="vs_player_2v2_normal.png",press="vs_player_2v2_press.png",disable="vs_player_2v2_press.png",order=35, pos={x=660/2,y=395/2}, cbfunc="on_click_mode", val=BattleModeTag_2V2},
   mode_melee = {normal="vs_player_melee_normal.png",press="vs_player_melee_press.png",disable="vs_player_melee_press.png",order=35, pos={x=832/2,y=337/2}, cbfunc="on_click_mode", val=BattleModeTag_Melee},
}
local option_info =
{ 
   --select_map={normal="vs_map_random_normal.png",press="vs_map_random_press.png",disable="vs_map_random_normal.png",order=55, pos={x=248,y=97},cbfunc="on_click_select_map", val=-1},
   diff_easy = {normal="vs_diff_easy_normal.png",press="vs_diff_easy_press.png",disable="vs_diff_easy_press.png",order=55, pos={x=263/2,y=88/2}, cbfunc="on_click_diff", val=AI_Fool},
   diff_middle = {normal="vs_diff_middle_normal.png",press="vs_diff_middle_press.png",disable="vs_diff_middle_press.png",order=55, pos={x=481/2,y=88/2},cbfunc="on_click_diff", val=AI_General},
   diff_hard = {normal="vs_diff_hard_normal.png",press="vs_diff_hard_press.png",disable="vs_diff_hard_press.png",order=55, pos={x=696/2,y=88/2},cbfunc="on_click_diff", val=AI_Clever},             
   back = {normal="back_normal.png",press="back_press.png",disable="back_disable.png",order=55, pos={x=105/2,y=84/2},cbfunc="on_click_back", val=-1},
   ok = {normal="ok_normal.png",press="ok_press.png",disable="ok_normal.png",order=55, pos={x=852/2,y=86/2},cbfunc="on_click_ok", val=-1},             
}



local enter_actions=
{
    {id="mode_1v1", type= "move",starttime=0, parm={duration=200, vibdir=0,offset={x=0,y=200},pos={x=131/2,y=359/2}, maxvibamp=40, maxvibtime=150}},
    {id="mode_1v1", starttime=500,type = "shake",parm={pos={x=131/2,y=359/2},dir=1,time=1111, delta=3, degree=0.01}}, 
   
    {id="mode_1v2",type= "move", starttime=0, parm={duration=200, vibdir=0, offset={x=0,y=200},pos={x=328/2,y=385/2},maxvibamp=40, maxvibtime=150}},
    {id="mode_1v2", starttime=500,type = "shake",parm={pos={x=328/2,y=385/2},dir=1,time=1333, delta=3, degree=0.01}}, 

    {id="mode_1v3", type= "move",starttime=0, parm={ duration=200, vibdir=0, offset={x=0,y=200},pos={x=489/2,y=357/2},maxvibamp=40, maxvibtime=150}},
    {id="mode_1v3", starttime=500,type = "shake",parm={pos={x=489/2,y=357/2},dir=1,time=1555, delta=3, degree=0.01}}, 

    {id="mode_2v2", type= "move",starttime=0, parm={ duration=200, vibdir=0, offset={x=0,y=200},pos={x=660/2,y=395/2},maxvibamp=40, maxvibtime=150}},
    {id="mode_2v2", starttime=500,type = "shake",parm={pos={x=660/2,y=395/2},dir=1,time=1222, delta=3, degree=0.01}}, 

    {id="mode_melee", type= "move",starttime=0, parm={ duration=200, vibdir=0, offset={x=0,y=200},pos={x=832/2,y=337/2},maxvibamp=40, maxvibtime=150}},
    {id="mode_melee", starttime=500,type = "shake",parm={pos={x=832/2,y=337/2},dir=1,time=1444, delta=3, degree=0.01}}, 

 
}


local leave_actions=
{

    
    {id="mode_1v1", type= "move",starttime=0, parm={duration=200, vibdir=0,offset={x=0,y=-300},pos={x=65,y=-109}, maxvibamp=40, maxvibtime=150}},
    {id="mode_1v2",type= "move", starttime=0, parm={duration=200, vibdir=0, offset={x=0,y=-300},pos={x=156,y=-102},maxvibamp=40, maxvibtime=150}},
    {id="mode_1v3", type= "move",starttime=0, parm={ duration=200, vibdir=0, offset={x=0,y=-300},pos={x=244,y=-75},maxvibamp=40, maxvibtime=150}},
    {id="mode_2v2", type= "move",starttime=0, parm={ duration=200, vibdir=0, offset={x=0,y=-300},pos={x=330,y=-109},maxvibamp=40, maxvibtime=150}},
    {id="mode_melee", type= "move",starttime=0, parm={ duration=200, vibdir=0, offset={x=0,y=-300},pos={x=419,y=-130},maxvibamp=40, maxvibtime=150}},
}



vs_mode_ui = {}
function vs_mode_ui:new()
    local o = baseui.base_ui:new()
    
    setmetatable(vs_mode_ui, baseui.base_ui)
    setmetatable(o, self)
    self.__index = self;
    o.ui_name="vs_mode_ui" 
    o.back_btn = nil
    o.mode = BattleModeTag_1V1
    o.diff = AI_Fool
    o.mode_list = {}
    o.btn_list={}
    o.diff_list={}
    o.select_map=-1
    o.select_map_btn = nil
    return o
end


vs_mode_inst= vs_mode_ui:new()
uimgr.ui_mgr_inst:register(vs_mode_inst)

function vs_mode_ui:init()

  for i,v in pairs(mode_info) do
        local btn = FxLuaButton:new()
        local pos = CCPoint:new(v["pos"]["x"], v["pos"]["y"])
        
        btn:CreateButton(v["normal"],v["press"],v["disable"],v["order"], pos, CCRect:new(28,73,58,76), "vsmodewin",v["cbfunc"])
        btn:SetInt(v["val"])
        btn:SetString(i)
        --vs_mode_inst:add_node(btn)
        self.mode_list[i] = btn
     
    end  
  for i,v in pairs(option_info) do
        local btn = FxLuaButton:new()
        local pos = CCPoint:new(v["pos"]["x"], v["pos"]["y"])
        
        btn:CreateButton(v["normal"],v["press"],v["disable"],v["order"], pos, "vsmodewin",v["cbfunc"])
        btn:SetInt(v["val"])
        btn:SetString(i)
        self.btn_list[i] = btn
        if i ~= "ok" and i ~= "back" then
            self.diff_list[i]=btn
        end
    end  
    
    for i,v in ipairs(enter_actions) do
        v["sprite"] = self.mode_list[v["id"]]
    end
    for i,v in ipairs(leave_actions) do
        v["sprite"] = self.mode_list[v["id"]]
    end
       

end

function vs_mode_ui:enter(parm)
    mainscene:SetUILayerTouchable(false) 
    game_music:playMusic(music_main_bg)
    game_music:playEffect(ui_item_in)

     --播伐出现动画
    self:play_enter_animate()
        --设置播放结束回调
     local timer = {func=on_enter_animate_over, {}}
     uimgr.ui_mgr_inst:add_timer(enter_animate_time, true, timer)  
  
end

function vs_mode_ui:leave(target, p)
    mainscene:SetUILayerTouchable(false)
    self:play_leave_animate()
     --设置播放结束回调
     local timer = {func=on_leave_animate_over, parm={next_win_name=target,diff=p["diff"],mode=p["mode"]}}
     uimgr.ui_mgr_inst:add_timer(leave_animate_time, true, timer)
  
end



function vs_mode_ui:play_enter_animate()
    --加入场景
    for i,v in pairs(self.mode_list) do
         v:SetVisible(false)
         mainscene:AddButton(v, v:GetOrderInParent())
    end
   
    self:do_move(enter_actions)
    
end


function on_enter_animate_over()
    self = vs_mode_inst
    for i,v in pairs(self.btn_list) do 
         v:SetVisible(true)
         mainscene:AddButton(v, v:GetOrderInParent())
    end
    update_mode_select("mode_1v1")
    update_diff_select("diff_easy")
    mainscene:SetUILayerTouchable(true)    
end

function vs_mode_ui:do_move(action_list)
    for i,action in ipairs(action_list) do 
        local timer = {}
        local timer = {func=baseui.do_action, parm=action}
        local starttime = action["starttime"]
        uimgr.ui_mgr_inst:add_timer(starttime, true, timer)      
    end 
end

function vs_mode_ui:play_leave_animate()
    mainscene:SetUILayerTouchable(false)
    for i,v in pairs(self.btn_list) do 
         v:SetVisible(false)
         mainscene:removeChild(v, false)
    end
    self:do_move(leave_actions)
end

function on_leave_animate_over(p)
    --让所有按钮进入不可选择状态,并隐去返回按钮
    self=vs_mode_inst
    for i,v in pairs(self.mode_list) do 
         v:SetVisible(false)
         mainscene:removeChild(v, false)
    end

    next_ui = p["next_win_name"]
    
    uimgr.ui_mgr_inst:goto_next_ui(next_ui,{mode=p["mode"],diff=p["diff"]})
end

function  update_mode_select(selected)
    self = vs_mode_inst
    
	for i,v in pairs(self.mode_list) do
		if selected == i then
			v:SetSelected(true)
		else
			v:SetSelected(false)
		end
	end
end		

function  update_diff_select(selected)
	for i,v in pairs(self.diff_list) do
		if selected == i then
			v:SetSelected(true)
		else
			v:SetSelected(false)
		end
	end

end

function on_click_mode(n,str)
    vs_mode_inst.mode = n
    update_mode_select(str)
    
end
function on_click_diff(n,str)
    vs_mode_inst.diff = n
    update_diff_select(str)
end
function on_click_select_map(n,str)
    vs_mode_inst:leave("vs_select_stage_ui",{})
end
function on_click_back(n,str)
    vs_mode_inst:leave("main_ui",{})
end
function on_click_ok(n,str)
    --mainscene:PlayVSGame(vs_mode_inst.diff,vs_mode_inst.mode,vs_mode_inst.select_map)
    
    vs_mode_inst:leave("vs_select_stage_ui",{diff=vs_mode_inst.diff,mode=vs_mode_inst.mode})
end




