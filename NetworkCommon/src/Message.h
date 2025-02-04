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
    int totalSize;   // Taille totale du message
    int packetIndex; // Index du fragment
    int packetCount; // Nombre total de fragments
};

class Message
{
public:
	std::string toString();
	static Message toMessage(const char* input);

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
