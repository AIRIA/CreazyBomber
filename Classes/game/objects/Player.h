//
//  Player.h
//  CreazyBomber
//
//  Created by AIRIA on 14-5-29.
//
//

#ifndef __CreazyBomber__Player__
#define __CreazyBomber__Player__

#include "common/CommonHeaders.h"
#include "game/objects/MapObject.h"

class Player:public MapObject
{
public:
    enum WalkDirection{
        kWalkUp,
        kWalkLeft,
        kWalkRight,
        kWalkDown,
        kWalkStand
    };
    
    virtual Rect getBoundingBox() const;
    
    /**
     * 根据用户选择的角色名字 来创建对应的角色
     */
    static Player *create(MapCell *mapCell);
    
    const Point &getCoordinate();
    std::string getDirectionStr();
    
    virtual void onEnter();
    virtual bool init();
    virtual void run();
    void born();
    void walk(WalkDirection direction);
    void update(float delta);
    void loadPlayerInfo();
    void stopWalkAction();
    /**
     * 玩家被攻击啊后 调用
     * @param heart 伤害值
     */
    void beAttack(float heart);
    
    void die();
    
protected:
    Point _coordinate;
    /**
     * 是否处于可以被攻击的状态
     */
    bool _isCanBeAttack = true;
    
    /**
     * 闪烁
     */
    void blink(float delta);

    int _blinkTime = 0;
    
    CC_SYNTHESIZE(float, m_fWidth, Width);
    CC_SYNTHESIZE(float, m_fHeight, Height);
    CC_SYNTHESIZE(float, m_fSpeed, Speed);
    CC_SYNTHESIZE(float, m_fFootPos, FootPos);
    CC_SYNTHESIZE(float, m_fHP, HP);
};

#endif /* defined(__CreazyBomber__Player__) */
