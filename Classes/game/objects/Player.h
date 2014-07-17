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
    
    virtual Rect getBoundingBox() const;
    /**
     * 根据用户选择的角色名字 来创建对应的角色
     */
    static Player *create(MapCell *mapCell);
    
    const Point &getCoordinate();
    /* 获取方向字符串 */
    std::string getDirectionStr();
    virtual void onEnter();
    virtual bool init();
    /*  */
    virtual void run();
    /* 玩家生成的时候 播放闪动的动画 并且播放音效 */
    void born();
    /** 
     * 控制玩家的行走方向
     * @param direction 行走的方向
     * @param isForce 是否要强制执行方法内的逻辑 跳过重复调用的判断
     */
    void walk(WalkDirection direction,bool isForce = false);
    /* 是炸弹人停止行走 并且播放对应的呼吸动画 */
    void stand();
    /* 进行与炸弹 怪物 以及建筑的碰撞逻辑 */
    void update(float delta);
    /* 加载玩家角色的属性 */
    void loadPlayerInfo();
    
    /**
     * 玩家被攻击后 调用
     * @param heart 伤害值
     */
    void beAttack(float heart);
    /**
     * 播放角色死亡的动画
     */
    void die();
    
protected:
    Point _coordinate;
    /**
     * 是否处于可以被攻击的状态
     */
    bool _isCanBeAttack = true;
    void _revive(Ref *pSender);
    
    CC_SYNTHESIZE(float, m_fWidth, Width);
    CC_SYNTHESIZE(float, m_fHeight, Height);
    CC_SYNTHESIZE(float, m_fSpeed, Speed);
    CC_SYNTHESIZE(float, m_fFootPos, FootPos);
    CC_SYNTHESIZE(float, m_fHP, HP);
    /* 角色的名字 */
    CC_SYNTHESIZE(std::string, m_sRoleName, RoleName);
    /* 角色行走的方向 */
    CC_SYNTHESIZE(WalkDirection, m_WalkDirection, WalkDirection);
    /* 角色行走的速度 */
    CC_SYNTHESIZE(Point, m_WalkSpeed, WalkSpeed);
    /* 智能判断出来的临时方向 */
    CC_SYNTHESIZE(WalkDirection, m_SmartDirection, SmartDirection);
    /* 是否处于被阻挡的状态 */
    CC_SYNTHESIZE(bool, m_bIsCollision, IsCollision);
};

#endif /* defined(__CreazyBomber__Player__) */
