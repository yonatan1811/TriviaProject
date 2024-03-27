#pragma once
#include <vector>
#include <string>
#include <map>
#include "RoomStructs.h"

#define ERROR_REQUEST_CODE 5
#define SIGNUP_REQUEST_CODE 6
#define LOGIN_REQUEST_CODE 7
#define CREATE_ROOM_REQUEST_CODE 8
#define GET_ROOMS_REQUEST_CODE 9
#define GET_PLAYERS_REQUEST_CODE 10
#define JOIN_ROOM_REQUEST_CODE 11
#define LOGOUT_REQUEST_CODE 12
#define GET_STATS_REQUEST_CODE 13
#define GET_HIGH_SCORES_REQUEST_CODE 14
#define CLOSE_ROOM_REQUEST_CODE 15
#define START_GAME_REQUEST_CODE 16
#define GET_ROOM_STATE_REQUEST_CODE 17
#define LEAVE_ROOM_REQUEST_CODE 18
#define GET_GAME_RESULTS_CODE 19
#define SUBMIT_ANSWER_RESPONSE 20
#define LEAVE_GAME_CODE 21
#define GET_QUETION_RESPONSE_CODE 22
#define HAS_GAME_ENDED_RESPONSE_CODE 23

#define SUCCESS 1
#define FAIL 0

using std::vector;
using std::string;
using std::map;


struct LoginResponse
{
	unsigned int status;
};

struct SignupResponse
{
	unsigned int status;

};

struct LogoutResponse
{
	unsigned int status;
	
};

struct GetRoomsResponse
{
	unsigned int status;
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse
{
	std::vector<std::string> players;
};

struct getHighScoreResponse
{
	unsigned int status;
	std::vector<std::string> statistics;
};

struct getPersonalStatsResponse
{
	unsigned int status;
	vector<string> statistics;
};

struct JoinRoomResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	unsigned int status;
};

struct CloseRoomResponse
{
	unsigned int status;

};

struct StartGameResponse
{
	unsigned int status;

};

struct GetRoomStateResponse
{
	unsigned int status;
	int hasGameBegun;
	vector<string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
};
struct LeaveRoomResponse
{
	unsigned int status;
};

struct ErrorResponse
{
	string message;
};


struct LeaveGameResponse
{
	unsigned int status;
};

struct GetQuestionResponse
{
	unsigned int status;
	string question;
	map<unsigned int, string> answers;
};
struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnswerId;
};

struct PlayerResult
{
	string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
};

struct GetGameResultsResponse
{
	unsigned int status;
	vector<PlayerResult> results;
};

struct HasGameEndedResponse
{
	unsigned int hasEnded;
};

