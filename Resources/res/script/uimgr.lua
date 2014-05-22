module (..., package.seeall)
function  init()
    ui_mgr_inst:init()
end


function update(elapse)
    ui_mgr_inst:update(elapse);
end

function enter_scene()
    ui_mgr_inst:enter_scene();
end

function leave_scene()
    ui_mgr_inst:leave_scene();
end

function touch_begin()
    ui_mgr_inst:touch_begin();
end

function touch_end()
    ui_mgr_inst:touch_end();
end


function touch_move()
    --ui_mgr_inst:touch_end();
end

function touch_cancel()
    --ui_mgr_inst:touch_end();
end




ui_mgr = {}

function ui_mgr:new ()
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    o.wins={}
    o.active_ui = nil
    o.timers={}
    o.game_time = 0
    return o
end

function ui_mgr:update (elapse)
    self.game_time = self.game_time + elapse*1000
    self:update_timers()    
    --[[
    for i,v in pairs(self.wins) do
        if v.update then
           v:update(elapse*1000)
        end   
    end
    --]]
    if self.active_ui then
    self.active_ui:update(elapse*1000)
    end

end

function ui_mgr:add_timer(time,is_relative, func_data)
    local key = time
    if is_relative then
     key = time +  self.game_time
    end 
    table.insert(self.timers,{time=key,data = func_data})
end

function ui_mgr:update_timers()
    for i,v in pairs(self.timers) do 
        if v["time"] < self.game_time then
            print (v["time"])
            local func = v["data"]["func"]
            local parm = v["data"]["parm"]
            --Ö´ÐÐº¯Êý
            func(parm)
            --É¾³ýtimer
            local pos = table.getn(self.timers)
                        
            table.remove(self.timers,i)
        else
            --return
        end
    end
end


function ui_mgr:init ()
    for i,v in pairs(self.wins) do
        if v.init then
           v:init()
        end   
    end
    self.active_ui:enter({})
end

function ui_mgr:register(o)
    self.wins[o.ui_name] = o
end

function ui_mgr:goto_next_ui(target,parm)
    local win = self.wins[target]
    self.active_ui = win
    win:enter(parm)
end

function ui_mgr:enter_scene()
    self.active_ui:enter_scene();
end

function ui_mgr:leave_scene()
    self.active_ui:leave_scene();
end

function ui_mgr:touch_begin()
    self.active_ui:touch_begin();
end

function ui_mgr:touch_end()
    self.active_ui:touch_end();
end


ui_mgr_inst=ui_mgr:new()



