#include "NetManager.h"
#include "Connection.h"
#include "MsgBase.h"
#include "CilentState.h"
#include "configor/configor.hpp"
#include <bits/stdint-intn.h>
#include <cstdio>
#include <memory>
#include <string>

//向一个客户发送协议
void NetManager::send(CilentState* state, MsgBase* msgBase) {
    if(state == nullptr) return;
    Connection* conn = state->_conn;
    //编码协议体
    std::string bodyBytes = MsgBase::Encode(msgBase);
    //编码协议名
    std::string nameBytes = MsgBase::EncodeName(msgBase);
    int len = bodyBytes.length() + nameBytes.length();
    std::string sendBytes(2, 0);
    sendBytes[0] = len % 256;
    sendBytes[1] = len / 256;
    sendBytes = sendBytes + nameBytes + bodyBytes;

    conn->send(sendBytes);
    
}
//添加协议处理方法
//非线程安全，请在构造函数中调用该函数
void NetManager::addMsgListener(std::string msgName, MsgFunction func) {
    if (!_msgListeners.count(msgName)) {
        _msgListeners[msgName] = func;
    }
}
//移除协议处理方法
//非线程安全
void NetManager::removeMsgListener(std::string msgName) {
    if (_msgListeners.count(msgName)) {
        _msgListeners.erase(msgName);
    }
}

//解析协议，事件分发
void NetManager::onReceiveData(CilentState* state, Buffer* readBuf) {
    if (readBuf->readableBytes() <= 2) {
        return;
    }
    //消息体的长度,不包括前面两个表示长度的字节
    int16_t bodyLength = readBuf->peekInt16();
    //如果消息不完整，直接返回
    if (readBuf->readableBytes() < bodyLength + 2) {
        return;
    }
    readBuf->retrieve(2);
    //解析协议名
    int nameCount = 0;
    std::string msg = readBuf->retrieveAsString(bodyLength);
    std::string protoName = MsgBase::DecodeName(msg, 0, nameCount);
    if (protoName == "") {
        //协议长度与说好的不一样
        printf("OnReceiveData MsgBase.DecodeName fail\n");
        return;
    }
    //解析协议体
    std::string bodyMsg = msg.substr(nameCount);
    int bodyCount = bodyLength - nameCount; //协议体的长度
    std::unique_ptr<MsgBase> msgBase(MsgBase::Decode(protoName, bodyMsg));
    if(msgBase != nullptr){
    //分发消息
    fireMsg(state, msgBase.get());
    
    }
    else {
        printf("unknown proto!!\n");
    }
    
    //如果缓冲区还有数据，继续处理
    if (readBuf->readableBytes() > 2) {
        onReceiveData(state, readBuf);
    }
}

void NetManager::fireMsg(CilentState* state, MsgBase* msgBase) {
    std::string protoName = msgBase->_protoName;
    if (_msgListeners.find(protoName) != _msgListeners.end()) {
        _msgListeners[protoName](state, msgBase);
    }
    else {
        printf("unkonwn protoNmae:%s\n", protoName.c_str());
    }
}