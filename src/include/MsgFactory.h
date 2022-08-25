#pragma once
#include "configor/configor.hpp"
#include "configor/json.hpp"

#include <functional>
#include <string>
#include <unordered_map>

class MsgBase;

using PTRCreateObject = std::function<MsgBase*(configor::json&)>;
class MsgFactory {
  private:
    std::unordered_map<std::string, PTRCreateObject> msgDict;
    MsgFactory() {}

  public:
    MsgBase* getMsgObjectByName(std::string& protoName, configor::json& j) {
        auto iter = msgDict.find(protoName);
        if (iter == msgDict.end())
            return NULL;
        else
            return iter->second(j);
    }

    void registClass(std::string protoName, PTRCreateObject func) {
        if (msgDict.find(protoName) == msgDict.end()) {
            msgDict.insert(std::pair<std::string, PTRCreateObject>(
                protoName, std::bind(func, std::placeholders::_1)));
        }
    }

    static MsgFactory& getInstance() {
        static MsgFactory instance;
        return instance;
    }
};

// 注册动作类
class RegisterAction {
  public:
    RegisterAction(std::string className, PTRCreateObject ptrCreateFn) {
        printf("register success!!\n");
        MsgFactory::getInstance().registClass(className, ptrCreateFn);
    }
};

#define REGISTER(className)                                                    \
    className* create##className(json& j) { return new className(j); }         \
    RegisterAction g_creatorRegister##className(                               \
        #className, (PTRCreateObject)create##className)