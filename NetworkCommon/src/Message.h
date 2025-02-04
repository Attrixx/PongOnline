#pragma once 

#include "Object.h"

enum MessageType
{
	PLAY,
	CONNECT,
	DISCONNECT
};

class Message
{
public:
	const char* toString(const Message& input);
	static const Message& toMessage(char* input);


private:
	int m_signature;
	class User* m_user;

	MessageType m_messageType;

	Object* m_ball;
	Object* m_paddleRight;
	Object* m_paddleLeft;
};