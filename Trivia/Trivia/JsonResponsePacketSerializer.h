#pragma once
#include "Responses.h"
#include <bitset>

#define BYTE_LEN 8

class JsonResponsePacketSerializer
{
public:

	
	static vector<unsigned char> serializeResponse(ErrorResponse);
	static vector<unsigned char> serializeResponse(LoginResponse);
	static vector<unsigned char> serializeResponse(SignupResponse);
	static vector<unsigned char> serializeResponse(LogoutResponse);
	static vector<unsigned char> serializeResponse(GetRoomsResponse);
	static vector<unsigned char> serializeResponse(GetPlayersInRoomResponse);
	static vector<unsigned char> serializeResponse(JoinRoomResponse);
	static vector<unsigned char> serializeResponse(CreateRoomResponse);
	static vector<unsigned char> serializeResponse(getHighScoreResponse);
	static vector<unsigned char> serializeResponse(getPersonalStatsResponse);
	static vector<unsigned char> serializeResponse(CloseRoomResponse);
	static vector<unsigned char> serializeResponse(StartGameResponse);
	static vector<unsigned char> serializeResponse(GetRoomStateResponse);
	static vector<unsigned char> serializeResponse(LeaveRoomResponse);
	static vector<unsigned char> serializeResponse(GetGameResultsResponse);
	static vector<unsigned char> serializeResponse(SubmitAnswerResponse);
	static vector<unsigned char> serializeResponse(GetQuestionResponse);
	static vector<unsigned char> serializeResponse(LeaveGameResponse);
	static vector<unsigned char> serializeResponse(HasGameEndedResponse);
	static std::string stringToByteString(std::string str);

	
private:
	//helpers
	static vector<unsigned char> serializeGeneralResponse(std::string message, int code);
};