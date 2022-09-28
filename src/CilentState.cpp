#include "CilentState.h"
#include "NoteServer.h"

CilentState::CilentState(Connection* conn, NoteServer* server)
    : _conn(conn),
      _server(server),
      _manager(&(server->_netManager)),
      _userManager(&(server->_userManager)),
      _isUser(false),
      _lastPingTime(0) {}
