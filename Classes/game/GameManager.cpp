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
    }
    return _instance;
}