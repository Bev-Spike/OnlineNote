#include "MsgHandler.h"

#include "CilentState.h"
#include "NetManager.h"
#include "Proto.h"
#include "DbManager.h"
#include "User.h"
#include <cstdio>
#include <memory>

extern UserManager userManager;

void MsgHandler::handleMoveMsg(CilentState* state, MsgBase* msgBase) {
    MsgMove* msgMove = dynamic_cast<MsgMove*>(msgBase);
    printf("[协议]get MoveMsg: x=%d, y=%d, z=%d\n",
           msgMove->_x,
           msgMove->_y,
           msgMove->_z);
    msgMove->_x += 1;
    msgMove->_y += 1;
    state->_manager->send(state, msgMove);
}


void MsgHandler::handleRegisterMsg(CilentState* state, MsgBase* msgBase) {
    MsgRegister* msgRegister = dynamic_cast<MsgRegister*>(msgBase);
    printf("[协议] get RegisterMsg: id=%s, pw=%s\n",
           msgRegister->id.c_str(),
           msgRegister->pw.c_str());
    if (DbManager::userRegister(msgRegister->id, msgRegister->pw)) {
        DbManager::createUserData(msgRegister->id);
        msgRegister->result = 0;
    }
    else {
        msgRegister->result = 1;
    }
    state->_manager->send(state, msgRegister);
}

void MsgHandler::handleLoginMsg(CilentState* state, MsgBase* msgBase) {
    MsgLogin* msgLogin = dynamic_cast<MsgLogin*>(msgBase);
    printf("[协议] get LoginMsg: id=%s, pw=%s\n",
           msgLogin->id.c_str(),
           msgLogin->pw.c_str());
    if (!DbManager::checkPassword(msgLogin->id, msgLogin->pw)) {
        //账号或密码错误
        msgLogin->result = 1;
        state->_manager->send(state, msgLogin);
        return;
    }
    //如果当前state已经有用户了，就将该用户下线
    if (state->_isUser) {
        //保存用户数据
        DbManager::updataUserData(state->_user->_id, state->_user->_data.get());
        state->_userManager->removeUser(state->_user->_id);
    }
    //如果该id在线，就把他踢下线！
    if (state->_userManager->isOnline(msgLogin->id)) {
        User* other = state->_userManager->getUser(msgLogin->id);
        MsgKick msgKick;
        msgKick.reason = 0;
        //提醒对面客户端，让其主动断开连接
        other->Send(&msgKick);
        // //断开这用户的连接
        // other->_state->_conn->close();
    }

    auto userData = DbManager::getUserData(msgLogin->id);
    if (userData == nullptr) {
        msgLogin->result = 1;
        state->_manager->send(state, msgLogin);
        return;
    }
    //构建user
    std::shared_ptr<User> user(new User(state));
    state->_user = user;
    user->_id = msgLogin->id;
    user->_data = std::move(userData);
    state->_userManager->addUser(user->_id, user);
    state->_isUser = true;
    //返回协议
    msgLogin->result = 0;
    user->Send(msgLogin);
}

//从服务器获取记事本的内容
void MsgHandler::handleGetTextMsg(CilentState* state, MsgBase* msgBase) {
    MsgGetText* msgGet = dynamic_cast<MsgGetText*>(msgBase);
    User* user = state->_user.get();
    if (user == nullptr)
        return;
    msgGet->text = user->_data->_text;
    user->Send(msgGet);
}
//将用户文本保存在服务器中
void MsgHandler::handleSaveTextMsg(CilentState* state, MsgBase* msgBase) {
    MsgSaveText* msgSave = dynamic_cast<MsgSaveText*>(msgBase);
    User* user = state->_user.get();
    if (user == nullptr) {
        return;
    }
    msgSave->result = 0;
    user->_data->_text = msgSave->text;
    user->Send(msgSave);
}