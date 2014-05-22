module (..., package.seeall)
function GetGameOverNumFont()
    return "font/smallfont.fnt"
end

function GetCurRolePng(roletype, flag)
   return "gameover_"..flag.."_"..RoleName[roletype]..".png"
end
function GetCurRole1Png_win()
    return GetCurRolePng(Role_P1_Zombie,"win")
end
function GetCurRole2Png_win()
    return GetCurRolePng(Role_P2_Smurf,"win")
end
function GetCurRole3Png_win()
   return GetCurRolePng(Role_P3_Viking,"win")
end
function GetCurRole4Png_win()
   return GetCurRolePng(Role_P4_Vampire,"win")
end

function GetCurRole1Png_lost()
    return GetCurRolePng(Role_P1_Zombie,"lost")
end
function GetCurRole2Png_lost()
    return GetCurRolePng(Role_P2_Smurf,"lost")
end
function GetCurRole3Png_lost()
   return GetCurRolePng(Role_P3_Viking,"lost")
end
function GetCurRole4Png_lost()
   return GetCurRolePng(Role_P4_Vampire,"lost")
end



function GetStoryOverBg()
    return "story_over_bg.png"
end
function GetStoryBgPos()
    --return CCPoint:new(263,154)
    return CCPoint:new(263,454)
end

function GetStoryRolePos()
    return CCPoint:new(4,75)
end


function GetStoryStar1Png()
    return "gameover_star1.png"
end
function GetStoryStar2Png()
    return "gameover_star2.png"
end
function GetStoryStar3Png()
    return "gameover_star3.png"
end

function GetStoryStar0Pos()
    return CCPoint:new(128, 161)
end
function GetStoryStar1Pos()
    return  CCPoint:new(128, 161)
end
function GetStoryStar2Pos()
    return  CCPoint:new(128, 161)
end

function GetStoryLevelIconPos()
    return  CCPoint:new(245,122)
end

function GetStoryCurScorePos()
    return  CCPoint:new(127, 100)
end

function GetStoryHighScorePos()
    return  CCPoint:new(127, 50)
end

function GetStoryGetCoinPos()
    return  CCPoint:new(260, 96)
end
function GetStoryAddCoinPos()
    return  CCPoint:new(260, 75)
end

function GetStoryTotalCoinPos()
    return  CCPoint:new(260, 50)

end

function GetStoryReturnPos()    
    return CCPoint:new(60, 12)
end    
function GetStoryRestartPos()    
    return CCPoint:new(174, 12)
end    
function GetStoryNextPos()    
    return CCPoint:new(279, 10)
end    


--

function GetBattleOverBg()
    return "battle_over_bg.png"
end
function GetBattleBgPos()
    --return CCPoint:new(260,154)
    return CCPoint:new(260,454)
end

function GetBattleRolePos()
    return  CCPoint:new(0,172)
end
function GetBattleResultPos()
    return  CCPoint:new(200,200)
end
function GetBattleCurScorePos()
    return  CCPoint:new(125,103)
end
function GetBattleP1ScorePos()
    return  CCPoint:new(142,78)
end
function GetBattleP2ScorePos()
    return  CCPoint:new(142,58)
end
function GetBattleP3ScorePos()
    return  CCPoint:new(142,40)
end

function GetBattleGetCoinPos()
    return  CCPoint:new(261,96)
end
function GetBattleAddCoinPos()
    return  CCPoint:new(261,75)
end
function GetBattleTotalCoinPos()
    return  CCPoint:new(261,49)
end
function GetBattleReturnPos()    
    return CCPoint:new(62, 9)
end    
function GetBattleRestartPos()    
    return CCPoint:new(169, 23)
end    
  

