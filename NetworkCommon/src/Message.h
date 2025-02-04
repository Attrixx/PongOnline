#pragma once 

#include "Object.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

constexpr int SIGNATURE = 0x12345678;
constexpr size_t SIGNATURE_SIZE = sizeof(int);

enum MessageType
{
	DEFAULT = 0,
	PLAY,
	LOGIC,
	CONNECT,
	DISCONNECT
};

struct MessageHeader
{
    int signature = SIGNATURE;
    int totalSize = 0;   // Taille totale du message
    int packetIndex = 0; // Index du fragment
    int packetCount = 0; // Nombre total de fragments
};

struct MessageBuffer
{
	std::vector<char> data;
	int receivedPackets = 0;
	int expectedPackets = 0;
};

class Message
{
public:
	std::string toString();
	static Message toMessage(const char* input);

	static Message CreateMessage(MessageType type = DEFAULT, const json& data = defaultData);

public:
    json content = {
        {"id", 0},
        {"messageType", DEFAULT},
        {"data", defaultData}
    };

	static const json defaultData;
	static const json connectData;
	static const json logicData;
	static const json playData;
};
