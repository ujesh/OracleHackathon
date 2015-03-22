#if !defined(_IPCMANAGER_H)
#define _IPCMANAGER_H
#include <iostream>

class IPCManager {
public:
	virtual void Send(const int slaveId, const std::string &data) = 0;
	virtual void Receive(std::string &data) = 0;
};

#endif  //_IPCMANAGER_H
