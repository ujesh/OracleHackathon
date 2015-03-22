#include "SocketCommunicator.h"
#include "CommonDefinitions.h"
#include <ws2tcpip.h>

int SocketCommunicator::apiInit = 0;

SocketCommunicator::SocketCommunicator()
  : listenerSocketId(INVALID_SOCKET), listenerThreadHandle(NULL), receiveThreadHandle(NULL)
{
  //REVISIT:: How to effiiently handle Socket API startup and cleanup
  if(!apiInit)
  {
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2,0);
    int startupStatus = WSAStartup(wVersionRequested, &wsaData);
    if (startupStatus)
    {
      // Socket API initialization failed. Throw error.
      //std::stringstream oss;
      //oss << "WSAStartup Failed: WSAError: " << startupReturn;
      //throw UDPSocketDllException(CURRENT_FILE_LOCATION,oss.str());
    }
  }

  listenerSocketId = socket(AF_INET, SOCK_STREAM, 0);
  if(INVALID_SOCKET == listenerSocketId)
  {
    if(1 == apiInit)
    {
      WSACleanup();
    }
    else
    {
      apiInit--;
    }
    // Socket creation failed. Throw from here.
  }

  listenerStopEvent = CreateEvent(NULL,false,false,NULL);
  if (listenerStopEvent == NULL)
  {
    if(1 == apiInit)
    {
      WSACleanup();
    }
    else
    {
      apiInit--;
    }

    closesocket(listenerSocketId);
    // Creating thread event failed. Throw from here.
    //    throw OSObjectCreationFailedException(CURRENT_FILE_LOCATION,
    //                                 "CreateEvent : mStopUDPListenerThreadEvent");
  }

  listenerStartedEvent = CreateEvent(NULL,false,false,NULL);
  if (listenerStartedEvent == NULL)
  {
    if(1 == apiInit)
    {
      WSACleanup();
    }
    else
    {
      apiInit--;
    }

    closesocket(listenerSocketId);
    CloseHandle(listenerStopEvent);
    // Creating thread event failed. Throw from here.
    //    throw OSObjectCreationFailedException(CURRENT_FILE_LOCATION,
    //                                 "CreateEvent : mStopUDPListenerThreadEvent");
  }
  

  receiveThreadStopEvent = CreateEvent(NULL,false,false,NULL);
  if (receiveThreadStopEvent == NULL)
  {
    if(1 == apiInit)
    {
      WSACleanup();
    }
    else
    {
      apiInit--;
    }

    closesocket(listenerSocketId);
    CloseHandle(listenerStopEvent);
    CloseHandle(listenerStartedEvent);
    // Creating thread event failed. Throw from here.
    //    throw OSObjectCreationFailedException(CURRENT_FILE_LOCATION,
    //                                 "CreateEvent : mStopUDPListenerThreadEvent");
  }

   receiveThreadStartedEvent = CreateEvent(NULL,false,false,NULL);
  if (receiveThreadStartedEvent == NULL)
  {
    if(1 == apiInit)
    {
      WSACleanup();
    }
    else
    {
      apiInit--;
    }

    closesocket(listenerSocketId);
    CloseHandle(listenerStopEvent);
    CloseHandle(listenerStartedEvent);
    CloseHandle(receiveThreadStopEvent);
    // Creating thread event failed. Throw from here.
    //    throw OSObjectCreationFailedException(CURRENT_FILE_LOCATION,
    //                                 "CreateEvent : mStopUDPListenerThreadEvent");
  }

}

SocketCommunicator::~SocketCommunicator()
{
  if(1 == apiInit)
  {
    WSACleanup();
  }
  
  //Stop listener thread
  if(listenerStopEvent)
  {
    SetEvent(listenerStopEvent);
    if(WAIT_TIMEOUT == WaitForSingleObject(listenerThreadHandle, THREAD_WAIT_TIMEOUT))
    {
      //Log error
    }
    CloseHandle(listenerThreadHandle);
    CloseHandle(listenerStopEvent);
  }

  if(receiveThreadStopEvent)
  {
    SetEvent(receiveThreadStopEvent);
    if(WAIT_TIMEOUT == WaitForSingleObject(receiveThreadHandle, THREAD_WAIT_TIMEOUT))
    {
      //Log error
    }
    CloseHandle(receiveThreadHandle);
    CloseHandle(receiveThreadStopEvent);
  }

  closesocket(listenerSocketId);
}

void SocketCommunicator::Send(const int slaveId, const std::string &data)
{
  IntToSocketMap::iterator slaveSocketIterator = slaveIdSocketMap.find(slaveId);
  SOCKET &slaveSocket = slaveSocketIterator->second;
  int bytesSent = Send(slaveSocket, data);
  if(bytesSent <= 0)
  {
    //Socket send failed. Log and throw from here.
  }
}

void SocketCommunicator::Receive(std::string &data)
{

}

//Private Functions
int SocketCommunicator::Send(const SOCKET& socket, const std::string &data)
{
  return send(socket, data.c_str(), 0, 0);
}

void SocketCommunicator::Listen(int port)
{
  Bind(port);
  if(listen(listenerSocketId, 0) == SOCKET_ERROR)
  {
    //Listen failed. Throw from here.
  }
  else
  {
    try
    {
      //Start Listner thread.
      DWORD threadId;
      listenerThreadHandle = CreateThread(
              NULL,
              0,
              (LPTHREAD_START_ROUTINE)ListenerThreadFunction,
              this,
              0,
              &threadId);

      if (listenerThreadHandle == NULL)
      {
        //Listener thread creation failed. Throw from here.
        //throw OSObjectCreationFailedException(CURRENT_FILE_LOCATION,
      } 
      WaitForSingleObject(listenerStartedEvent, THREAD_WAIT_TIMEOUT);
    }
    catch(...)
    {
      //Create thread failed. Throw from here.
    }
  }
}

void SocketCommunicator::Bind(int port)
{
  struct sockaddr_in sockAddr;  
  sockAddr.sin_port=htons(port);
  sockAddr.sin_addr.s_addr=htonl(INADDR_ANY);
  int ii=0;
  if(bind(listenerSocketId, (sockaddr *)&(sockAddr), sizeof(sockAddr)) == SOCKET_ERROR)
  {
    //Bind error. Throw from here.
  }
}

SOCKET SocketCommunicator::Accept(SOCKET socket)
{
  struct sockaddr_in clientSockAddr;
  int clientAddrLenght = 0;
  return accept(socket, (sockaddr *)&clientSockAddr, &clientAddrLenght);
}

void SocketCommunicator::ListenerThreadFunction(SocketCommunicator *communicator)
{
  SetEvent(communicator->listenerStartedEvent);
  while(WAIT_OBJECT_0 != WaitForSingleObject(communicator->listenerStopEvent, 0))
  {
    SOCKET clientSocket = communicator->Accept(communicator->listenerSocketId);
    if(clientSocket == INVALID_SOCKET)
    {
      //Accept failed.
    }
    else
    {
      communicator->OnAccept(clientSocket);
    }
  }
}

void SocketCommunicator::OnAccept(SOCKET socket)
{
  DWORD threadId;
  receiveThreadHandle = CreateThread(
              NULL,
              0,
              (LPTHREAD_START_ROUTINE)ReceiveThread,
              this,
              0,
              &threadId);

  if (receiveThreadHandle == NULL)
  {
    //Listener thread creation failed. Throw from here.
    //throw OSObjectCreationFailedException(CURRENT_FILE_LOCATION,
  } 
  WaitForSingleObject(receiveThreadStartedEvent, THREAD_WAIT_TIMEOUT);
}

/*void SocketCommunicator::ReceiveThread( socket)
{
  SetEvent(receiveThreadStartedEvent)
  char receiveData[RECEIVE_BUFFER_SIZE] = {0};
  while(recv(socket, receiveData, RECEIVE_BUFFER_SIZE, 0) != SOCKET_ERROR)
  {
    //Send to master
  }
}
*/


