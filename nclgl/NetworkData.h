#pragma once
#include <string.h>
#include "Vector3.h"
#include <vector>

#define MAX_PACKET_SIZE 1000000
#define MAX_PLAYER_NUMBER 10
#define NUMBER_OF_INPUT 15

using namespace std;

enum PacketTypes {

    INIT_CONNECTION = 0,

    ACTION_EVENT = 1,

};

struct Packet {

    unsigned int packet_type;
	int numberOfPlayers;
	int clientNumber;
	Vector3 positon[MAX_PLAYER_NUMBER];
	int inputs[NUMBER_OF_INPUT];

    void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
    }
};