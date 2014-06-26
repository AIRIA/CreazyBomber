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
#include "components/MapLayer.h"
#include "game/scenes/GameScene.h"
#include "game/objects/PlayerItem.h"

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
    
    CC_SYNTHESIZE(bool,m_bIsGameOver,IsGameOver);
    CC_SYNTHESIZE(int, m_iGameScore, GameScore);
    CC_SYNTHESIZE(int, m_iMonsterCount, MonsterCount);
    CC_SYNTHESIZE(MapLayer*, m_pMapLayer,MapLayer);
    /**
     * 可以放置的炸弹的数量
     */
    CC_SYNTHESIZE(int, m_iBombNum, BombNum);
    CC_SYNTHESIZE(int, m_iBombPower, BombPower);
    CC_SYNTHESIZE(int, m_iShoe, Shoe);
    CC_SYNTHESIZE(int, m_iCoin,Coin);
    CC_SYNTHESIZE(std::vector<PlayerItemType>, m_vPlayerItems, PlayerItems);
    
    
    Vector<WoodBox*> &getMovingBoxes(){ return m_vMovingBoxes;};
    
public:
    /**
     * 解析游戏道具的动画
     */
    void initPlayerItemAnimations(std::string name,std::string aniName,float delta = 0.1f);
    
    Vector<WoodBox*> m_vMovingBoxes;
    
};

#endif /* defined(__CreazyBomber__GameManager__) */
