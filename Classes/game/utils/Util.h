//
//  Util.h
//  CreazyBomber
//
//  Created by AIRIA on 14-6-23.
//
//

#ifndef __CreazyBomber__Util__
#define __CreazyBomber__Util__

#include "common/CommonHeaders.h"

class Util
{
public:
    static std::string itoa(int i);
    static void playSound(std::string sound,bool repeat=false);
    static void playEffect(std::string effect,bool repeat=false);
};

#endif /* defined(__CreazyBomber__Util__) */
