//
//  GameManager.h
//  CreazyBomber
//
//  Created by AIRIA on 14-5-30.
//
//

#ifndef __CreazyBomber__GameManager__
#define __CreazyBomber__GameManager__

#include "common/CommonHeaders.h"
#include "game/objects/Player.h"

class GameManager
{
public:
    static GameManager *getInstance();
    CC_SYNTHESIZE(Player::WalkDirection, m_WalkDirection, WalkDirection);
    CC_SYNTHESIZE(Player *, m_pPlayer, Player);
    CC_SYNTHESIZE(Player::WalkDirection, m_currentWalkDirection, CurrentWalkDirection);
    CC_SYNTHESIZE(Point, m_pSpeed, Speed);
    CC_SYNTHESIZE(Player::WalkDirection, m_prevWalkDirection, PrevWalkDirection);
    CC_SYNTHESIZE(float, m_fScaleFactor, ScaleFactor);
    CC_SYNTHESIZE(Node *, m_pMapTileLayer, MapTileLayer);
    CC_SYNTHESIZE(bool, m_bIsConllision, IsCollision);
    CC_SYNTHESIZE(int, m_iBombPower, BombPower);
    CC_SYNTHESIZE(bool,m_bIsGameOver,IsGameOver);
    CC_SYNTHESIZE(int, m_iGameScore, GameScore);
    CC_SYNTHESIZE(int, m_iMonsterCount, MonsterCount);
};

#endif /* defined(__CreazyBomber__GameManager__) */
