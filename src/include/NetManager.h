// 封装使用网络库的各种操作

#pragma once

#include "Buffer.h"
#include "MsgBase.h"
#include <functional>
#include <string>
#include <unordered_map>
#include "CilentState.h"

//封装消息处理、发送数据的方法
class NetManager {
    using MsgFunction = std::function<void(CilentState*, MsgBase*)>;
  private:
    //保存协议处理的方法
    std::unordered_map<std::string, MsgFunction> _msgListeners;
  public:
    //向一个客户发送一条协议
    void send(CilentState*, MsgBase*);
    //添加协议处理方法
    void addMsgListener(std::string msgName, MsgFunction func);
    //移除协议处理方法
    void removeMsgListener(std::string msgName);

    //解析协议，事件分发
    void onReceiveData(CilentState*, Buffer*);

  private:
    void fireMsg(CilentState* ,MsgBase*);
};