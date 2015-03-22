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
		REGISTER,
		SLAVEIDLE
	};
	MessagePacket();
	MessagePacket(MessageType,int,int, std::string);

	~MessagePacket();
	std::string GetMessagePacket();
private:
	MessageType	mu_messageType;
	int		mi_messageId;
	int		mi_processId;
	std::string	ms_data;
};

#endif