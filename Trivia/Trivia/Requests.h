#pragma once

#include <iostream>
#include <string>

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
#define END_COMM 33


using std::string;

struct LoginRequest
{
	string username;
	string password;
};

struct SignupRequest
{
	string username;
	string password;
	string email;
};

struct GetPlayersInRoomRequest
{
	unsigned int roomID;
};

struct JoinRoomRequest
{
	unsigned int roomID;
};

struct CreateRoomRequest
{
	string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeOut;
};

struct SubmitAnswerRequest
{
	unsigned int answerId;
	unsigned int timeToAnswer;
};
