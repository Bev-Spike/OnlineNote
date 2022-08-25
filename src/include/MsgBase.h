// 定义协议类的基类，并定义协议的解码编码
#pragma once
#include "MsgFactory.h"
#include "configor/json.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
using namespace configor;
class MsgBase {
  public:
    //协议名，要求与类名完全相同
    std::string _protoName;

  public:
    //返回一个json对象，要求子类必须实现
    virtual configor::json getJson() = 0;

    //编码
    static std::string Encode(MsgBase* msgBase) {
        configor::json j = msgBase->getJson();
        //支持unicode编码
        return j.dump();
    }

    //解码
    //将字符串解码为具体的协议类
    //返回的是一个堆对象，小心内存泄漏
    static MsgBase* Decode(std::string protoName, std::string& data) {
        json j = json::parse(data);
        return MsgFactory::getInstance().getMsgObjectByName(protoName, j);
    }

    //编码协议名
    //将协议名编码为string对象，在前面附上两个字节的长度信息
    static std::string EncodeName(MsgBase* msgBase) {
        //名字bytes和长度
        std::string& nameBytes = msgBase->_protoName;
        std::int16_t len = static_cast<std::int16_t>(nameBytes.length());

        std::string bytes(2 + len, 0);
        //低位放低字节，高位放高字节
        bytes[0] = (len % 256);
        bytes[1] = (len / 256);
        //组装名字
        for (size_t i = 0; i < nameBytes.size(); i++) {
            bytes[i + 2] = nameBytes[i];
        }

        return bytes;
    }

    //协议名解码
    //该方法会根据bytes和offset找到协议名数据的起始地址，先解析出协议名长度，再将协议名字符串解析出并返回。协议名的长度由count返回
    static std::string DecodeName(std::string& bytes, int offset, int& count) {
        count = 0;
        //字节数组必须大于2字节
        if (offset + 2 > bytes.length()) {
            return "";
        }
        std::int16_t len =
            static_cast<std::int16_t>((bytes[offset + 1] << 8) | bytes[offset]);
        //长度必须足够
        if (offset + 2 + len > bytes.length()) {
            return "";
        }
        //解析
        count = 2 + len;
        std::string name = std::string(bytes, offset + 2, len);
        return name;
    }
};
