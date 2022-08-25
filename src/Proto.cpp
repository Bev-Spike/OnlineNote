#include "Proto.h"
#include "MsgFactory.h"



configor::json MsgMove::getJson() {
        return {{"protoName", _protoName}, {"x", _x}, {"y", _y}, {"z", _z}};
}

configor::json MsgHit::getJson() {
        return {{"protoName", _protoName}, {"x", _x}, {"y", _y}, {"z", _z}};
}


REGISTER(MsgMove);
REGISTER(MsgHit);