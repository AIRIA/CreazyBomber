//
//  TempScene.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-7-11.
//
//

#include "TempScene.h"
#include "HomeScene.h"

Scene *TempScene::scene()
{
    auto scene = Scene::create();
    auto ts = TempScene::create();
    scene->addChild(ts);
    return scene;
}

bool TempScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    return true;
}

void TempScene::onEnter()
{
    HomeScene::create()->run();
}