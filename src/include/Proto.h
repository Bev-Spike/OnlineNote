#ifndef __PROTO__
#define __PROTO__
//将所有协议都写在这个文件中

#include "MsgBase.h"
class MsgMove : public MsgBase {
  public:
    int _x;
    int _y;
    int _z;
    MsgMove(int x, int y, int z) : _x(x), _y(y), _z(z) {
        _protoName = "MsgMove";
    }
    MsgMove(json& j) : _x(j["x"]), _y(j["y"]), _z(j["z"]) {
        _protoName = "MsgMove";
    }

    configor::json getJson() ;
};



class MsgHit : public MsgBase {
  public:
    int _x;
    int _y;
    int _z;
    MsgHit(int x, int y, int z) : _x(x), _y(y), _z(z) {
        _protoName = "MsgHit";
    }
    MsgHit(json& j) : _x(j["x"]), _y(j["y"]), _z(j["z"]) {
        _protoName = "MsgHit";
    }

    configor::json getJson();
};


#endif