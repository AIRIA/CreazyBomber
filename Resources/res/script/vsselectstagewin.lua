module (..., package.seeall)
require "baseui"
require "uimgr"
local game_music = GameMusic:GetGameMusic()






local map_infos =
{

    map_0={id=0,name = "1",rc = CCRect:new(16,32,49,47),normal="cl_stage1_normal_vs.png",press="cl_stage1_press_vs.png",disable="cl_stage1_normal_vs.png",pos={357/2,142/2},name_pos={96/2,80/2},order=45,cbfunc="on_click_level",val=0},
    map_1={id=1,name = "2",rc = CCRect:new(16,32,49,47),normal="cl_stage1_normal_vs.png",press="cl_stage1_press_vs.png",disable="cl_stage1_normal_vs.png",pos={519/2,207/2},name_pos={100/2,85/2},order=45,cbfunc="on_click_level",val=1},
    map_2={id=2,name = "3",rc = CCRect:new(16,32,49,47),normal="cl_stage1_normal_vs.png",press="cl_stage1_press_vs.png",disable="cl_stage1_normal_vs.png",pos={696/2,231/2},name_pos={100/2,85/2},order=45,cbfunc="on_click_level",val=2},
    map_3={id=3,name = "4", rc = CCRect:new(10,34,71,37),normal="md_stage2_normal_vs.png",press="md_stage2_press_vs.png",disable="md_stage2_normal_vs.png",pos={850/2,267/2},name_pos={85/2,88/2},order=35,cbfunc="on_click_level",val=3},
    map_4={id=4,name = "5",rc = CCRect:new(10,34,71,37),normal="md_stage2_normal_vs.png",press="md_stage2_press_vs.png",disable="md_stage2_normal_vs.png",pos={635/2,314/2},name_pos={88/2,104/2},order=35,cbfunc="on_click_level",val=4},
    map_5={id=5,name = "6",rc = CCRect:new(10,34,71,37),normal="md_stage2_normal_vs.png",press="md_stage2_press_vs.png",disable="md_stage2_normal_vs.png",pos={469/2,336/2},name_pos={88/2,101/2},order=35,cbfunc="on_click_level",val=5},
    map_6={id=6,name = "7",rc = CCRect:new(10,34,71,37),normal="md_stage2_normal_vs.png",press="md_stage2_press_vs.png",disable="md_stage2_normal_vs.png",pos={306/2,338/2},name_pos={88/2,98/2},order=35,cbfunc="on_click_level",val=6},
    map_7={id=7,name = "8",rc = CCRect:new(10,34,71,37),normal="md_stage2_normal_vs.png",press="md_stage2_press_vs.png",disable="md_stage2_normal_vs.png",pos={111/2,325/2},name_pos={88/2,103/2},order=35,cbfunc="on_click_level",val=7},
    map_8={id=8,name = "9",rc = CCRect:new(19,35,60,50),normal="bc_stage1_normal_vs.png",press="bc_stage1_press_vs.png",disable="bc_stage1_normal_vs.png",pos={224/2,465/2},name_pos={80/2,109/2},order=25,cbfunc="on_click_level",val=8},
    map_9={id=9,name = "10",rc = CCRect:new(19,35,60,50),normal="bc_stage1_normal_vs.png",press="bc_stage1_press_vs.png",disable="bc_stage1_normal_vs.png",pos={438/2,486/2},name_pos={75/2,104/2},order=25,cbfunc="on_click_level",val=9},
    map_10={id=10,name = "11",rc = CCRect:new(19,35,60,50),normal="bc_stage1_normal_vs.png",press="bc_stage1_press_vs.png",disable="bc_stage1_normal_vs.png",pos={614/2,488/2},name_pos={74/2,112/2},order=25,cbfunc="on_click_level",val=10},
    map_11={id=11,name = "12",rc = CCRect:new(19,35,60,50),normal="bc_stage1_normal_vs.png",press="bc_stage1_press_vs.png",disable="bc_stage1_normal_vs.png",pos={807/2,472/2},name_pos={75/2,115/2},order=25,cbfunc="on_click_level",val=11},
    map_back={name="back_btn",normal="back_normal.png",press="back_press.png",disable="back_normal.png",pos={106/2,85/2},order=100,cbfunc="on_click_back",val=-1} ,     
    map_random={name="random_btn",normal="vs_map_stage_random_normal.png",press="vs_map_random_press.png",disable="vs_map_random_normal.png",pos={350,32},order=100,cbfunc="on_click_random",val=-1} ,     
}
local actions = 
{     
        enter_animate_time = 600,
        leave_animate_time = 600,
        enter=
        {  
                [0]={ type= "move",starttime=0,   parm={duration=50, vibdir=0,offset={x=0,y=100}, maxvibamp=30, maxvibtime=100}},
                [1]={ type= "move",starttime=50,  parm={duration=50, vibdir=0,offset={x=0,y=100}, maxvibamp=30, maxvibtime=100}},
                [2]={ type= "move",starttime=100, parm={duration=50, vibdir=0,offset={x=0,y=100}, maxvibamp=30, maxvibtime=100}},
                [3]={ type= "move",starttime=150, parm={duration=50, vibdir=0,offset={x=0,y=100}, maxvibamp=30, maxvibtime=100}},
                [4]={ type= "move",starttime=200, parm={duration=50, vibdir=0,offset={x=0,y=100}, maxvibamp=30, maxvibtime=100}},
                [5]={ type= "move",starttime=250, parm={duration=50, vibdir=0,offset={x=0,y=100}, maxvibamp=30, maxvibtime=100}},
                [6]={ type= "move",starttime=300, parm={duration=50, vibdir=0,offset={x=0,y=100}, maxvibamp=30, maxvibtime=100}},
                [7]={ type= "move",starttime=350, parm={duration=50, vibdir=0,offset={x=0,y=100}, maxvibamp=30, maxvibtime=100}},
                [8]={ type= "move",starttime=400, parm={duration=50, vibdir=0,offset={x=0,y=100}, maxvibamp=30, maxvibtime=100}},
                [9]={ type= "move",starttime=450, parm={duration=50, vibdir=0,offset={x=0,y=100}, maxvibamp=30, maxvibtime=100}},
               [10]={ type= "move",starttime=500, parm={duration=50, vibdir=0,offset={x=0,y=100}, maxvibamp=30, maxvibtime=100}},
               [11]={ type= "move",starttime=550, parm={duration=50, vibdir=0,offset={x=0,y=100}, maxvibamp=30, maxvibtime=100}},
        },
        shake=
        {
                [0]={ starttime=200,type = "shake",parm={dir=1,time=1111, delta=3, degree=0.01}}, 
                [1]={ starttime=250,type = "shake",parm={dir=1,time=1222, delta=3, degree=0.01}}, 
                [2]={ starttime=300,type = "shake",parm={dir=1,time=1333, delta=3, degree=0.01}}, 
                [3]={ starttime=350,type = "shake",parm={dir=1,time=1333, delta=3, degree=0.01}}, 
                [4]={ starttime=400,type = "shake",parm={dir=1,time=1555, delta=3, degree=0.01}}, 
                [5]={ starttime=450,type = "shake",parm={dir=1,time=1666, delta=3, degree=0.01}}, 
                [6]={ starttime=500,type = "shake",parm={dir=1,time=1111, delta=3, degree=0.01}}, 
                [7]={ starttime=550,type = "shake",parm={dir=1,time=1222, delta=3, degree=0.01}}, 
                [8]={ starttime=600,type = "shake",parm={dir=1,time=1333, delta=3, degree=0.01}}, 
                [9]={ starttime=650,type = "shake",parm={dir=1,time=1444, delta=3, degree=0.01}}, 
                [10]={ starttime=700,type = "shake",parm={dir=1,time=1555, delta=3, degree=0.01}}, 
                [11]={ starttime=750,type = "shake",parm={dir=1,time=1666, delta=3, degree=0.01}},                 
       },
        leave=
            {  
                [0]={ type= "move",starttime=0,   parm={duration=50, vibdir=0,offset={x=0,y=-100}, maxvibamp=0, maxvibtime=0}},
                [1]={ type= "move",starttime=50,  parm={duration=50, vibdir=0,offset={x=0,y=-100}, maxvibamp=0, maxvibtime=0}},
                [2]={ type= "move",starttime=100, parm={duration=50, vibdir=0,offset={x=0,y=-100}, maxvibamp=0, maxvibtime=0}},
                [3]={ type= "move",starttime=150, parm={duration=50, vibdir=0,offset={x=0,y=-100}, maxvibamp=0, maxvibtime=0}},
                [4]={ type= "move",starttime=200, parm={duration=50, vibdir=0,offset={x=0,y=-100}, maxvibamp=0, maxvibtime=0}},
                [5]={ type= "move",starttime=250, parm={duration=50, vibdir=0,offset={x=0,y=-100}, maxvibamp=0, maxvibtime=0}},
                [6]={ type= "move",starttime=300, parm={duration=50, vibdir=0,offset={x=0,y=-100}, maxvibamp=0, maxvibtime=0}},
                [7]={ type= "move",starttime=350, parm={duration=50, vibdir=0,offset={x=0,y=-100}, maxvibamp=0, maxvibtime=0}},
                [8]={ type= "move",starttime=400, parm={duration=50, vibdir=0,offset={x=0,y=-100}, maxvibamp=0, maxvibtime=0}},
                [9]={ type= "move",starttime=450, parm={duration=50, vibdir=0,offset={x=0,y=-100}, maxvibamp=0, maxvibtime=0}},
               [10]={ type= "move",starttime=500, parm={duration=50, vibdir=0,offset={x=0,y=-100}, maxvibamp=0, maxvibtime=0}},
               [11]={ type= "move",starttime=550, parm={duration=50, vibdir=0,offset={x=0,y=-100}, maxvibamp=0, maxvibtime=0}},
            },

}


local mainscene = MainScene:GetMainScene()
local game_music = GameMusic:GetGameMusic()

vs_select_stage_ui = {}
function vs_select_stage_ui:new()
    local o = baseui.base_ui:new()
    
    setmetatable(vs_select_stage_ui, baseui.base_ui)
    setmetatable(o, self)
    self.__index = self;
    o.ui_name="vs_select_stage_ui" 
    --o.back_btn = nil
    o.select_map_id = 0
    o.diff  = 0
    o.mode = 0
    o.enter_actions = {}
    o.leave_actions = {}
    o.shake_actions = {}
    o.enter_animate_time = 0
    o.leave_animate_time = 0
    o.btn_list={}
    return o
end
--声明窗口示例，注册窗口
vs_select_stage_inst = vs_select_stage_ui:new()
uimgr.ui_mgr_inst:register(vs_select_stage_inst)

function vs_select_stage_ui:init()
    --创建关卡按钮
    for i,v in pairs(map_infos) do
        local btn = FxLuaButton:new("stage_select_num")
        local pos = CCPoint:new(v["pos"][1], v["pos"][2])
        if v["rc"]then
            btn:CreateButton(v["normal"], v["press"], v["disable"], v["order"], pos,v["rc"],"vsselectstagewin",v["cbfunc"])        
            local namepos = CCPoint:new(v["name_pos"][1], v["name_pos"][2])    
            btn:SetNameString(v["name"],namepos)
        else
            btn:CreateButton(v["normal"], v["press"], v["disable"], v["order"], pos,"vsselectstagewin",v["cbfunc"])
            
        end

        btn:SetVisible(false)
        btn:SetClickable(false)
        btn:SetInt(v["val"])
        --记录btn按钮
        self.btn_list[v["name"]] = btn
        if  v["rc"]    then           
            local enter_action ={}
            enter_action["sprite"] = btn
            enter_action["type"] = actions["enter"][v["id"]]["type"]
            enter_action["starttime"] = actions["enter"][v["id"]]["starttime"]
            enter_action["parm"] = {}
            
            enter_action["parm"]["duration"] = actions["enter"][v["id"]]["parm"]["duration"]
            enter_action["parm"]["vibdir"] = actions["enter"][v["id"]]["parm"]["vibdir"]
            enter_action["parm"]["maxvibamp"] = actions["enter"][v["id"]]["parm"]["maxvibamp"]
            enter_action["parm"]["maxvibtime"] = actions["enter"][v["id"]]["parm"]["maxvibtime"]
            enter_action["parm"]["offset"] = {}
            enter_action["parm"]["offset"]["x"] = actions["enter"][v["id"]]["parm"]["offset"]["x"]
            enter_action["parm"]["offset"]["y"] = actions["enter"][v["id"]]["parm"]["offset"]["y"]
            enter_action["parm"]["pos"] = {}
            enter_action["parm"]["pos"]["x"] = v["pos"][1]
            enter_action["parm"]["pos"]["y"] = v["pos"][2]
            self.enter_actions[v["id"]] = enter_action
            
            
            local leave_action ={}
            leave_action["sprite"] = btn
            leave_action["type"] = actions["leave"][v["id"]]["type"]
            leave_action["starttime"] = actions["leave"][v["id"]]["starttime"]
            leave_action["parm"] = {}
            
            leave_action["parm"]["duration"] = actions["leave"][v["id"]]["parm"]["duration"]
            leave_action["parm"]["vibdir"] = actions["leave"][v["id"]]["parm"]["vibdir"]
            leave_action["parm"]["maxvibamp"] = actions["leave"][v["id"]]["parm"]["maxvibamp"]
            leave_action["parm"]["maxvibtime"] = actions["leave"][v["id"]]["parm"]["maxvibtime"]
            leave_action["parm"]["offset"] = {}
            leave_action["parm"]["offset"]["x"] = actions["leave"][v["id"]]["parm"]["offset"]["x"]
            leave_action["parm"]["offset"]["y"] = actions["leave"][v["id"]]["parm"]["offset"]["y"]
            leave_action["parm"]["pos"] = {}
            leave_action["parm"]["pos"]["x"] = v["pos"][1] + actions["leave"][v["id"]]["parm"]["offset"]["x"]
            leave_action["parm"]["pos"]["y"] = v["pos"][2] + actions["leave"][v["id"]]["parm"]["offset"]["y"]
            self.leave_actions[v["id"]] = leave_action        
                    
            local shake_action ={}
            shake_action["sprite"] = btn
            shake_action["type"] = actions["shake"][v["id"]]["type"]
            shake_action["starttime"] = actions["shake"][v["id"]]["starttime"]
            shake_action["parm"] = {}
            shake_action["parm"]["dir"] = actions["shake"][v["id"]]["parm"]["dir"]
            shake_action["parm"]["time"] = actions["shake"][v["id"]]["parm"]["time"]
            shake_action["parm"]["delta"] = actions["shake"][v["id"]]["parm"]["delta"]
            shake_action["parm"]["degree"] = actions["shake"][v["id"]]["parm"]["degree"]        
            shake_action["parm"]["pos"]={}
            shake_action["parm"]["pos"]["x"] = v["pos"][1]
            shake_action["parm"]["pos"]["y"] = v["pos"][2]
            self.shake_actions[v["id"]] = shake_action        
        end
    end
    
    self.enter_animate_time = actions.enter_animate_time
    self.leave_animate_time  = actions.leave_animate_time   
end

function vs_select_stage_ui:enter(parm)
    mainscene:SetUILayerTouchable(false)
    game_music:playMusic(music_main_bg)
    game_music:playEffect(ui_item_in)

    self.diff=parm["diff"]    
    self.mode=parm["mode"]    
    parm["diff"] = nil
    self:play_enter_animate()
    
    --设置播放结束回调
     local timer = {func=on_enter_animate_over, {}}
     local enter_animate_time = self.enter_animate_time
     uimgr.ui_mgr_inst:add_timer(enter_animate_time, true, timer)
    
end
function vs_select_stage_ui:play_enter_animate()
    mainscene:SetUILayerTouchable(false)
        --设置按钮可见
    for i,v in pairs(self.btn_list) do
       if i ~= "back_btn" and i ~= "random_btn"then
          --v:SetVisible(true)
          mainscene:AddButton(v, v:GetOrderInParent())
       end
    end
    self:do_move(self.enter_actions)

end

function on_enter_animate_over()
    self = vs_select_stage_inst
    --设置按钮可见
    for i,v in pairs(self.btn_list) do

       if i == "back_btn" or i == "random_btn"then
         --设置返回按钮可见并加入场景
          v:SetVisible(true)
          mainscene:AddButton(v, v:GetOrderInParent())
       end 
       v:SetClickable(true)
    end
    mainscene:SetUILayerTouchable(true)
end

function vs_select_stage_ui:leave(target, parm)
    mainscene:SetUILayerTouchable(false)
    self:play_leave_animate()
     
     --设置播放结束回调
     local timer = {func=on_leave_animate_over, parm={next_win_name=target, map_id=parm["map_id"]}}
     local leave_animate_time = self.leave_animate_time
     uimgr.ui_mgr_inst:add_timer(leave_animate_time, true, timer)    
end

function vs_select_stage_ui:play_leave_animate()
  
    mainscene:SetUILayerTouchable(false)
    --让所有按钮进入不可选择状态,并隐去返回按钮
    for i,v in pairs(self.btn_list) do
       if i == "back_btn" or i == "random_btn"then
         --设置返回按钮可见并加入场景
          v:SetVisible(false)
          v:SetClickable(false)
          mainscene:removeChild(v, false)
       end
    end
    
    self:do_move(self.leave_actions)
end

function on_leave_animate_over(parm)
    --让所有按钮进入不可选择状态,并隐去返回按钮
    self=vs_select_stage_inst
    for i,v in pairs(self.btn_list) do
        if i~= "back_btn" and i~= "random_btn" then
            v:SetVisible(false)
            v:SetClickable(false)            
            mainscene:removeChild(v, false)
        end
    end
    mainscene:SetUILayerTouchable(true)

    next_ui = parm["next_win_name"]
    uimgr.ui_mgr_inst:goto_next_ui(next_ui,parm)
end

function vs_select_stage_ui:do_move(action_list)
    for i,action in pairs(action_list) do 
        local timer = {}
        
        local timer = {func=baseui.do_action, parm=action}
        local starttime = action["starttime"]
        uimgr.ui_mgr_inst:add_timer(starttime, true, timer)      
    end 
end


function vs_select_stage_ui:enter_scene()
   		self=vs_select_stage_inst
		self:enter({mode=self.mode,diff=self.diff})
end

function vs_select_stage_ui:leave_scene()
    --让所有按钮进入不可选择状态,并隐去返回按钮
    for i,v in pairs(self.btn_list) do
            v:SetClickable(false)
            v:SetVisible(false)
            mainscene:removeChild(v, false)
    end
    --next_ui = parm["next_win_name"]
    --uimgr.ui_mgr_inst:goto_next_ui(next_ui,{})	
end


function on_click_random(n,str)
     --vs_select_stage_inst:leave("vs_mode_ui",{map_id=-1})      
     mainscene:PlayVSGame(vs_select_stage_inst.diff,vs_select_stage_inst.mode,-1) 
end

function on_click_level(n,str)
     --vs_select_stage_inst:leave("vs_mode_ui",{map_id=n})    

     mainscene:PlayVSGame(vs_select_stage_inst.diff,vs_select_stage_inst.mode,n) 
end
function on_click_back(n,str)
     vs_select_stage_inst:leave("vs_mode_ui",{map_id=-2})
end
