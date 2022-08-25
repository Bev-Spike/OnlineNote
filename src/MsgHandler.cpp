#include "MsgHandler.h"

#include "CilentState.h"
#include "NetManager.h"

#include <cstdio>
void MsgHandler::handleMoveMsg(CilentState* state, MsgBase* msgBase) {
    MsgMove* msgMove = dynamic_cast<MsgMove*>(msgBase);
    printf("get MoveMsg: x=%d, y=%d, z=%d\n",
           msgMove->_x,
           msgMove->_y,
           msgMove->_z);
    msgMove->_x += 1;
    msgMove->_y += 1;
    state->_manager->send(state, msgMove);
}