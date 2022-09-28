#include "User.h"
#include "CilentState.h"
#include "NetManager.h"
#include <memory>
#include <mutex>
#include <pthread.h>

UserManager userManager;

User::User(CilentState* state) : _id(""), _state(state) {}

void User::Send(MsgBase* msgBase) {
    _state->_manager->send(_state ,msgBase);    
}

bool UserManager::isOnline(std::string id) {
    std::unique_lock<std::mutex> lock(_mtx);
    return _users.count(id); }

    //获取用户
User* UserManager::getUser(std::string id) {
    std::unique_lock<std::mutex> lock(_mtx);
    if (_users.count(id)) {
        return _users[id].get();
    }
    return nullptr;
}

    //添加用户
void UserManager::addUser(std::string id, std::shared_ptr<User> user) {
    std::unique_lock<std::mutex> lock(_mtx);
    _users[id] = std::shared_ptr<User>(user);
}
    //删除用户
void UserManager::removeUser(std::string id) {
    std::unique_lock<std::mutex> lock(_mtx);
    if (_users.count(id)) {
        int count = _users[id].use_count();
        _users.erase(id);
    }    
}