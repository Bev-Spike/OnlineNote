#ifndef __PROTO__
#define __PROTO__
//将所有协议都写在这个文件中

#include "MsgBase.h"
#include <string>
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
//注册协议
class MsgRegister :public MsgBase {
  public:
    //由客户端发送
    std::string id;
    std::string pw;
    //服务端回复,0为成功，1为失败
    int result;
    MsgRegister() : result(0) { _protoName = "MsgRegister"; }
    MsgRegister(json& j) : id(j["id"]), pw(j["pw"]), result(j["result"]) {
        _protoName = "MsgRegister";
    }
    configor::json getJson();
};

//登录协议
class MsgLogin : public MsgBase {
  public:
    std::string id;
    std::string pw;
    int result;
    MsgLogin() : result(0) { _protoName = "MsgLogin"; }
    MsgLogin(json& j) : id(j["id"]), pw(j["pw"]), result(j["result"]) {
        _protoName = "MsgLogin";
    }
    configor::json getJson();
};

//踢下线协议（由服务端主动发出)
class MsgKick : public MsgBase {
  public:
    //原因 0-其他人登录同一账号
    int reason;
    MsgKick() : reason(0) { _protoName = "MsgKick"; }
    MsgKick(json& j) : reason(0) {
        _protoName = "MsgKick";
    }
    configor::json getJson();
};


//记事本协议
//获取记事本内容
class MsgGetText : public MsgBase {
  public:
    //由服务端回应
    std::string text;
    MsgGetText() { _protoName = "MsgGetText"; }
    MsgGetText(json& j) { _protoName = "MsgGetText"; }
    configor::json getJson();
};

//保存记事本
class MsgSaveText : public MsgBase {
  public:
    //由客户端发送
    std::string text;
    //服务端回应:0-成功 1-失败(虽然不知道为什么失败总之先写着)
    int result;
    MsgSaveText() : result(0) { _protoName = "MsgSaveText"; }
    MsgSaveText(json& j):text(j["text"]){_protoName = "MsgSaveText";}
    configor::json getJson() override;
};
#endif