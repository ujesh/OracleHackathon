#if !defined(_MESSAGE_PACKET_H)
#define _MESSAGE_PACKET_H
#include <string>

class MessagePacket
{
public:
	enum MessageType{
		HEARTBEAT,
		JOBSTATUS,
		JOBDATA,
		REGISTER
	};
	MessagePacket();
	MessagePacket(MessageType messageType, int sourceId, int fileId, std::string data);

	~MessagePacket();
	std::string GetMessagePacket();
private:
	MessageType	mu_messageType;
	int		mi_messageId;
	int		mi_processId;
	std::string	ms_data;
};

#endif