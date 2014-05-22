module (..., package.seeall)
require "baseui"
require "uimgr"

local help_img = 
{
    [1]="loading.png",
    [2]="shangcheng_bg.png",
    [3]="bg.png",
}




local mainscene = MainScene:GetMainScene()
local game_music = GameMusic:GetGameMusic()

help_ui = {}
function help_ui:new()
    local o = baseui.base_ui:new()
    
    setmetatable(help_ui, baseui.base_ui)
    setmetatable(o, self)
    self.__index = self;
    o.ui_name="help_ui" 
    o.help_bg = nil
    return o
end


help_ui_inst= help_ui:new()
uimgr.ui_mgr_inst:register(help_ui_inst)

function help_ui:init()

    local sprite = FxLuaSprite:new()
    sprite:CreateFromFile("cover.png")
    pos = CCPoint:new(240,160)
    sprite:SetPosition(pos)
    self.help_bg = sprite    
    self:add_node(self.help_bg)  
 
    
    local btn = FxLuaButton:new()
    pos = CCPoint:new(50,50)
    btn:CreateButton("back_normal.png","back_press.png","back_disable.png", 0, pos, "help_win","on_click_back")
    sprite:AddButton(btn, 10) 
    self.back_btn = btn

    btn = FxLuaButton:new()
    pos = CCPoint:new(400,50)
    btn:CreateButton("ok_normal.png","ok_press.png","ok_normal.png", 0, pos, "help_win","on_click_next")
    sprite:AddButton(btn, 10) 
    self.next_btn = btn

    btn = FxLuaButton:new()
    pos = CCPoint:new(400,250)
    btn:CreateButton("exit_normal.png","exit_press.png","exit_normal.png", 0, pos, "help_win","on_click_prev")
    sprite:AddButton(btn, 10) 
    self.prev_btn = btn


    self.help_bg:SetVisible(false) 
   
    self.page_number = 0
    self.cur_page = nil
 end

function help_ui:enter(parm)
    game_music:playMusic(music_main_bg)


    self.help_bg:SetVisible(true)
    mainscene:AddSprite(self.help_bg,60)
 
   local sprite = FxLuaSprite:new()
   sprite:CreateFromFile(help_img[1])
   sprite:SetTurnBackCBFunc("help_win","on_turn_page_over")     
   pos = CCPoint:new(240,160)
   sprite:SetPosition(pos)    
   self.page_number = 1
   self.cur_page = sprite
   self.help_bg:AddSprite(sprite, 5)
   self:show_btn()
    
    
end

function help_ui:leave(target, parm)
   self:show(false)
   self:remove_from_scene()   
   uimgr.ui_mgr_inst:goto_next_ui(target,parm) 
end
function help_ui:show_btn()
        self.prev_btn:SetVisible(true)
        self.prev_btn:SetClickable(true)
        self.next_btn:SetVisible(true)
        self.next_btn:SetClickable(true)
   
     if self.page_number == 1  then
        self.prev_btn:SetVisible(false)
        self.prev_btn:SetClickable(false)
     end
    
    if self.page_number == table.getn(help_img)  then
        self.next_btn:SetVisible(false)
        self.next_btn:SetClickable(false)
    end
    
    local parent_sprite = self.help_bg
    self.back_btn:removeFromParentAndCleanup(false)
    self.prev_btn:removeFromParentAndCleanup(false)
    self.next_btn:removeFromParentAndCleanup(false)

    if self.cur_page then
        parent_sprite = self.cur_page       
    end
    parent_sprite:AddButton(self.back_btn, 10)
    parent_sprite:AddButton(self.prev_btn, 10)    
    parent_sprite:AddButton(self.next_btn, 10)           
end  
    
function on_click_back()
    help_ui_inst:leave("main_ui", {})
end

function on_click_next()
    --help_ui_inst:leave("main_ui", {})
    self = help_ui_inst
    if self.page_number < table.getn(help_img)   then
        self.page_number = self.page_number + 1
        local sprite = FxLuaSprite:new()
        sprite:CreateFromFile(help_img[self.page_number])
        sprite:SetTurnBackCBFunc("help_win","on_turn_page_over")          
        pos = CCPoint:new(240,160)
        sprite:SetPosition(pos)    
        self.next_page = sprite
        self.help_bg:ReorderChild(self.cur_page, 5)
        self.help_bg:AddSprite(sprite, 4)
    
        self.cur_page:TurnNextPage()
        --self.cur_page = self.next_page
        --self.next_page:removeFromParentAndCleanup(true)
        --self.next_page = nil        
    end    
    --self:show_btn()
end




function on_click_prev()
    --help_ui_inst:leave("main_ui", {})
    self = help_ui_inst
    if self.page_number > 1  then
        self.page_number = self.page_number - 1
        local sprite = FxLuaSprite:new()
        sprite:CreateFromFile(help_img[self.page_number])
        sprite:SetTurnBackCBFunc("help_win","on_turn_page_over")          
        pos = CCPoint:new(240,160)
        sprite:SetPosition(pos)
    
        self.next_page = sprite
        self.help_bg:ReorderChild(self.cur_page, 4)
        self.help_bg:AddSprite(sprite, 5)
        self.next_page:TurnPrevPage()
        --self.cur_page = self.next_page
        
        --self.next_page:removeFromParentAndCleanup(true)
        --self.next_page = nil        
    end    
    --self:show_btn()
end

function  on_turn_page_over()
    self=help_ui_inst
        self.cur_page:removeFromParentAndCleanup(true)
        self.cur_page = self.next_page
        self:show_btn()
end