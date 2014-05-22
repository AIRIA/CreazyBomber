module (..., package.seeall)
require "uimgr"
local mainscene = MainScene:GetMainScene()
base_ui={}

function base_ui:new()
    local o = o or {}
    setmetatable(o, self)
    self.__index = self
    o.nodes={}
    o.ui_name="uiname"

return o
end

function base_ui:init()
end

function base_ui:enter(parm)
end

function base_ui:touch_begin(parm)

end
function base_ui:touch_end(parm)

end


function base_ui:leave(target, parm)
end

function base_ui:remove_from_scene()
    for _,v in pairs(self.nodes) do
         mainscene:removeChild(v, false)
    end
end



function base_ui:update()
end

function base_ui:show(b)
    for _,v in pairs(self.nodes) do
      v:SetVisible(b)
      --v:SetClickable(b)
    end
end
function base_ui:add_node(node)
    table.insert(self.nodes,node)
end

function base_ui:enter_scene()
    
end

function base_ui:leave_scene()
    
end



function do_action(action)
    if  action["type"] == "move" then
        do_move_action(action)
    elseif  action["type"]=="shake" then
        do_shake_action(action)
    elseif  action["type"]=="fade" then
    		do_fade_action(action)
    elseif action["type"] == "show"  then
				do_show_action(action)
    elseif action["type"] == "bomb"  then
				do_bomb_action(action)
    elseif action["type"] == "rotate"  then
				do_rotate_action(action)
				
				
    end    
end

function do_move_action(action)
    local parm = action["parm"]
    local offset = CCPoint:new(parm["offset"]["x"], parm["offset"]["y"])
    local pos = CCPoint:new(parm["pos"]["x"], parm["pos"]["y"])
    local sprite = action["sprite"]
    sprite:SetVisible(true)
    sprite:stopAllActions()
    sprite:VibMoveTo(parm["duration"], offset, pos, parm["maxvibamp"],parm["maxvibtime"], parm["vibdir"])

end

 
function do_shake_action(action)
    local parm = action["parm"]
    local pos = CCPoint:new(parm["pos"]["x"], parm["pos"]["y"])
    local sprite = action["sprite"]
    sprite:SetVisible(true)
    sprite:stopAllActions()
    sprite:SetPosition(pos)
    sprite:Shake(parm["time"], parm["delta"], parm["degree"],parm["dir"])
end    


function do_show_action(action)
    local parm = action["parm"]
    local pos = CCPoint:new(parm["pos"]["x"], parm["pos"]["y"])
    local sprite = action["sprite"]
	sprite:SetPosition(pos)
	sprite:SetVisible(true)
end

function do_fade_action(action)
    local parm = action["parm"]
    local o = parm["opacity"]
    local sprite = action["sprite"]    
	sprite:SetVisible(true)
	sprite:SetOpacity(o)
end

function do_bomb_action(action)
    local parm = action["parm"]
    local max_scale = parm["max_scale"]
    local duration = parm["duration"]
    local sprite = action["sprite"]    
	sprite:SetVisible(true)
	sprite:ShowBomb(duration, max_scale)
end

function do_rotate_action(action)
    local id= action["id"]
    local parm = action["parm"]
    local pos = CCPoint:new(parm["pos"]["x"], parm["pos"]["y"])
    local sprite = action["sprite"]
    sprite:SetVisible(true)
    sprite:stopAllActions()
    sprite:SetPosition(pos)

    sprite:Rotate( parm["time"])

end    
