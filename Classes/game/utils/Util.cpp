//
//  Util.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-23.
//
//

#include "Util.h"

std::string Util::itoa(int i)
{
    char buff[50];
    sprintf(buff, "%d",i);
    return buff;
}