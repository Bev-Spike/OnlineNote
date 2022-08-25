
#include "unordered_map"
#include "Buffer.h"
#include "NetManager.h"

#include <memory>
#include <mutex>
#include <unordered_map>
class EventLoop;
class CilentState;
class Server;
class Connection;
class NoteServer {
  private:
    NetManager _manager;
    std::unique_ptr<Server> _server;
    std::unordered_map<int, std::unique_ptr<CilentState>> _cilents;
    int _userCnt;
    std::mutex _mtx;

  public:
    NoteServer(EventLoop* loop);
    ~NoteServer() = default;
    //在建立和关闭连接时调用
    void OnConnection(Connection* conn);
    //在读取到数据时调用
    void OnReadData(Connection* conn, Buffer* buffer);

    void addListenner();
};