#pragma once

#include "MsgBase.h"
#include "configor/json.hpp"
#include <memory>
#include <mutex>
#include <string>
#include <map>
class CilentState;
//存进数据库的数据
class UserData {
  public:
    //记事本
    std::string _text;
    UserData()=default;
    //configor库 json绑定
    CONFIGOR_BIND_ALL_REQUIRED(json, UserData, _text);
};

class User {
  public:
    std::string _id;
    CilentState* _state;
    std::unique_ptr<UserData> _data;

  public:
    User(CilentState*);
    ~User() {
        printf("user 析构\n");
    }
    void Send(MsgBase*);
};

//管理所有用户类
//非线程安全
class UserManager {
  private:
    std::map<std::string, std::shared_ptr<User>> _users;
    std::mutex _mtx;
  public:
    bool isOnline(std::string);
    //获取用户
    User* getUser(std::string);
    //添加用户
    void addUser(std::string, std::shared_ptr<User> user);
    //删除用户
    void removeUser(std::string);
};