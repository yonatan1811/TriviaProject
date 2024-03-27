#pragma once

#include <iostream>
#include "Requests.h"
#include <vector>
#include "json.hpp"
#include <string.h>

#define BYTE_LEN 8

class JsonRequestPacketDeserializer
{
public:
    static LoginRequest deserializeLoginRequest(std::vector<unsigned char> s);
    static SignupRequest deserializeSignupRequest(std::vector<unsigned char> s);
    static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(std::vector<unsigned char> s);
    static JoinRoomRequest deserializeJoinRoomRequest(std::vector<unsigned char> buffer);
    static CreateRoomRequest deserializeCreateRoomRequest(std::vector<unsigned char> buffer);
    static SubmitAnswerRequest deserializeSubmitAnswerRequest(std::vector<unsigned char> buffer );
    static int binaryToDecInt(std::string num);
    static std::string BinToString(std::string bin);
private:
};