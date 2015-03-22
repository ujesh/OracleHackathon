#if !defined(_SOCKETCOMMUNICATOR_H)
#define _SOCKETCOMMUNICATOR_H

#include "IPCManager.h"
#include <WinSock2.h>
#include <map>

typedef std::map<int, SOCKET> IntToSocketMap;

struct ReceiveThreadParam;

class SocketCommunicator : public IPCManager {
public:
  SocketCommunicator();
  ~SocketCommunicator();
  void Send(const int slaveId, const std::string &data);
  void Receive(std::string &data);
private: //Functions
  int Send(const SOCKET& socket, const std::string &data);
  static void ListenerThreadFunction(SocketCommunicator *communicator);
  void Listen(int port);
  void Bind(int port);
  SOCKET Accept(SOCKET socket);
  void OnAccept(SOCKET socket);
  static void ReceiveThread(ReceiveThreadParam *argument);
private:
  SOCKET listenerSocketId;
  static int apiInit;
  IntToSocketMap slaveIdSocketMap;
  HANDLE listenerThreadHandle;
  HANDLE listenerStartedEvent;
  HANDLE listenerStopEvent; 

  HANDLE receiveThreadHandle;
  HANDLE receiveThreadStartedEvent;
  HANDLE receiveThreadStopEvent;
};

struct ReceiveThreadParam
{
  SocketCommunicator *commuicator;
  SOCKET socket;
};

#endif  //_SOCKETCOMMUNICATOR_H
