#include "pch.h"
#include "Message.h"
#include <iostream>

const json Message::defaultData = { {"message", ""} };
const json Message::connectData = { {"name", ""}, {"port", 0}, {"address", ""}};
const json Message::logicData = {
	{"ball", {{"posX", 0}, {"posY", 0}, {"dirX", 0}, {"dirY", 0}, {"speed", 0}}},
	{"paddleRight", {{"posX", 0}, {"posY", 0}, {"dirY", 0}}},
	{"paddleLeft", {{"posX", 0}, {"posY", 0}, {"dirY", 0}}}
};
const json Message::playData = {
	{"movedPaddle", {{"dirY", 0}}}
};
const json Message::scoreData = {
	{"healthPoints", 0}
};

std::string Message::toString()
{
	return content.dump();
}

Message Message::toMessage(const char* input)
{
	Message newMessage;
	try
	{
		newMessage.content = json::parse(input);
		return newMessage;
	}
	catch (const std::exception&)
	{
		std::cout << "can't read message" << std::endl;
		return Message();
	}
}

Message Message::CreateMessage(MessageType type, const json& data)
{
	Message message;
	message.content["id"] = -1;
	message.content["messageType"] = static_cast<int>(type);
	message.content["data"] = data;
	return message;
}
