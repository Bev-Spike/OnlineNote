//所有协议的处理方式都放在这里面
#pragma once
#include "Proto.h"
#include "MsgFactory.h"
class CilentState;
class MsgBase;
class MsgHandler {
  public:
    static void handleMoveMsg(CilentState* state, MsgBase* msgBase);
    static void handleRegisterMsg(CilentState* state, MsgBase* msgBase);
    static void handleLoginMsg(CilentState* state, MsgBase* msgBase);
    static void handleGetTextMsg(CilentState* state, MsgBase* msgBase);
    static void handleSaveTextMsg(CilentState* state, MsgBase* msgBase);
};