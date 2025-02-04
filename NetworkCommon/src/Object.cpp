#include "pch.h"
#include "Object.h"
#include <vector>


Object::Object(Vector2 position, Vector2 direction, float speed)
	: m_position(position), m_direction(direction), m_speed(speed)
{
}

const Object& Object::FromString(char* input)
{
	std::string strInput = input;
	strInput = strInput.substr(1, strInput.size() - 2); //Remove "{" and "}"

	std::string delimiter = ")";

	std::vector<std::string> tokens;
	size_t pos = 0;
	std::string token;
	while ((pos = strInput.find(delimiter)) != std::string::npos) {
		token = strInput.substr(0, pos);

		tokens.push_back(token);
		strInput.erase(0, pos + delimiter.length());
	}
	tokens.push_back(strInput);

	for (auto s : tokens)
	{
		s = s.substr(1, s.size() - 2); //remove "(" and ")"
	}

}

const std::string Object::ToString(const Object& input)
{
	std::string retVal;

	retVal += "{(" + std::to_string(input.m_position.x) + ", " + std::to_string(input.m_position.y) + ")";
	retVal += ",(" + std::to_string(input.m_direction.x) + ", " + std::to_string(input.m_direction.y) + ")";
	retVal += ",(" + std::to_string(input.m_speed) + ")}";

	return retVal;
}
