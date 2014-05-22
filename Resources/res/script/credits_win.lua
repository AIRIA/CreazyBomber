module (..., package.seeall)
require "baseui"
require "uimgr"


local mainscene = MainScene:GetMainScene()
local game_music = GameMusic:GetGameMusic()


credits_ui = {}
function credits_ui:new()
    local o = baseui.base_ui:new()
    setmetatable(credits_ui, baseui.base_ui)
    setmetatable(o, self)
    self.__index = self;
    o.ui_name="credits_ui" 
    o.cover = nil
    o.back_btn = nil
    self.credit_height = 600
    self.interval_prev_next = 240    
    self.prev_credit_y = 0

    
    self.prev_credit = nil
    self.next_credit = nil
    self.pause_flag = false
    return o
end


credits_ui_inst= credits_ui:new()
uimgr.ui_mgr_inst:register(credits_ui_inst)

function credits_ui:init()

    local sprite = FxLuaSprite:new()
    sprite:CreateFromFile("cover.png")
    pos = CCPoint:new(240,160)
    sprite:SetPosition(pos)
    self.cover = sprite    
    
    --加入名单底图
    sprite = FxLuaSprite:new()
    sprite:CreateFromFile("credit_bg.png")
    sprite:SetPosition(CCPoint:new(371,160))
    self.credit_bg = sprite
    self.cover:AddSprite(self.credit_bg,0 )
    
    --add credits
    sprite = FxLuaSprite:new()
    sprite:CreateFromFile("credit.png")
    sprite:setAnchorPoint(CCPoint:new(0.5, 1.0))
    sprite:SetPosition(CCPoint:new(106,self.prev_credit_y ))
    self.prev_credit = sprite
    self.credit_bg:AddSprite(self.prev_credit, 0)

    --add credits
    sprite = FxLuaSprite:new()
    sprite:CreateFromFile("credit.png")
    sprite:setAnchorPoint(CCPoint:new(0.5, 1.0))
    sprite:SetPosition(CCPoint:new(106,self.prev_credit_y  + self.credit_height + self.interval_prev_next))
    self.next_credit = sprite
    self.credit_bg:AddSprite(self.next_credit, 0)

 
    local btn = FxLuaButton:new()
    pos = CCPoint:new(106/2,85/2)
    btn:CreateButton("exit_normal.png","exit_press.png","exit_normal.png", 0, pos, "credits_win","on_click_back")
    self.cover:AddButton(btn, 0) 
    self.back_btn = btn
   
    self.cover:SetVisible(false)    
end

function credits_ui:enter(parm)
    self.pause_flag = false
    game_music:playMusic(music_ui_credit)
    self.cover:SetVisible(true)    
    self.prev_credit_y  = 0
    self:move_up(0)
    mainscene:AddSprite(self.cover,60)
    mainscene:SetUILayerTouchable(true)    
end

function credits_ui:leave(target, parm)
    mainscene:SetUILayerTouchable(false)
   self.pause_flag = true
   self.cover:SetVisible(false)    
   self.cover:removeFromParentAndCleanup(false)
   uimgr.ui_mgr_inst:goto_next_ui(target,parm) 
end


function credits_ui:move_up(dis)
    if self.pause_flag then
        return 
     end
     
    self.prev_credit_y = self.prev_credit_y + dis
    self.prev_credit:SetPosition(CCPoint:new(106, self.prev_credit_y))
    self.next_credit:SetPosition(CCPoint:new(106, self.prev_credit_y - self.credit_height - self.interval_prev_next))
    
    --上一张已经超过界限    
    if self.prev_credit_y > self.credit_height + 320 then
     local credit = self.prev_credit
     self.prev_credit = self.next_credit
     self.prev_credit_y = self.prev_credit_y  - self.credit_height - self.interval_prev_next
     self.next_credit = credit
     self.next_credit:SetPosition(CCPoint:new(106, self.prev_credit_y - self.credit_height - self.interval_prev_next))
      end
end



function credits_ui:touch_begin(parm)
    self.pause_flag = true
end

function credits_ui:touch_end(parm)
    self.pause_flag = false
end

function credits_ui:update(elapse)
    self:move_up(elapse * 50 / 1000)
end

function  on_click_back()
    credits_ui_inst:leave("main_ui", {})
end

