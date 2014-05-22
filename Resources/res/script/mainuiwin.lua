module (..., package.seeall)
require "uimgr"
require "baseui"

local mainscene = MainScene:GetMainScene()
--启动游戏，入场动画时
local enter_animate_time = 200
--点击play按钮后，出现mainui之前的动画效果时间
local leave_animate_time = 250

local mainmenu_btn =
{ 
    story={order=45,pos={x=317,y=123},  rc={91,45,136,41}, cbfunc="on_click_story", next_ui="select_scene_ui",normal="story_normal.png",press="story_press.png",disable="story_normal.png"},
    battle={order=35,pos={x=380/2,y=191}, rc={99,68,125,38},cbfunc="on_click", next_ui="vs_mode_ui",normal="battle_normal.png",press="battle_press.png",disable="battle_disable.png"},
    credit={order=55,pos={x=42,y=42}, cbfunc="on_click", next_ui="credits_ui",normal="credit_normal.png",press="credit_press.png"},
    leadboard={order=55,pos={x=80,y=42},cbfunc="on_leadboard_click", next_ui="setting_ui",normal="leadboard_normal.png",press="leadboard_press.png"},
    setting={order=55,pos={x=118,y=42},cbfunc="on_click", next_ui="setting_ui",normal="setting_normal.png",press="setting_normal.png"},

}

local enter_action=
{
    {id="story", type= "move",starttime=0, parm={duration=50, vibdir=0,offset={x=-500,y=0},pos={x=317,y=123}, maxvibamp=70, maxvibtime=370}},
    {id="battle",type= "move", starttime=77, parm={duration=50, vibdir=0, offset={x=500,y=0},pos={x=380/2,y=191},maxvibamp=70, maxvibtime=430}},
    --{id="shop", type= "move",starttime=300, parm={ duration=200, vibdir=0, offset={x=-500,y=0},pos={x=271,y=246},maxvibamp=40, maxvibtime=150}},
    {id="story", starttime=600,type = "shake",parm={pos={x=317,y=123},dir=1,time=1111, delta=3, degree=0.01}}, 
    {id="battle", starttime=600,type = "shake",parm={pos={x=380/2,y=191},dir=1,time=1333,delta=3,degree=0.01}}, 
    --{id="shop", starttime=700,type = "shake",parm={pos={x=271,y=246},dir=1,time=50,delta=1,degree=0.01}}, 
};

--目前离开动作沿着进来路线退出，计算退出动作
local leave_action = 
{
    {id="story", type= "move",starttime=0, parm={duration=200, vibdir=0,offset={x=500,y=0},pos={x=828,y=114}, maxvibamp=0, maxvibtime=0}},
    {id="battle",type= "move", starttime=0, parm={duration=200, vibdir=0, offset={x=-500,y=0},pos={x=-329,y=186},maxvibamp=0, maxvibtime=0}},
    --{id="shop", type= "move",starttime=400, parm={ duration=200, vibdir=0, offset={x=500,y=0},pos={x=-129,y=246},maxvibamp=0, maxvibtime=0}},
}

local mainscene = MainScene:GetMainScene()
local game_music = GameMusic:GetGameMusic()

main_ui = {}

function main_ui:new()
    local o = baseui.base_ui:new()
    
    setmetatable(main_ui, baseui.base_ui)
    setmetatable(o, self)
    self.__index = self;
    o.ui_name="main_ui" 
    o.animate_list={}
    o.static_list={}
    o.arrow = nil
    return o
end

main_ui_inst =main_ui:new()

uimgr.ui_mgr_inst:register(main_ui_inst)

function main_ui:init()
    --创建除了头像之外的所有按钮
    for i,v in pairs(mainmenu_btn) do 
        local btn = FxLuaButton:new()
        if v["rc"] then        
            local pos = CCPoint:new(v["pos"]["x"], v["pos"]["y"])
            --local btn_pos = CCPoint:new(v["btn_pos"]["x"], v["btn_pos"]["y"])
            --btn:SetPosition(pos);
            local rc = CCRect:new(v.rc[1],v.rc[2],v.rc[3],v.rc[4])
 	        btn:CreateButton(v["normal"], v["press"], v["disable"], v["order"], pos, rc, "mainuiwin",v["cbfunc"])
 	        self.animate_list[i] = btn
        else
            local pos = CCPoint:new(v["pos"]["x"], v["pos"]["y"])
 	        btn:CreateButton(v["normal"], v["press"], v["order"], pos, "mainuiwin",v["cbfunc"])
            self.static_list[i]= btn
        end
        btn:SetVisible(false)
        btn:SetClickable(false)
        btn:SetString(i)
 	   --self:add_node(btn)

    end
    
    self.arrow = FxActionSprite:new("change_role.png", CCPoint:new(388, 36))
    
    --self:add_node(role_btn)	
	--mainscene:AddButton(main_ui_inst.role_btn , 60)	
end

function main_ui:play_enter_action()
    for i,v in pairs(self.animate_list) do 
        --v:SetVisible(true)
        mainscene:AddButton(v, v:GetOrderInParent())
    end
    self:do_move(enter_action)
end

function on_enter_action_over(parm)
    main_ui_inst:create_select_role_btn()
    for i,v in pairs(main_ui_inst.static_list) do 
        v:SetVisible(true)
        v:SetClickable(true)        
        mainscene:AddButton(v, v:GetOrderInParent())
    end

    for i,v in pairs(main_ui_inst.animate_list) do 
        v:SetVisible(true)
        if i == "battle" then
            local isUnlock = mainscene:IsSceneUnlock(SceneCatlog_VS,0)
            v:SetSelected(not isUnlock)
        else
            v:SetClickable(true)        
        end
    end
    mainscene:AddActionSprite(main_ui_inst.arrow,60)
    main_ui_inst.arrow:Action(true)
    mainscene:SetUILayerTouchable(true)
    
end


function main_ui:enter(parm)
    game_music:playMusic(music_main_bg)
    game_music:playEffect(ui_item_in)

    mainscene:SetUILayerTouchable(false)
  	--
    
    self:play_enter_action()

     local timer = {func=on_enter_action_over, parm=p}
     uimgr.ui_mgr_inst:add_timer(enter_animate_time, true, timer)
end


function main_ui:do_move(action_list)
    for i,action in ipairs(action_list) do 
        
        local timer = {}
        
        local timer = {func=baseui.do_action, parm=action}
        local starttime = action["starttime"]
        local id=action["id"]
        local sprite = self.animate_list[id]
        action["sprite"] = sprite
        uimgr.ui_mgr_inst:add_timer(starttime, true, timer)      
    end 
end

function main_ui:leave(target, parm)
    mainscene:SetUILayerTouchable(false)
    game_music:playEffect(ui_item_out);
     if self.static_list["role"] then 
        mainscene:removeChild(self.static_list["role"] ,true)
        self.static_list["role"] = nil
    end
    for i,v in pairs(self.static_list) do 
        v:SetVisible(false)
        --v:SetClickable(false)
        mainscene:removeChild(v,false)
    end     
    
    
    self:play_leave_animate()   
     
    local timer={func=on_leave_animate_over, parm={next_win_name=target}}
    uimgr.ui_mgr_inst:add_timer(leave_animate_time, true, timer)
end


function main_ui:play_leave_animate(next_ui)
    game_music:playEffect(ui_item_out);
    self.arrow:Action(false)    
    self.arrow:removeFromParentAndCleanup(false)
    self:do_move(leave_action)
end

function on_leave_animate_over(parm)
   for i,v in pairs(main_ui_inst.animate_list) do 
        v:SetVisible(false)
        v:SetClickable(false)
        v:stopAllActions()
        mainscene:removeChild(v,false)
    end    
    local next_ui = parm["next_win_name"]
    uimgr.ui_mgr_inst:goto_next_ui(next_ui,{})  
end

function main_ui:create_select_role_btn()
   	local cur_role = mainscene:GetCurRole()
	if cur_role < Role_None or cur_role >Role_P4_Vampire then
		cur_role = Role_P1_Zombie
	end	

	local normalfile = "mainmenu_role_".. RoleName[cur_role].."_normal.png"
	local pressfile = "mainmenu_role_".. RoleName[cur_role].."_press.png"
 
    --创建头像按钮，分别处理
	local role_btn = FxLuaButton:new()
    local pos =  CCPoint:new(446,33)  
	role_btn:CreateButton(normalfile,pressfile, 55, pos, "mainuiwin","OnClickChangeRole")
    role_btn:SetVisible(false)
    role_btn:SetClickable(false)
	self.static_list["role"] = role_btn
end



function on_click(n,str)
    local next_ui = nil
    for i,v in pairs(mainmenu_btn) do
        if i == str then
            next_ui = v["next_ui"]
        end  
    end
	main_ui_inst:leave(next_ui,{})
    
end

function OnClickChangeRole(n,str)
	main_ui_inst:leave("select_role_ui",{})
end

function on_leadboard_click(n,str)
	--调用接口
    showLeaderBoard();
end

    
function on_click_story()
    local next_ui = nil
    local play_cartoon = mainscene:GetIsCartoonPlayed()
     if false == play_cartoon then
        next_ui = "cartoon_ui"     
    else
        next_ui = "select_scene_ui"     
    end 
	main_ui_inst:leave(next_ui,{})    
end
