#include "JsonResponsePacketSerializer.h"

std::string JsonResponsePacketSerializer::stringToByteString(std::string str)
{
	std::string binaryStr = "";
	for (char c : str) {
		std::bitset<BYTE_LEN> binaryChar(c);
		binaryStr += binaryChar.to_string();
	}
	return binaryStr;
}

vector<unsigned char> JsonResponsePacketSerializer::serializeGeneralResponse(std::string message,int code)
{
	vector<unsigned char> buff;
	std::string strCode = std::bitset<BYTE_LEN>(code).to_string();
	std::string binaryMsg = stringToByteString(message);
	std::string msgLen = std::bitset< BYTE_LEN * 4 >(binaryMsg.length()).to_string();
	std::string totalMsg = strCode + msgLen + binaryMsg;

	for (char c : totalMsg)
	{
		buff.push_back(c);
	}
	return buff;
}


vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(ErrorResponse res)
{
	std::string message = "{message:" + res.message + "}";
	return serializeGeneralResponse(message,ERROR_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LoginResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status) + "}";
	return serializeGeneralResponse(message,LOGIN_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(SignupResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status) + "}";
	return serializeGeneralResponse(message,SIGNUP_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LogoutResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status) + "}";
	return serializeGeneralResponse(message, LOGOUT_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status);
	std::string baseStr = "rooms:[";
	std::string rooms = baseStr;
	for (auto i : res.rooms)
	{
		rooms += "{\"id\":" + std::to_string(i.id) + ",\"name\":\"" + i.name + "\",\"maxPlayers\":" + std::to_string(i.maxPlayers) + ",\"numOfQuestions\":" + std::to_string(i.numOfQuestions) + ",\"timePerQuestion\":" + std::to_string(i.timePerQuestion) + ",\"isActive\":" + std::to_string(i.isActive) + "},";
	}

	//only remove ',' if there was one added
	if (rooms.length() > baseStr.length())
	{
		rooms.pop_back();
	}
	rooms += "]";
	std::string final = message + ","  + rooms + "}";
	return serializeGeneralResponse(final , GET_ROOMS_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse res)
{
	std::string baseMsg = "{PlayersInRoom:";
	std::string mes = baseMsg;
	for (auto i : res.players)
	{
		mes += i + ",";
	}
	//only remove ',' if there was one added
	if (mes.length() > baseMsg.length())
	{
		mes.pop_back();
	}
	mes += "}";
	return serializeGeneralResponse(mes, GET_PLAYERS_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status) + "}";
	return serializeGeneralResponse(message, JOIN_ROOM_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status) + "}";
	return serializeGeneralResponse(message, CREATE_ROOM_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(getPersonalStatsResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status);
	std::string stats = "\"statistics\":[\"" + res.statistics[0] + "\",\"" + res.statistics[1] + "\",\"" + res.statistics[2] + "\",\"" + res.statistics[3] + "\"]";
	std::string final = message + "," + stats + "}";
	return serializeGeneralResponse(final, GET_STATS_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(getHighScoreResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status);
	std::string baseStr = "scores:[";
	std::string scores = baseStr;
	for (auto i : res.statistics)
	{
		scores += '\"' + i + '\"' + ",";
	}

	//only remove ',' if there was one added
	if (scores.length() > baseStr.length())
	{
		scores.pop_back();
	}
	scores += "]";
	std::string final = message + "," + scores + "}";
	return serializeGeneralResponse(final, GET_HIGH_SCORES_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status) + "}";
	return serializeGeneralResponse(message, CREATE_ROOM_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(StartGameResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status) + "}";
	return serializeGeneralResponse(message, START_GAME_REQUEST_CODE);
}



vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetGameResultsResponse res)
{
	std::string baseMsg = "[";
	std::string results = baseMsg;
	for (auto i : res.results)
	{
		results += "{\"username\":\""+i.username +"\",\"correctAnswerCount\":"+ std::to_string(i.correctAnswerCount) + ",\"wrongAnswerCount\":" + std::to_string(i.wrongAnswerCount) + ",\"averageAnswerTime\":" + std::to_string(i.averageAnswerTime) + "},";
	}
	//only remove ',' if there was one added
	if (results.length() > baseMsg.length())
	{
		results.pop_back();
	}
	results += "]";
	std::string message = "{\"status\":" + std::to_string(res.status) + ",\"results\":" + results + "}";
	return serializeGeneralResponse(message, GET_GAME_RESULTS_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status) + ",\"correctAnswerId\":" + std::to_string(res.correctAnswerId) + "}";
	return serializeGeneralResponse(message, SUBMIT_ANSWER_RESPONSE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse res)
{
	std::string result = ((res.question == "") ? "0" : "1");
	std::string baseStr = "{";
	std::string scores = baseStr;
	for (auto i : res.answers)
	{
		scores += std::to_string(i.first) + ":\"" + i.second + "\",";
	}

	//only remove ',' if there was one added
	if (scores.length() > baseStr.length())
	{
		scores.pop_back();
	}
	scores += "}";
	std::string msg = "{\"status\":" + result+ ",\"question\":\"" + res.question + "\",\"answers\":" + scores + "}";
	return serializeGeneralResponse(msg, GET_QUETION_RESPONSE_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status) + "}";
	return serializeGeneralResponse(message, LEAVE_GAME_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse res)
{
	std::string baseMsg = "[";
	std::string msg = baseMsg;
	for (auto i : res.players)
	{
		msg += + "\"" + i + "\",";
	}
	//only remove ',' if there was one added
	if (msg.length() > baseMsg.length())
	{
		msg.pop_back();
	}
	msg += "]";
	std::string message = "{\"status\":" + std::to_string(res.status) + "," + "\"hasGameBegun\":" + std::to_string(res.hasGameBegun) + ",\"players\":" + msg + ",\"questionCount\":" + std::to_string(res.questionCount) + ",\"answerTimeout\":" + std::to_string(res.answerTimeout) + "}";
	return serializeGeneralResponse(message, CREATE_ROOM_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse res)
{
	std::string message = "{\"status\":" + std::to_string(res.status) + "}";
	return serializeGeneralResponse(message, LEAVE_ROOM_REQUEST_CODE);
}

vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(HasGameEndedResponse res)
{
	std::string message = "{\"hasEnded\":" + std::to_string(res.hasEnded) + "}";
	return serializeGeneralResponse(message, LEAVE_ROOM_REQUEST_CODE);
}







