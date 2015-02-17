#pragma once
#include "ServerNetwork.h"
#include "NetworkData.h"

enum UserInputs{
	UP = 0,
	LEFT = 1,
	RIGHT = 2,
	DOWN = 3
};

class ServerGame
{

public:

    ServerGame(void);
    ~ServerGame(void);

    void update();

	void receiveFromClients();

	void sendActionPackets();

private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

   // The ServerNetwork object 
    ServerNetwork* network;
	Packet recvPacket;
	Packet sendPacket;
	// data buffer
   char network_data[MAX_PACKET_SIZE];
};