#include "pch.h"
#include "Message.h"
#include <iostream>

void Message::toString(const Message& input, std::string& output)
{
	output = input.content.dump();
}

const Message& Message::toMessage(const char* input)
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
