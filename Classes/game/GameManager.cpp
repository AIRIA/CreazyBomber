//
//  GameManager.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-5-30.
//
//

#include "GameManager.h"

GameManager *GameManager::getInstance()
{
    static GameManager *_instance = nullptr;
    if(_instance==nullptr)
    {
        _instance = new GameManager();
        _instance->setGameScore(345);
        _instance->setBombNum(1);
    }
    return _instance;
}