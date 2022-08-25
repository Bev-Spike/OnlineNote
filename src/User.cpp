#include "User.h"
#include "CilentState.h"
#include "NetManager.h"
#include <memory>

User::User(CilentState* state) : _id(""), _state(state) {}

void User::Send(MsgBase* msgBase) {
    _state->_manager->send(_state ,msgBase);    
}

bool UserManager::isOnline(std::string id) { return _users.count(id); }

    //获取用户
User* UserManager::getUser(std::string id) {
    if (_users.count(id)) {
        return _users[id].get();
    }
    return nullptr;
}

    //添加用户
void UserManager::addUser(std::string id, User* user) {
    _users[id] = std::shared_ptr<User>(user);
}
    //删除用户
void UserManager::removeUser(std::string id) {
    if (_users.count(id)) {
        _users.erase(id);
    }    
}