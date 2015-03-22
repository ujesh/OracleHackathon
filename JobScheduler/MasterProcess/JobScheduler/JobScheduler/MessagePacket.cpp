#include "MessagePacket.h"
#include <sstream>


MessagePacket::MessagePacket() {

}
MessagePacket::MessagePacket(MessageType au_messageType ,int ai_processID, int ai_messageId, std::string as_data)
{
	mi_messageId = ai_messageId;
	mi_processId = ai_processID;
	mu_messageType = au_messageType;
	ms_data = as_data;
}

MessagePacket::~MessagePacket()
{
}
std::string MessagePacket::GetMessagePacket()
{
	
	std::stringstream	ls_message;
	std::string ls_returnMessage;

	ls_message<< mi_processId<<"|" << mu_messageType <<"|" <<mi_messageId <<"|" << ms_data;
	ls_returnMessage = ls_message.str();
	return ls_returnMessage;

}
