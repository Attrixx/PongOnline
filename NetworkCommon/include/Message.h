#pragma once 

#include "Object.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum MessageType
{
	DEFAULT = 0,
	PLAY,
	LOGIC,
	CONNECT,
	DISCONNECT
};

class Message
{
public:
	void toString(const Message& input, std::string& output);
	static const Message& toMessage(const char* input);

public:
	json content = {
		{"id", 0},
		{"messageType", DEFAULT},
		{"data", defaultData}
	};

	json defaultData = {
		{"message"}
	};

	json connectData = {
		{"name"},
		{"port"}
	};

	json logic = {
		{"ball", {
			{"posX"},
			{"posy"},
			{"dirX"},
			{"dirY"},
			{"speed"},
}},
		{"paddleRight", {
			{"posX"},
			{"posy"},
			{"dirX"},
			{"dirY"},
			{"speed"},
}},
		{"paddelLeft", {
			{"posX"},
			{"posy"},
			{"dirX"},
			{"dirY"},
			{"speed"},
}} };

	json play = {
		{"movedPaddle", {
			{"posX"},
			{"posy"},
			{"dirX"},
			{"dirY"},
			{"speed"},
}} };

};