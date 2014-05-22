module (..., package.seeall)
require "baseui"
require "uimgr"

local cartoon_img = 
{
    [1]="manhua1.png",
    [2]="manhua2.png",
}




local mainscene = MainScene:GetMainScene()
local game_music = GameMusic:GetGameMusic()

cartoon_ui = {}
function cartoon_ui:new()
    local o = baseui.base_ui:new()
    
    setmetatable(cartoon_ui, baseui.base_ui)
    setmetatable(o, self)
    self.__index = self;
    o.ui_name="cartoon_ui" 
    o.cartoon_bg = nil
    o.page_number = 1
    return o
end


cartoon_ui_inst= cartoon_ui:new()
uimgr.ui_mgr_inst:register(cartoon_ui_inst)

function cartoon_ui:init()

    local sprite = FxLuaSprite:new()
    sprite:CreateFromFile("cover.png")
    pos = CCPoint:new(240,160)
    sprite:SetPosition(pos)
    self.cartoon_bg = sprite    
 
end

function cartoon_ui:enter(parm)
    mainscene:SetUILayerTouchable(true)
    game_music:playMusic(music_main_bg)
    self.cartoon_bg:SetVisible(true)
    mainscene:AddSprite(self.cartoon_bg,60)

   local sprite = FxLuaSprite:new()
   sprite:CreateFromFile(cartoon_img[self.page_number])
   self.page_number = self.page_number + 1
   pos = CCPoint:new(240,160)
   sprite:SetTurnBackCBFunc("cartoon_win","on_turn_page_over")        
   sprite:SetPosition(pos)    
   self.cur_page = sprite
   self.cartoon_bg:AddSprite(self.cur_page ,80)
    
end

function cartoon_ui:leave(target, parm)
    self.page_number = 1
    mainscene:SetIsCartoonPlayed()
    mainscene:removeChild(self.cartoon_bg, true)
    uimgr.ui_mgr_inst:goto_next_ui("select_scene_ui",parm) 
end


function cartoon_ui:touch_end()
        if self.page_number == (table.getn(cartoon_img)  + 1) then
            --self:leave(nil,nil)
            self.page_number = self.page_number + 1
            self.cur_page:TurnNextPage()
            return
        end
        local sprite = FxLuaSprite:new()
        sprite:CreateFromFile(cartoon_img[self.page_number])
        self.page_number = self.page_number + 1
        sprite:SetTurnBackCBFunc("cartoon_win","on_turn_page_over")          
        pos = CCPoint:new(240,160)
        sprite:SetPosition(pos)    
        self.next_page = sprite
        self.cartoon_bg:AddSprite(sprite, 4)
        self.cartoon_bg:ReorderChild(self.cur_page, 5)
        self.cur_page:TurnNextPage()

end

    
function  on_turn_page_over()
        self=cartoon_ui_inst
        if self.page_number == (table.getn(cartoon_img)  + 2) then
            self:leave(nil,nil)
        end


        self.cur_page:removeFromParentAndCleanup(true)
        self.cur_page = self.next_page

        
end    
