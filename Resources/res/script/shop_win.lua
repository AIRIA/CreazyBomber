module (..., package.seeall)
require "baseui"
require "uimgr"
require "config"
--感觉不需要考虑用户手动改配置文件问题，直接使用此数据显示和游戏内使用。


local mainscene = MainScene:GetMainScene()
local game_music = GameMusic:GetGameMusic()
local shop = FxLuaShop:GetShop()

local bomb_num_pos =CCPoint:new(224/2,274/2)
local hp_num_pos =CCPoint:new(224/2,275/2)
local gold_sprite_num_pos =CCPoint:new(509/2,77/2)
local gold_num_pos =CCPoint:new(70,20)

local  enter_animate_time = 1000
local  leave_animate_time = 1000

local option_info =
{
   hp = {rc=CCRect:new(18,89,71,72),normal="shop_item_hp_normal.png",press="shop_item_hp_press.png",disable="shop_item_hp_normal.png",order=35, pos={x=355,y=173}, cbfunc="on_click_buy_hp"},
   timer_bomb = {rc=CCRect:new(18,89,71,72),normal="shop_item_timerbomb_normal.png",press="shop_item_timerbomb_press.png",disable="shop_item_timerbomb_normal.png",order=35, pos={x=160,y=175}, cbfunc="on_click_buy_timer_bomb"},
   back_btn = {normal="back_normal.png",press="back_press.png",disable="back_disable.png",order=55, pos={x=106/2,y=85/2},cbfunc="on_click_back"},
   charge_btn = {normal="shop_charge_btn_normal.png",press="shop_charge_btn_press.png",disable="shop_charge_btn_normal.png",order=55, pos={x=831/2,y=63/2},cbfunc="on_click_charge"},              
}
local enter_actions=
{
    {id="hp", starttime=0,type = "move",parm={duration=100,  vibdir=1,offset={x=0,y=200},pos={x=355,y=173},maxvibamp=20, maxvibtime=500}},   
    {id="hp", starttime=650,type = "shake",parm={pos={x=355,y=173},time=800,delta=1,degree=0.01,dir=1}},   

    {id="timer_bomb", starttime=0, type = "move",parm={duration=100,  vibdir=1, offset={x=0,y=200},pos={x=160,y=175},maxvibamp=20, maxvibtime=500}},
    {id="timer_bomb", starttime=650,type = "shake",parm={pos={x=160,y=175},time=800,delta=1,degree=0.01,dir=1}},   
}

local leave_actions=
{
    {id="timer_bomb", starttime=0,type = "move",parm={duration=100,  vibdir=1,offset={x=0,y=-200},pos={x=365,y=-17},maxvibamp=20, maxvibtime=500}},   
    {id="hp", starttime=0, type = "move",parm={duration=100,  vibdir=1, offset={x=0,y=-200},pos={x=160,y=-17},maxvibamp=20, maxvibtime=500}},
}

shop_ui = {}
function shop_ui:new()
    local o = baseui.base_ui:new()
    setmetatable(shop_ui, baseui.base_ui)
    setmetatable(o, self)
    self.__index = self;
    o.ui_name="shop_ui" 
    o.btn_list = {}
    self.updatetime = 0;
    return o
end


shop_ui_inst= shop_ui:new()
uimgr.ui_mgr_inst:register(shop_ui_inst)

function shop_ui:init()
    for i,v in pairs(option_info) do
        local btn = FxLuaButton:new("shop_item_count_font")
        local pos = CCPoint:new(v["pos"]["x"], v["pos"]["y"])
        if v["rc"] then
            btn:CreateButton(v["normal"],v["press"],v["disable"],v["order"], pos, v["rc"],"shop_win",v["cbfunc"])
            --btn:CreateFontStr("shop_item_count_font",CCPoint:new(95/2,369/2))
        else
            btn:CreateButton(v["normal"],v["press"],v["disable"],v["order"], pos, "shop_win",v["cbfunc"])
        end
            
        self:add_node(btn)
        self.btn_list[i] = btn
    end 
    
    for i,v in ipairs(enter_actions) do
        v["sprite"] = self.btn_list[v["id"]]
    end
    for i,v in ipairs(leave_actions) do
        v["sprite"] = self.btn_list[v["id"]]
    end
    
    --
    self.title_sprite = FxLuaSprite:new()
    self.title_sprite:CreateFromFile("shop_ui_title.png")
    pos = CCPoint:new(162/3,582/2)
    self.title_sprite:SetPosition (pos)
    self.title_sprite:SetVisible(false)
    self.coinsnum_sprite = FxLuaSprite:new("shop_item_price_font")
    self.coinsnum_sprite:CreateFromFile("shop_coinsnum.png")
    self.coinsnum_sprite:SetPosition (gold_sprite_num_pos)
    self.coinsnum_sprite:SetVisible(false)
    
    self.coinsprice_sprite = FxLuaSprite:new()
    self.coinsprice_sprite:CreateFromFile("shop_price.png")
    pos = CCPoint:new(830/2,122/2)
    self.coinsprice_sprite:SetPosition (pos)
    self.coinsprice_sprite:SetVisible(false)
    


    shop_ui_inst:show(false)    
end

function shop_ui:update(elapse)
	self.updatetime =  self.updatetime + elapse
	if self.updatetime > 1000 then
        local gold_num = shop:GetCurGold()
        if self.coinsnum_sprite ~= nil then
        	self.coinsnum_sprite:SetNameString(gold_num, gold_num_pos)
        end
	end
end
function shop_ui:enter(parm)
    mainscene:SetUILayerTouchable(false)
    game_music:playMusic(music_main_bg)
    game_music:playEffect(ui_item_in)

    self.mode=parm["mode"]
    self.type=parm["type"]
    local btn = self.btn_list["timer_bomb"]
    btn:SetNameString(shop:GetItemNum(config.item_timer_bomb_id),bomb_num_pos)
    btn = self.btn_list["hp"]
    btn:SetNameString(shop:GetItemNum(config.item_hp_id),hp_num_pos)
    
      --播放按钮出现动画
    self:play_enter_animate()

    --设置播放结束回调
     local timer = {func=on_enter_animate_over, {}}
     uimgr.ui_mgr_inst:add_timer(enter_animate_time, true, timer)  
end

function shop_ui:leave(target, p)
    mainscene:SetUILayerTouchable(false)
    self:play_leave_animate()
     --设置播放结束回调
     local timer = {func=on_leave_animate_over, parm={next_win_name=target,parm=p}}
     uimgr.ui_mgr_inst:add_timer(leave_animate_time, true, timer)
   
end


function shop_ui:play_enter_animate()
    --加入场景
    for i,v in pairs(self.btn_list) do
        if i ~= "charge_btn" and  i ~= "back_btn" then
            v:SetVisible(true)
            mainscene:AddButton(v, v:GetOrderInParent())
        end    
    end
    
    self:do_move(enter_actions)
end


function on_enter_animate_over()
    self = shop_ui_inst
    for i,v in pairs(self.btn_list) do
        if i == "charge_btn" or  i == "back_btn" then
            v:SetVisible(true)
            mainscene:AddButton(v, v:GetOrderInParent())
        end    
        
    end
    
    self.title_sprite:SetVisible(true);
    mainscene:AddSprite(self.title_sprite,60)
    self.coinsnum_sprite:SetVisible(true);
    mainscene:AddSprite(self.coinsnum_sprite,60)
    self.coinsnum_sprite:SetVisible(true)
    self.coinsnum_sprite:SetNameString(shop:GetCurGold(),CCPoint:new(70,20))    
    self.coinsprice_sprite:SetVisible(true)
    mainscene:AddSprite(self.coinsprice_sprite,60)
    mainscene:SetUILayerTouchable(true)

end
function shop_ui:do_move(action_list)
    for i,action in ipairs(action_list) do 
        
        local timer = {}
        
        local timer = {func=baseui.do_action, parm=action}
        local starttime = action["starttime"]
        uimgr.ui_mgr_inst:add_timer(starttime, true, timer)      
    end 
end

function shop_ui:play_leave_animate()
  
    self = shop_ui_inst
     for i,v in pairs(self.btn_list) do
        if i== "charge_btn" or  i== "back_btn" then
            v:SetVisible(false)
            mainscene:removeChild(v, false)
        end
    end
    self.title_sprite:SetVisible(false);
    mainscene:removeChild(self.title_sprite, false)
    self.coinsnum_sprite:SetVisible(false);
    mainscene:removeChild(self.coinsnum_sprite,false)
    mainscene:removeChild(self.coinsprice_sprite,false)

    self:do_move(leave_actions)


end

function on_leave_animate_over(parm)
    --让所有按钮进入不可选择状态,并隐去返回按钮
    self=shop_ui_inst
    for i,v in pairs(self.btn_list) do
        if i~= "charge_btn" and  i~= "back_btn" then
            v:SetVisible(false)
            mainscene:removeChild(v, false)
        end
    end

    next_ui = parm["next_win_name"]
    uimgr.ui_mgr_inst:goto_next_ui(next_ui,parm["parm"])
end

function on_click_back(n,str)
    self=shop_ui_inst
    self:leave("select_stage_ui",{mode=self.mode,type=self.type})
end

function on_click_charge(n,str)
    BuyItem(0);
end


function on_click_buy_hp(n,str)
    shop:BuyItem(config.item_hp_id, "shop_win", "on_click_buy_hp_ret")
    
end

function on_click_buy_hp_ret(id)
    self = shop_ui_inst
    if id == config.item_hp_id then
        local num = shop:GetItemNum(config.item_hp_id)
        local gold_num = shop:GetCurGold()
        self.btn_list["hp"]:SetNameString(num, hp_num_pos)
        self.coinsnum_sprite:SetNameString(gold_num, gold_num_pos)
  end
end

function on_click_buy_timer_bomb(n, str)
    shop:BuyItem(config.item_timer_bomb_id,"shop_win", "on_click_buy_timer_bomb_ret") 
end

function on_click_buy_timer_bomb_ret(id)
    self = shop_ui_inst
    if id == config.item_timer_bomb_id then
        local num = shop:GetItemNum(config.item_timer_bomb_id)
        local gold_num = shop:GetCurGold()
        self.btn_list["timer_bomb"]:SetNameString(num, bomb_num_pos)
        self.coinsnum_sprite:SetNameString(gold_num, gold_num_pos)
  end
end



