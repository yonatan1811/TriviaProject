#include "JsonResponsePacketDeserializer.h"

#define END_OF_CODE 8
#define END_OF_LENGTH 40

char strToChar(std::string str)
{
	char parsed = 0;
	for (int i = 0; i < 8; i++)
	{
		if (str[i] == '1')
		{
			parsed |= 1 << (7 - i);
		}
	}
	return parsed;
}

//Login Request:


// Define a function to parse a JSON string into a MyStruct object
LoginRequest parseJsonStringLogin(const std::string& jsonString) {
	// Parse the JSON string using nlohmann::json
	nlohmann::json json = nlohmann::json::parse(jsonString);

	// Create a MyStruct object and initialize its fields using the values from the JSON object
	LoginRequest result;
	result.username = json["username"];
	result.password = json["password"];
	return result;
}


LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(std::vector<unsigned char> s)
{
	std::string dataInStr = "";

	for (unsigned char c : s)
	{
		dataInStr += c;
	}
	return parseJsonStringLogin(dataInStr);
}

SignupRequest parseJsonStringSignUp(const std::string& jsonString)
{
	nlohmann::json json = nlohmann::json::parse(jsonString);

	// Create a MyStruct object and initialize its fields using the values from the JSON object
	SignupRequest result;
	result.username = json["username"];
	result.password = json["password"];
	result.email = json["email"];
	return result;
}

GetPlayersInRoomRequest parseJsonStringGetPlayersInRoom(const std::string& jsonString)
{
	nlohmann::json json = nlohmann::json::parse(jsonString);

	GetPlayersInRoomRequest res;
	res.roomID = json["roomId"];
	return res;
}


SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(std::vector<unsigned char> s)
{
	std::string dataInStr = "";
	for (char c : s)
	{
		dataInStr += c;
	}
	return parseJsonStringSignUp(dataInStr);
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(std::vector<unsigned char> s)
{
	std::string dataInStr = "";
	for (char i : s)
	{
		dataInStr += i;
	}
	return parseJsonStringGetPlayersInRoom(dataInStr);
}

JoinRoomRequest ParseJoinRoomRequest(const std::string& jsonString)
{
	nlohmann::json json = nlohmann::json::parse(jsonString);

	JoinRoomRequest res;
	res.roomID = json["roomID"];
	return res;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(std::vector<unsigned char> buffer)
{
	std::string dataInStr = "";
	for (char i : buffer)
	{
		dataInStr += i;
	}
	return ParseJoinRoomRequest(dataInStr);

}

CreateRoomRequest ParseCreateRoomRequest(const std::string& jsonString)
{
	nlohmann::json json = nlohmann::json::parse(jsonString);
	CreateRoomRequest res;
	res.answerTimeOut = json["answerTimeOut"];
	res.maxUsers = json["maxUsers"];
	res.questionCount = json["questionCount"];
	res.roomName = json["roomName"];
	return res;
}


CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(std::vector<unsigned char> buffer)
{
	std::string dataInStr = "";
	for (char i : buffer)
	{
		dataInStr += i;
	}
	return ParseCreateRoomRequest(dataInStr);

}


SubmitAnswerRequest ParseSubmitAnswerRequest(const std::string& jsonString)
{
	nlohmann::json json = nlohmann::json::parse(jsonString);
	SubmitAnswerRequest res;
	res.answerId = json["answerId"];
	res.timeToAnswer = json["timeToAnswer"];
	return res;
}

SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(std::vector<unsigned char> buffer)
{
	std::string dataInStr = "";
	for (char i : buffer)
	{
		dataInStr += i;
	}
	return ParseSubmitAnswerRequest(dataInStr);
}

int JsonRequestPacketDeserializer::binaryToDecInt(std::string num)
{
	int dec = 0;
	for (int i = 0; i < num.length(); i++)
	{
		dec += (num[i] - '0') * std::pow(2, num.length() - i - 1);
	}
	return dec;
}

std::string JsonRequestPacketDeserializer::BinToString(std::string bin)
{
	std::string finalStr = "";
	for (int i = 0; i <= bin.length() - BYTE_LEN; i += BYTE_LEN)
	{
		finalStr += (char)binaryToDecInt(bin.substr(i, BYTE_LEN));
	}
	if (finalStr =="")
	{
		finalStr = "-1";
	}
	return finalStr;
}


