//保存连接客户的数据
#pragma once

#include <memory>
#include "Buffer.h"
#include "Connection.h"
#include "User.h"
class Connection;
class NetManager;
class Buffer;
class User;
class CilentState {
  public:
    Connection* _conn;
    NetManager* _manager;
    std::shared_ptr<User> _user;
    //该客户数据是否已经对应一个User
    bool _isUser;
    long _lastPingTime;

  public:
    CilentState(Connection* conn, NetManager* manager)
        : _conn(conn),
          _manager(manager),
          _isUser(false),
          _lastPingTime(0)
        {
       
    }
};