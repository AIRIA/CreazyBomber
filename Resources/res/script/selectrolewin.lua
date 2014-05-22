module (..., package.seeall)
require "baseui"
require "uimgr"

local mainscene = MainScene:GetMainScene()
local game_music = GameMusic:GetGameMusic()

select_role_ui = {}
function select_role_ui:new()
    local o = baseui.base_ui:new()

    setmetatable(select_role_ui, baseui.base_ui)
    setmetatable(o, self)
    self.__index = self;
    o.ui_name="select_role_ui"
    o.select_role = Role_P1_Zombie
    o.roles={}
	return o
end


select_role_inst= select_role_ui:new()
uimgr.ui_mgr_inst:register(select_role_inst)



local option_info =
{
   zombie = {normal="selectrole_zombie_normal.png",press="selectrole_zombie_press.png",disable="selectrole_zombie_press.png",order=45, pos={x=390/2,y=247/2}, cbfunc="on_click_select_role", val=Role_P1_Zombie},
   smurf = {normal="selectrole_smurf_normal.png",press="selectrole_smurf_press.png",disable="selectrole_smurf_press.png",order=45, pos={x=573/2,y=431/2}, cbfunc="on_click_select_role", val=Role_P2_Smurf},
   viking = {normal="selectrole_viking_normal.png",press="selectrole_viking_press.png",disable="selectrole_viking_press.png",order=45, pos={x=816/2,y=315/2}, cbfunc="on_click_select_role", val=Role_P3_Viking},
   vampire = {normal="selectrole_vampire_normal.png",press="selectrole_vampire_press.png",disable="selectrole_vampire_press.png",order=45, pos={x=167/2,y=351/2}, cbfunc="on_click_select_role", val=Role_P4_Vampire},
   ok = {normal="back_normal.png",press="back_press.png",disable="back_normal.png",order=55, pos={x=106/2,y=85/2}, cbfunc="on_click_ok"},
}



function select_role_ui:init()

    for i,v in pairs(option_info) do
    local btn = FxLuaButton:new()
    local pos = CCPoint:new(v["pos"]["x"], v["pos"]["y"])
	
    btn:CreateButton(v["normal"],v["press"],v["disable"],v["order"], pos, "selectrolewin",v["cbfunc"])
	if option_info[i]["val"] then

		btn:SetInt(v["val"])
		self.roles[v["val"]] = btn
	end
    select_role_inst:add_node(btn)
    select_role_inst:show(false)

    end
end

function select_role_ui:enter(parm)
    mainscene:SetUILayerTouchable(false)
    game_music:playMusic(music_main_bg)

    select_role_inst:show(true)
    for i,v in pairs(select_role_inst.nodes) do
        order= v:GetOrderInParent()
        mainscene:AddButton(v, order)
    end
	local selected = mainscene:GetCurRole()
	if selected < Role_None or selected >Role_P4_Vampire then
		selected = Role_P1_Zombie
	end	
	update_select(selected)
    mainscene:SetUILayerTouchable(true)

end

function select_role_ui:leave(target, parm)
    mainscene:SetUILayerTouchable(false)
   select_role_inst:show(false)
   select_role_inst:remove_from_scene()
   uimgr.ui_mgr_inst:goto_next_ui("main_ui",parm)
end

function  update_select(selected)
    
	for i,v in pairs(select_role_inst.roles) do
		if selected == i then
			v:SetSelected(true)
		else
			v:SetSelected(false)
		end

	end

end

function on_click_select_role(n,str)
    select_role_inst.select_role = n
	update_select(select_role_inst.select_role)
    mainscene:SetCurRole(select_role_inst.select_role)
    select_role_inst:leave("main_ui",{role=select_role_inst.select_role})		
end

function on_click_ok(n,str)
    mainscene:SetCurRole(select_role_inst.select_role)
    select_role_inst:leave("main_ui",{role=select_role_inst.select_role})
end

