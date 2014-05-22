module (..., package.seeall)
require "baseui"
require "uimgr"

local mainscene = MainScene:GetMainScene()
local game_music = GameMusic:GetGameMusic()

local  state_info = 
{
    
    --主线关卡
    [0] =       
    {
        --丛林
        [0] =
        {
            [0]={basefile="cl_stage1",pos={357/2,142/2},  order=45, val=1, angle=0, clickrc={16,32,49,47}, name_pos={96/2,80/2},star1pos={43,55},star2pos={},star2pos={}},
            [1]={basefile="cl_stage1",pos={519/2,207/2}  ,order=45, val=1, angle=0, clickrc={16,32,49,47}, name_pos={100/2,85/2},star1pos={43,55},star2pos={},star2pos={}},
            [2]={basefile="cl_stage1",pos={696/2,231/2}  ,order=45, val=1, angle=0, clickrc={16,32,49,47}, name_pos={100/2,85/2},star1pos={43,55},star2pos={},star2pos={}},
            [3]={basefile="cl_stage2",pos={838/2,273/2}  ,order=35, val=1, angle=0, clickrc={16,32,49,47}, name_pos={83/2,85/2},star1pos={43,55},star2pos={},star2pos={}},
            [4]={basefile="cl_stage2",pos={646/2,334/2}  ,order=35, val=1, angle=0, clickrc={16,32,49,47}, name_pos={78/2,90/2},star1pos={43,55},star2pos={},star2pos={}},
            [5]={basefile="cl_stage2",pos={480/2,357/2}  ,order=35, val=1, angle=0, clickrc={16,32,49,47}, name_pos={80/2,88/2},star1pos={43,55},star2pos={},star2pos={}},
            [6]={basefile="cl_stage2",pos={312/2,359/2}  ,order=35, val=1, angle=0, clickrc={16,32,49,47}, name_pos={78/2,84/2},star1pos={43,55},star2pos={},star2pos={}},
            [7]={basefile="cl_stage2",pos={136/2,352/2}  ,order=35, val=1, angle=0, clickrc={16,32,49,47}, name_pos={80/2,84/2},star1pos={43,55},star2pos={},star2pos={}},
            [8]={basefile="cl_stage1",pos={199/2,469/2}  ,order=25, val=1, angle=0, clickrc={16,32,49,47}, name_pos={98/2,78/2},star1pos={43,55},star2pos={},star2pos={}},
            [9]={basefile="cl_stage1",pos={394/2,489/2}  ,order=25, val=1, angle=0, clickrc={16,32,49,47}, name_pos={96/2, 78/2},star1pos={43,55},star2pos={},star2pos={}},
            [10]={basefile="cl_stage1",pos={587/2,495/2}  ,order=25, val=1, angle=0, clickrc={16,32,49,47}, name_pos={94/2,82/2},star1pos={43,55},star2pos={},star2pos={}},
            [11]={basefile="cl_stage1",pos={786/2,483/2}  ,order=25, val=1, angle=0, clickrc={16,32,49,47}, name_pos={96/2,82/2},star1pos={43,55},star2pos={},star2pos={}} ,
        },
 

        
        [1]=
        {
            [0]={basefile="md_stage1",pos={383/2,135/2},  order=45, val=1, angle=0, clickrc={19,35,60,50}, name_pos={72/2,92/2},star1pos={43,55},star2pos={},star2pos={}},
            [1]={basefile="md_stage1",pos={549/2,200/2}  ,order=45, val=1, angle=0, clickrc={19,35,60,50}, name_pos={78/2,100/2},star1pos={43,55},star2pos={},star2pos={}},
            [2]={basefile="md_stage1",pos={731/2,223/2}  ,order=45, val=1, angle=0, clickrc={19,35,60,50}, name_pos={75/2,103/2},star1pos={43,55},star2pos={},star2pos={}},
            [3]={basefile="md_stage2",pos={828/2,263/2}  ,order=35, val=1, angle=0, clickrc={19,35,60,50}, name_pos={95/2,99/2},star1pos={43,55},star2pos={},star2pos={}},
            [4]={basefile="md_stage2",pos={630/2,326/2}  ,order=35, val=1, angle=0, clickrc={19,35,60,50}, name_pos={92/2,98/2},star1pos={43,55},star2pos={},star2pos={}},
            [5]={basefile="md_stage2",pos={463/2,347/2}  ,order=35, val=1, angle=0, clickrc={19,35,60,50}, name_pos={94/2,101/2},star1pos={43,55},star2pos={},star2pos={}},
            [6]={basefile="md_stage2",pos={285/2,340/2}  ,order=35, val=1, angle=0, clickrc={19,35,60,50}, name_pos={93/2,102/2},star1pos={43,55},star2pos={},star2pos={}},
            [7]={basefile="md_stage2",pos={100/2,326/2}  ,order=35, val=1, angle=0, clickrc={19,35,60,50}, name_pos={94/2,105/2},star1pos={43,55},star2pos={},star2pos={}},
            [8]={basefile="md_stage1",pos={220/2,451/2}  ,order=25, val=1, angle=0, clickrc={19,35,60,50}, name_pos={78/2,95/2},star1pos={43,55},star2pos={},star2pos={}},
            [9]={basefile="md_stage1",pos={411/2,467/2}  ,order=25, val=1, angle=0, clickrc={19,35,60,50}, name_pos={72/2,96/2},star1pos={43,55},star2pos={},star2pos={}},
            [10]={basefile="md_stage1",pos={616/2,474/2}  ,order=25, val=1, angle=0, clickrc={19,35,60,50}, name_pos={72/2,100/2},star1pos={43,55},star2pos={},star2pos={}},
            [11]={basefile="md_stage1",pos={819/2,460/2}  ,order=25, val=1, angle=0, clickrc={19,35,60,50}, name_pos={78/2,105/2},star1pos={43,55},star2pos={},star2pos={}} ,
        },

        [2]=
        {
            [0]={basefile="bc_stage1",pos={378/2,126/2},  order=45, val=1, angle=0, clickrc={10,34,71,37}, name_pos={78/2,100/2},star1pos={43,55},star2pos={},star2pos={}},
            [1]={basefile="bc_stage1",pos={543/2,198/2}  ,order=45, val=1, angle=0, clickrc={10,34,71,37}, name_pos={80/2,105/2},star1pos={43,55},star2pos={},star2pos={}},
            [2]={basefile="bc_stage1",pos={713/2,222/2}  ,order=45, val=1, angle=0, clickrc={10,34,71,37}, name_pos={82/2,103/2},star1pos={43,55},star2pos={},star2pos={}},
            [3]={basefile="bc_stage2",pos={842/2,263/2}  ,order=35, val=1, angle=0, clickrc={10,34,71,37}, name_pos={85/2,105/2},star1pos={43,55},star2pos={},star2pos={}},
            [4]={basefile="bc_stage2",pos={636/2,324/2}  ,order=35, val=1, angle=0, clickrc={10,34,71,37}, name_pos={80/2,107/2},star1pos={43,55},star2pos={},star2pos={}},
            [5]={basefile="bc_stage2",pos={467/2,347/2}  ,order=35, val=1, angle=0, clickrc={10,34,71,37}, name_pos={84/2,106/2},star1pos={43,55},star2pos={},star2pos={}},
            [6]={basefile="bc_stage2",pos={301/2,348/2}  ,order=35, val=1, angle=0, clickrc={10,34,71,37}, name_pos={82/2,104/2},star1pos={43,55},star2pos={},star2pos={}},
            [7]={basefile="bc_stage2",pos={114/2,342/2}  ,order=35, val=1, angle=0, clickrc={10,34,71,37}, name_pos={88/2,104/2},star1pos={43,55},star2pos={},star2pos={}},
            [8]={basefile="bc_stage1",pos={221/2,461/2}  ,order=25, val=1, angle=0, clickrc={10,34,71,37}, name_pos={80/2,98/2},star1pos={43,55},star2pos={},star2pos={}},
            [9]={basefile="bc_stage1",pos={406/2,476/2}  ,order=25, val=1, angle=0, clickrc={10,34,71,37}, name_pos={78/2,100/2},star1pos={43,55},star2pos={},star2pos={}},
            [10]={basefile="bc_stage1",pos={608/2,483/2}  ,order=25, val=1, angle=0, clickrc={10,34,71,37}, name_pos={76/2,104/2},star1pos={43,55},star2pos={},star2pos={}},
            [11]={basefile="bc_stage1",pos={802/2,473/2}  ,order=25, val=1, angle=0, clickrc={10,34,71,37}, name_pos={82/2,104/2},star1pos={43,55},star2pos={},star2pos={}},
        },
        
     },
    --支线关卡
   [1] =
    {   
       [0]=
        {  
            [0]={basefile="cl_stage1",pos={357/2,142/2},  order=45, val=1, angle=0, clickrc={16,32,49,47}, name_pos={96/2,80/2},star1pos={43,55},star2pos={},star2pos={}},
            [1]={basefile="cl_stage1",pos={519/2,207/2}  ,order=45, val=1, angle=0, clickrc={16,32,49,47}, name_pos={100/2,85/2},star1pos={43,55},star2pos={},star2pos={}},
            [2]={basefile="cl_stage2",pos={838/2,273/2}  ,order=35, val=1, angle=0, clickrc={16,32,49,47}, name_pos={83/2,85/2},star1pos={43,55},star2pos={},star2pos={}},
            [3]={basefile="cl_stage2",pos={646/2,334/2}  ,order=35, val=1, angle=0, clickrc={16,32,49,47}, name_pos={78/2,90/2},star1pos={43,55},star2pos={},star2pos={}},
            [4]={basefile="cl_stage2",pos={480/2,357/2}  ,order=35, val=1, angle=0, clickrc={16,32,49,47}, name_pos={80/2,88/2},star1pos={43,55},star2pos={},star2pos={}},
            [5]={basefile="cl_stage1",pos={199/2,469/2}  ,order=25, val=1, angle=0, clickrc={16,32,49,47}, name_pos={100/2,78/2},star1pos={43,55},star2pos={},star2pos={}},
            [6]={basefile="cl_stage1",pos={394/2,489/2}  ,order=25, val=1, angle=0, clickrc={16,32,49,47}, name_pos={98/2, 78/2},star1pos={43,55},star2pos={},star2pos={}},
            [7]={basefile="cl_stage1",pos={587/2,495/2}  ,order=25, val=1, angle=0, clickrc={16,32,49,47}, name_pos={98/2,82/2},star1pos={43,55},star2pos={},star2pos={}},
        },
       [1]=
        {  
            [0]={basefile="md_stage1",pos={383/2,135/2},  order=45, val=1, angle=0, clickrc={19,35,60,50}, name_pos={72/2,92/2},star1pos={43,55},star2pos={},star2pos={}},
            [1]={basefile="md_stage1",pos={549/2,200/2}  ,order=45, val=1, angle=0, clickrc={19,35,60,50}, name_pos={76/2,100/2},star1pos={43,55},star2pos={},star2pos={}},
            [2]={basefile="md_stage2",pos={828/2,263/2}  ,order=35, val=1, angle=0, clickrc={19,35,60,50}, name_pos={100/2,99/2},star1pos={43,55},star2pos={},star2pos={}},
            [3]={basefile="md_stage2",pos={630/2,326/2}  ,order=35, val=1, angle=0, clickrc={19,35,60,50}, name_pos={86/2,98/2},star1pos={43,55},star2pos={},star2pos={}},
            [4]={basefile="md_stage2",pos={463/2,347/2}  ,order=35, val=1, angle=0, clickrc={19,35,60,50}, name_pos={91/2,101/2},star1pos={43,55},star2pos={},star2pos={}},
            [5]={basefile="md_stage1",pos={220/2,451/2}  ,order=25, val=1, angle=0, clickrc={19,35,60,50}, name_pos={75/2,100/2},star1pos={43,55},star2pos={},star2pos={}},
            [6]={basefile="md_stage1",pos={411/2,467/2}  ,order=25, val=1, angle=0, clickrc={19,35,60,50}, name_pos={74/2,102/2},star1pos={43,55},star2pos={},star2pos={}},
            [7]={basefile="md_stage1",pos={616/2,474/2}  ,order=25, val=1, angle=0, clickrc={19,35,60,50}, name_pos={78/2,110/2},star1pos={43,55},star2pos={},star2pos={}},
        },
       [2]=
        {  
            [0]={basefile="bc_stage1",pos={378/2,126/2},  order=45, val=1, angle=0, clickrc={10,34,71,37}, name_pos={78/2,102/2},star1pos={43,55},star2pos={},star2pos={}},
            [1]={basefile="bc_stage1",pos={543/2,198/2}  ,order=45, val=1, angle=0, clickrc={10,34,71,37}, name_pos={84/2,104/2},star1pos={43,55},star2pos={},star2pos={}},
            [2]={basefile="bc_stage2",pos={842/2,263/2}  ,order=35, val=1, angle=0, clickrc={10,34,71,37}, name_pos={85/2,104/2},star1pos={43,55},star2pos={},star2pos={}},
            [3]={basefile="bc_stage2",pos={636/2,342/2}  ,order=35, val=1, angle=0, clickrc={10,34,71,37}, name_pos={84/2,105/2},star1pos={43,55},star2pos={},star2pos={}},
            [4]={basefile="bc_stage2",pos={467/2,347/2}  ,order=35, val=1, angle=0, clickrc={10,34,71,37}, name_pos={82/2,105/2},star1pos={43,55},star2pos={},star2pos={}},
            [5]={basefile="bc_stage1",pos={221/2,461/2}  ,order=25, val=1, angle=0, clickrc={10,34,71,37}, name_pos={88/2,100/2},star1pos={43,55},star2pos={},star2pos={}},
            [6]={basefile="bc_stage1",pos={406/2,476/2}  ,order=25, val=1, angle=0, clickrc={10,34,71,37}, name_pos={80/2,100/2},star1pos={43,55},star2pos={},star2pos={}},
            [7]={basefile="bc_stage1",pos={608/2,483/2}  ,order=25, val=1, angle=0, clickrc={10,34,71,37}, name_pos={84/2,104/2},star1pos={43,55},star2pos={},star2pos={}},
        },
   },
}


local actions = 
{     
    [0]=
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
       },
       [1] =
       {
        enter_animate_time = 500,
        leave_animate_time = 500,
       
        enter=
        {  
                [0]={ type= "move",starttime=0,   parm={duration=50, vibdir=0,offset={x=0,y=100},maxvibamp=70, maxvibtime=370}},
                [1]={ type= "move",starttime=50,  parm={duration=50, vibdir=0,offset={x=0,y=100},maxvibamp=70, maxvibtime=370}},
                [2]={ type= "move",starttime=100, parm={duration=50, vibdir=0,offset={x=0,y=100},maxvibamp=70, maxvibtime=370}},
                [3]={ type= "move",starttime=150, parm={duration=50, vibdir=0,offset={x=0,y=100},maxvibamp=70, maxvibtime=370}},
                [4]={ type= "move",starttime=200, parm={duration=50, vibdir=0,offset={x=0,y=100},maxvibamp=70, maxvibtime=370}},
                [5]={ type= "move",starttime=250, parm={duration=50, vibdir=0,offset={x=0,y=100},maxvibamp=70, maxvibtime=370}},
                [6]={ type= "move",starttime=300, parm={duration=50, vibdir=0,offset={x=0,y=100},maxvibamp=70, maxvibtime=370}},
                [7]={ type= "move",starttime=350, parm={duration=50, vibdir=0,offset={x=0,y=100},maxvibamp=70, maxvibtime=370}},
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
            },
       }
}

local leave_acitons = {}

select_stage_ui = {}
function select_stage_ui:new()
    local o = baseui.base_ui:new()
    setmetatable(select_stage_ui, baseui.base_ui)
    setmetatable(o, self)
    self.__index = self;
    o.ui_name="select_stage_ui"
    o.select_map_id = 0
    o.all_ui_infos={}
    return o
end

function select_stage_ui:init()
    local template={}
    local prefix={}
    local rc=nil
    for i=SceneCatlog_Main, SceneCatlog_Branch do
        self.all_ui_infos[i] = {}
        for j=SceneArt_CaoDi, SceneArt_Size - 1 do
             self.all_ui_infos[i][j] = {}

             self.all_ui_infos[i][j]["mode"] = i
             self.all_ui_infos[i][j]["scenetype"] = j
            --按钮列表初始化
             self.all_ui_infos[i][j]["btn_list"] = {}
            --进入动作初始化
             self.all_ui_infos[i][j]["enter_actions"] = {}
             --离开动作初始化
             self.all_ui_infos[i][j]["leave_actions"] = {}
             self.all_ui_infos[i][j]["shake_actions"] = {}
             self:init_btn(state_info[i][j],self.all_ui_infos[i][j],actions[i],actions[i]["enter_animate_time"],actions[i]["leave_animate_time"])
        end
    end
end

function select_stage_ui:init_btn(template, btn_info,action,enter_animate_time, leave_animate_time)
    --创建关卡按钮
    for i,v in pairs(template) do
        local btn = FxLuaButton:new("stage_select_num")
        local pos = CCPoint:new(v["pos"][1], v["pos"][2])
        local rc = CCRect:new(v["clickrc"][1],v["clickrc"][2],v["clickrc"][3],v["clickrc"][4])
        local basefile = v["basefile"]
        btn:CreateButton(basefile.."_normal.png", basefile.."_press.png", basefile.."_disable.png", v["order"], pos, rc,"selectstagewin","on_select_level")

        local starpos = CCPoint:new(v["star1pos"][1],v["star1pos"][2])
        local star1File = ""..basefile.."_star1.png"
        local star2File = ""..basefile.."_star2.png"
        local star3File = ""..basefile.."_star3.png"
        --三个星同一位置
        btn:CreateStarsSprite(star1File, starpos, star2File, starpos, star3File, starpos)
        btn:SetVisible(false)
        btn:SetAngle(v["angle"])
        btn_info["btn_list"][i] = btn
        local enter_action ={}
        enter_action["sprite"] = btn
        enter_action["type"] = action["enter"][i]["type"]
        enter_action["starttime"] = action["enter"][i]["starttime"]
        enter_action["parm"] = {}
        
        enter_action["parm"]["duration"] = action["enter"][i]["parm"]["duration"]
        enter_action["parm"]["vibdir"] = action["enter"][i]["parm"]["vibdir"]
        enter_action["parm"]["maxvibamp"] = action["enter"][i]["parm"]["maxvibamp"]
        enter_action["parm"]["maxvibtime"] = action["enter"][i]["parm"]["maxvibtime"]
        enter_action["parm"]["offset"] = {}
        enter_action["parm"]["offset"]["x"] = action["enter"][i]["parm"]["offset"]["x"]
        enter_action["parm"]["offset"]["y"] = action["enter"][i]["parm"]["offset"]["y"]
        enter_action["parm"]["pos"] = {}
        enter_action["parm"]["pos"]["x"] = v["pos"][1]
        enter_action["parm"]["pos"]["y"] = v["pos"][2]
		btn_info["enter_actions"][i] = enter_action
        
        
        local leave_action ={}
        leave_action["sprite"] = btn
        leave_action["type"] = action["leave"][i]["type"]
        leave_action["starttime"] = action["leave"][i]["starttime"]
        leave_action["parm"] = {}
        
        leave_action["parm"]["duration"] = action["leave"][i]["parm"]["duration"]
        leave_action["parm"]["vibdir"] = action["leave"][i]["parm"]["vibdir"]
        leave_action["parm"]["maxvibamp"] = action["leave"][i]["parm"]["maxvibamp"]
        leave_action["parm"]["maxvibtime"] = action["leave"][i]["parm"]["maxvibtime"]
        leave_action["parm"]["offset"] = {}
        leave_action["parm"]["offset"]["x"] = action["leave"][i]["parm"]["offset"]["x"]
        leave_action["parm"]["offset"]["y"] = action["leave"][i]["parm"]["offset"]["y"]
        leave_action["parm"]["pos"] = {}
        leave_action["parm"]["pos"]["x"] = v["pos"][1] + action["leave"][i]["parm"]["offset"]["x"]
        leave_action["parm"]["pos"]["y"] = v["pos"][2] + action["leave"][i]["parm"]["offset"]["y"]
        btn_info["leave_actions"][i] = leave_action        
                
        local shake_action ={}
        shake_action["sprite"] = btn
        shake_action["type"] = action["shake"][i]["type"]
        shake_action["starttime"] = action["shake"][i]["starttime"]
        shake_action["parm"] = {}
        shake_action["parm"]["dir"] = action["shake"][i]["parm"]["dir"]
        shake_action["parm"]["time"] = action["shake"][i]["parm"]["time"]
        shake_action["parm"]["delta"] = action["shake"][i]["parm"]["delta"]
        shake_action["parm"]["degree"] = action["shake"][i]["parm"]["degree"]        
        shake_action["parm"]["pos"]={}
        shake_action["parm"]["pos"]["x"] = v["pos"][1]
        shake_action["parm"]["pos"]["y"] = v["pos"][2]
        btn_info["shake_actions"][i] = shake_action
        
    end
    btn_info["enter_animate_time"] = enter_animate_time
    btn_info["leave_animate_time"] = leave_animate_time
    
    


    local back_pos = CCPoint:new(105/2,86/2)
    local back_btn = FxLuaButton:new()
    back_btn:CreateButton("back_normal.png","back_press.png","back_disable.png", 60, back_pos, "selectstagewin","on_click_back")
    back_btn:SetClickable(false)
    back_btn:SetVisible(false)
    btn_info["btn_list"]["back_btn"] = back_btn
    
    local store_pos = CCPoint:new(711/2,84/2)
    local store_btn = FxLuaButton:new()
    store_btn:CreateButton("store_btn_normal.png","store_btn_press.png","store_btn_normal.png",60, store_pos, "selectstagewin","on_click_store")
    store_btn:SetClickable(false)
    store_btn:SetVisible(false)
    btn_info["btn_list"]["store_btn"] = store_btn
    
end

function select_stage_ui:enter(parm)
    mode=parm["mode"]
    type=parm["type"]
    self.mode = mode
    self.type = type
    game_music:playMusic(music_main_bg)
    game_music:playEffect(ui_item_in)

    --初始化各个按钮状态
    mainscene:SetUILayerTouchable(false)
    self:init_btn_state(mode,type)

    --播放按钮出现动画
    self:play_enter_animate()

    --设置播放结束回调
     local timer = {func=on_enter_animate_over, {}}
     local enter_animate_time = self.all_ui_infos[mode][type]["enter_animate_time"]
     uimgr.ui_mgr_inst:add_timer(enter_animate_time, true, timer)

end
function select_stage_ui:do_move(action_list)
    for i,action in pairs(action_list) do 
        
        local timer = {}
        
        local timer = {func=baseui.do_action, parm=action}
        local starttime = action["starttime"]
        --local id=action["id"]
        --local sprite = action["sprite"]
        --action["sprite"] = sprite
        uimgr.ui_mgr_inst:add_timer(starttime, true, timer)      
    end 
end
function select_stage_ui:play_enter_animate()
        --设置按钮可见
    for i,v in pairs(self.all_ui_infos[self.mode][self.type]["btn_list"]) do
       if i ~= "back_btn" and i ~= "store_btn" then
          --v:SetVisible(true)
          mainscene:AddButton(v, v:GetOrderInParent())
       end
    end
    
    self:do_move(self.all_ui_infos[self.mode][self.type]["enter_actions"])
    self:do_move(self.all_ui_infos[self.mode][self.type]["shake_actions"])

end


function on_enter_animate_over()
    --设置按钮可见
    for i,v in pairs(select_stage_inst.all_ui_infos[select_stage_inst.mode][select_stage_inst.type]["btn_list"]) do

       if i == "back_btn" or  i == "store_btn" then
         --设置返回按钮可见并加入场景
          v:SetVisible(true)
          mainscene:AddButton(v, v:GetOrderInParent())
          v:SetClickable(true)
       end
    end
    mainscene:SetUILayerTouchable(true)
end

function select_stage_ui:init_btn_state(mode, scenetype)
    mainscene:refresh_stage_ui_data(mode, scenetype)
    btn_info = self.all_ui_infos[mode][scenetype]["btn_list"]
    for i =0,mainscene["StageNum"] - 1 do
        local islock = mainscene["stage_infos"][i]["bLock"]
        local stars = mainscene["stage_infos"][i]["nStars"]
        local map_id = mainscene["stage_infos"][i]["map_id"]
        local map_name = mainscene["stage_infos"][i]["map_name"]
        local pos = state_info[mode][scenetype][i]["name_pos"]
        local namepos = CCPoint:new(pos[1],pos[2])

        btn_info[i]:SetVisible(false)
        btn_info[i]:SetInt(map_id)
        --是否加锁
        -- islock=false
        if islock then
            btn_info[i]:SetClickable(false)
            btn_info[i]:SetStarsBg(false)
            map_name=" "
       else
           btn_info[i]:SetClickable(true)
           btn_info[i]:SetStarsBg(true)
           btn_info[i]:SetStars(stars)
       end
       btn_info[i]:SetNameString(map_name,namepos)

    end
       --btn_info["back_btn"]:SetClickable(true)
       --btn_info["back_btn"]:SetVisible(true)
       --mainscene:AddButton(btn_info["back_btn"], 60)
       --self:add_node(btn_info["back_btn"])
end


function select_stage_ui:leave(target, parm)
    mainscene:SetUILayerTouchable(false)
    self:play_leave_animate()

     --设置播放结束回调
     local timer = {func=on_leave_animate_over, parm={next_win_name=target,parm=parm}}
     local leave_animate_time = self.all_ui_infos[self.mode][self.type]["leave_animate_time"]
     uimgr.ui_mgr_inst:add_timer(leave_animate_time, true, timer)

end

function select_stage_ui:play_leave_animate()
    mainscene:SetUILayerTouchable(false)
    --让所有按钮进入不可选择状态,并隐去返回按钮
    for i,v in pairs(self.all_ui_infos[self.mode][self.type]["btn_list"]) do
        if i == "back_btn" or  i == "store_btn" then
        
            v:SetVisible(false)
            v:SetClickable(false)           
            mainscene:removeChild(v, false)
        end
        --v:SetClickable(false)
    end
    self:do_move(self.all_ui_infos[self.mode][self.type]["leave_actions"])
end

function on_leave_animate_over(parm)
    --让所有按钮进入不可选择状态,并隐去返回按钮
    self=select_stage_inst
    for i,v in pairs(self.all_ui_infos[self.mode][self.type]["btn_list"]) do
        if i~= "back_btn" and i ~= "store_btn"then
            v:SetClickable(false)
            v:SetVisible(false)
            mainscene:removeChild(v, false)
        end
    end
    mainscene:SetUILayerTouchable(true)

    next_ui = parm["next_win_name"]
    uimgr.ui_mgr_inst:goto_next_ui(next_ui,parm["parm"])
end




function select_stage_ui:enter_scene()
		self=select_stage_inst
		self.mode = mainscene:GetCurSceneCatlog()
		self.type = mainscene:GetCurSceneArtType()
		self:enter({mode=self.mode,type=self.type})
end

function select_stage_ui:leave_scene()
    --让所有按钮进入不可选择状态,并隐去返回按钮
    self=select_stage_inst
    for i,v in pairs(self.all_ui_infos[self.mode][self.type]["btn_list"]) do
            v:SetClickable(false)
            v:SetVisible(false)
            mainscene:removeChild(v, false)
    end
    --next_ui = parm["next_win_name"]
    --uimgr.ui_mgr_inst:goto_next_ui(next_ui,{})	
end

--声明窗口示例，注册窗口
select_stage_inst =select_stage_ui:new()
uimgr.ui_mgr_inst:register(select_stage_inst)





function on_select_level(n,str)
    mainscene:PlayGame(n)
end
function on_click_back(n,str)
    select_stage_inst:leave("select_scene_ui", {})
end

function on_click_store(n,str)
    self=select_stage_inst
    self:leave("shop_ui", {mode=self.mode,type=self.type})
end


