module (..., package.seeall)
require "baseui"
require "uimgr"


local mainscene = MainScene:GetMainScene()
local game_music = GameMusic:GetGameMusic()
setting_ui = {}
function setting_ui:new()
    local o = baseui.base_ui:new()
    
    setmetatable(setting_ui, baseui.base_ui)
    setmetatable(o, self)
    self.__index = self;
    o.ui_name="setting_ui" 
    o.music_btn_normal = nil
    o.sound_btn_normal = nil
    o.music_btn_checked = nil
    o.sound_btn_checked = nil

    o.music_flag = true
    o.sound_flag = false
    o.setting_bg = nil
    return o
end


setting_ui_inst= setting_ui:new()
uimgr.ui_mgr_inst:register(setting_ui_inst)

function setting_ui:init()
    
    local sprite = FxLuaSprite:new()
    sprite:CreateFromFile("setting_bg.png")
    local pos = CCPoint:new(487/2, 317/2)
    sprite:SetPosition(pos)
    self.setting_bg = sprite
     
    btn = FxLuaButton:new()
    pos = CCPoint:new(82/2,107/2)
    btn:CreateButton("music_on_normal.png","music_on_press.png","music_on_normal.png", 0, pos, "setting_win","on_click_music")
    --btn:SetSelected(true)
    self.music_btn_checked = btn
    sprite:AddButton(btn, 1)

    btn = FxLuaButton:new()
    pos = CCPoint:new(83/2,107/2)
    btn:CreateButton("music_off_normal.png","music_off_press.png","music_off_normal.png", 0, pos, "setting_win","on_click_music")
    --btn:SetSelected(true)
    self.music_btn_normal = btn
    sprite:AddButton(btn, 1)


    btn = FxLuaButton:new()
    pos = CCPoint:new(474/2,105/2)
    btn:CreateButton("sound_on_normal.png","sound_on_press.png","sound_on_normal.png", 0, pos, "setting_win","on_click_sound")
    --btn:SetSelected(true)
    self.sound_btn_checked = btn
    sprite:AddButton(btn, 1) 
    
     btn = FxLuaButton:new()
    pos = CCPoint:new(474/2,105/2)
    btn:CreateButton("sound_off_normal.png","sound_off_press.png","sound_off_normal.png", 0, pos, "setting_win","on_click_sound")
    --btn:SetSelected(true) 
    self.sound_btn_normal = btn
    sprite:AddButton(btn, 1)    
    
    btn = FxLuaButton:new()
    pos = CCPoint:new(276/2,290/2)
    btn:CreateButton("white_exit_normal.png","white_exit_press.png","white_exit_normal.png", 0, pos, "setting_win","on_click_ok")
    sprite:AddButton(btn, 0) 
   
    self.setting_bg:SetVisible(false)    
end

function setting_ui:enter(parm)
    mainscene:SetUILayerTouchable(false)
    game_music:playMusic(music_main_bg)
    self.music_flag = game_music:GetMusicFlag()
    self.sound_flag = game_music:GetEffectFlag()
    mainscene:AddSprite(self.setting_bg,60)
    self.setting_bg:SetVisible(true)    
    self:update_ui();
    mainscene:SetUILayerTouchable(true)
end

function setting_ui:leave(target, parm)
   mainscene:SetUILayerTouchable(false)
   game_music:SetMusicFlag(self.music_flag)
   game_music:SetEffectFlag(self.sound_flag)
   self.setting_bg:SetVisible(false)    
   --self:remove_from_scene()   
   self.setting_bg:removeFromParentAndCleanup(false)
   uimgr.ui_mgr_inst:goto_next_ui(target,parm) 
end

function setting_ui:update_ui()
        self.music_btn_normal:SetVisible(not self.music_flag)
        self.music_btn_checked:SetVisible(self.music_flag)
        self.sound_btn_normal:SetVisible(not self.sound_flag)
        self.sound_btn_checked:SetVisible(self.sound_flag)
end

function on_click_music(n,str)
    setting_ui_inst.music_flag = not setting_ui_inst.music_flag;
    game_music:SetMusicFlag(setting_ui_inst.music_flag)
    setting_ui_inst:update_ui();
end

function on_click_sound(n,str)
    setting_ui_inst.sound_flag = not setting_ui_inst.sound_flag
    game_music:SetEffectFlag(setting_ui_inst.sound_flag)
    setting_ui_inst:update_ui();
end

function on_click_ok(n,str)
    setting_ui_inst:leave("main_ui", {})
end
