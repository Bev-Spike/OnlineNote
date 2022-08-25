#include "NoteServer.h"
#include "Buffer.h"
#include "Connection.h"
#include "EventLoop.h"
#include "MsgHandler.h"
#include "Server.h"
#include "NetManager.h"
#include "CilentState.h"
#include <functional>
#include <memory>
#include <mutex>
#include <sys/types.h>
#include "SqlConnectionPool.h"
NoteServer::NoteServer(EventLoop* loop):_userCnt(0) {
    _server = std::make_unique<Server>(loop);
    _server->setConnectionCallBack(
        std::bind(&NoteServer::OnConnection, this, std::placeholders::_1));
    _server->setMessageCallback(std::bind(&NoteServer::OnReadData,
                                          this,
                                          std::placeholders::_1,
                                          std::placeholders::_2));
    addListenner();
    //初始化数据库连接
    ConnectionPool::GetInstance().init("82.156.16.46", "root", "594137", "game", 3306, 2);
}

//添加协议处理方法
void NoteServer::addListenner() {
    _manager.addMsgListener("MsgMove", &MsgHandler::handleMoveMsg);
}


//在建立和关闭连接时调用
void NoteServer::OnConnection(Connection* conn) {
    int socketfd = conn->getSocket()->getFd();
    printf("Connection %d %s\n", socketfd, conn->getState()== Connection::Connected? "up" : "down");

    std::unique_lock<std::mutex> lock(_mtx);
    //新用户连接
    if (conn->getState() == Connection::Connected) {
        _cilents[socketfd] = std::make_unique<CilentState>(conn, &_manager);
        _userCnt++;
    }
    //关闭用户连接
    else {
        //如果缓冲区有剩余数据，就将其发完（这里没实现）

        _cilents.erase(socketfd);
        _userCnt --;
    }
}
//在读取到数据时调用
void NoteServer::OnReadData(Connection* conn, Buffer* readBuffer) {
    int socketfd = conn->getSocket()->getFd();
    CilentState* state = _cilents[socketfd].get();
    _manager.onReceiveData(state, readBuffer);

}