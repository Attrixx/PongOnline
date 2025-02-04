#pragma once 

#include "Object.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum MessageType
{
	PLAY,
	LOGIC,
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

	json m_message = {
		{"signature"},
		{"id"},
		{"messageType"},
		{"data"}
	};

	json m_connectData = {
		{"name"},
		{"port"}
	};

	json m_logic = {
		{"ball"},
		{"paddleRight"},
		{"paddelLeft"}
	};

	json m_play = {
		{"movedPaddle"}
	};

};