#include "DbManager.h"

#include "DataBase.h"
#include "SqlConnectionPool.h"
#include "User.h"
#include "configor/json.hpp"

#include <cstdio>
#include <memory>
#include <mysql/mysql.h>
#include <string>

//根据用户名判断用户是否注册
bool DbManager::isInAccount(std::string id) {
    std::string s = "select * from account where id='" + id + "';";
    MYSQL* sqlConn;
    try {

        {
            ConnectionGuard sqlGuard(sqlConn, ConnectionPool::GetInstance());
            DataBase db(sqlConn);
            auto result = db.query(s);
            return !result.getRows().empty();
        }

    } catch (MySQLException e) {
        printf("%s", e.what());
        return false;
    }
}

//注册
bool DbManager::userRegister(std::string id, std::string pw) {
    //安全的做法应该防止SQL注入，但我懒得写（C++字符串处理太麻烦了）
    if (isInAccount(id)) {
        printf("[数据库] 注册失败，用户已存在！\n");
        return false;
    }
    std::string sql =
        "insert into account set id='" + id + "', pw='" + pw + "';";
    try {
        MYSQL* sqlConn;
        ConnectionGuard sqlGuard(sqlConn, ConnectionPool::GetInstance());
        DataBase db(sqlConn);
        db.execute(sql);
        return true;
    } catch (MySQLException e) {
        printf("%s", e.what());
        return false;
    }
}


//将用户数据插入到userdata表中
void DbManager::createUserData(std::string id) {
    UserData data;
    configor::json j = data;
    std::string dbData = j.dump();

    std::string sql =
        "insert into userdata set id='" + id + "', data='" + dbData + "';";
    try {
        MYSQL* sqlConn;
        ConnectionGuard sqlGuard(sqlConn, ConnectionPool::GetInstance());
        DataBase db(sqlConn);
        db.execute(sql);
    } catch (MySQLException e) {
        printf("%s", e.what());
    }
}

std::unique_ptr<UserData> DbManager::getUserData(std::string id) {
    std::string s = "select * from userdata where id='" + id + "';";
    MYSQL* sqlConn;
    try {
        QueryResult result;
        {
            ConnectionGuard sqlGuard(sqlConn, ConnectionPool::GetInstance());
            DataBase db(sqlConn);
            result = db.query(s);
        }
        if (result.getRows().size() == 0) {
            printf("[数据库] 无该用户数据！\n");
            return nullptr;
        }
        else {
            std::string key = "data";
            QueryRow row = result.getRows()[0];
            std::string s = row[key];
            configor::json j = json::parse(s);
            std::unique_ptr<UserData> userdata(new UserData((UserData)j));
            return userdata;
        }
    } catch (MySQLException e) {
        printf("%s", e.what());
        return nullptr;
    }
}

//更新用户数据
void DbManager::updataUserData(std::string id, UserData* data) {
    std::string sql = "update userdata set data='" +
                      ((configor::json)(*data)).dump() + "' where id='" + id +
                      "';";
    try {
        MYSQL* sqlConn;
        ConnectionGuard sqlGuard(sqlConn, ConnectionPool::GetInstance());
        DataBase db(sqlConn);
        db.execute(sql);
    } catch (MySQLException e) {
        printf("%s", e.what());
    }
}
//检查密码是否正确
bool DbManager::checkPassword(std::string id, std::string pw) {
    std::string s =
        "select * from account where id='" + id + "' and pw='" + pw + "';";
    MYSQL* sqlConn;
    try {
        ConnectionGuard sqlGuard(sqlConn, ConnectionPool::GetInstance());
        {
            DataBase db(sqlConn);
            auto result = db.query(s);
            return !result.getRows().empty();
        }

    } catch (MySQLException e) {
        printf("%s", e.what());
        return false;
    }
}
