#include "Proto.h"
#include "MsgFactory.h"



configor::json MsgMove::getJson() {
        return {{"protoName", _protoName}, {"x", _x}, {"y", _y}, {"z", _z}};
}
REGISTER(MsgMove);

configor::json MsgHit::getJson() {
        return {{"protoName", _protoName}, {"x", _x}, {"y", _y}, {"z", _z}};
}
REGISTER(MsgHit);

configor::json MsgRegister::getJson() {
    return {{"protoName", _protoName}, {"id", id}, {"pw", pw}, {"result", result}};
}
REGISTER(MsgRegister);

configor::json MsgLogin::getJson() {
    return {{"protoName", _protoName}, {"id", id}, {"pw", pw}, {"result", result}};
}
REGISTER(MsgLogin);

configor::json MsgKick::getJson() {
    return {{"protoName", _protoName},  {"reason", reason}};
}
REGISTER(MsgKick);


configor::json MsgGetText::getJson() {
    return {{"protoName", _protoName},  {"text", text}};
}
REGISTER(MsgGetText);


configor::json MsgSaveText::getJson() {
    return {{"protoName", _protoName}, {"text", text}, {"result", result}};
}
REGISTER(MsgSaveText);