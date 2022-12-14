#pragma once
#include <memory>
#include <string>
#include "User.h"

//线程安全的纯工具类，使用连接池(SqlConnectionPool)对数据库进行操作
//无情的数据库操作工具
class DbManager {
  public:
    
    //根据用户名判断用户是否注册
    static bool isInAccount(std::string id) ;
    //注册用户数据到account表中
    static bool userRegister(std::string id, std::string pw);

    //将用户数据插入到userdata表中
    static void createUserData(std::string id);
    static std::unique_ptr<UserData> getUserData(std::string id);
    //更新用户数据
    static void updataUserData(std::string id, UserData* data);
    //检查密码是否正确
    static bool checkPassword(std::string id, std::string pw);
};