//
//  PlayerRole.h
//  CreazyBomber
//
//  Created by AIRIA on 14-5-21.
//
//

#ifndef __CreazyBomber__PlayerRole__
#define __CreazyBomber__PlayerRole__

#include "common/CommonHeaders.h"

class PlayerRole:public Ref
{
private:
    std::string _roleName;
public:
    std::string getRoleName() {return _roleName;};
    void setRoleName(std::string name) {_roleName = name;};
};

#endif /* defined(__CreazyBomber__PlayerRole__) */
