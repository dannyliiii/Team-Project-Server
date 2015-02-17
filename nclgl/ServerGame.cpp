#include "StdAfx.h"
#include "ServerGame.h"

unsigned int ServerGame::client_id; 

ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork(); 
}

ServerGame::~ServerGame(void)
{
}

void ServerGame::update()
{
    // get new clients
   if(network->acceptNewClient(client_id))
   {
        printf("client %d has been connected to the server\n",client_id);
        client_id++;
   }

   receiveFromClients();
}

void ServerGame::receiveFromClients()
{

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;
    for(iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        int data_length = network->receiveData(iter->first, network_data);

        if (data_length <= 0) 
        {
            //no data recieved
            continue;
        }

        int i = 0;
        while (i < (unsigned int)data_length) 
        {
            recvPacket.deserialize(&(network_data[i]));
            i += sizeof(Packet);

            switch (recvPacket.packet_type) {

                case INIT_CONNECTION:


					sendPacket.packet_type = INIT_CONNECTION;
					sendPacket.clientNumber = iter->first;

                    printf("server received init packet from client\n");

                    sendActionPackets();

                    break;

                case ACTION_EVENT:

					sendPacket.packet_type = ACTION_EVENT;
					sendPacket.positon[iter->first] = recvPacket.positon[iter->first];

				/*	for(int i = 0 ; i < MAX_PLAYER_NUMBER; i ++){
						cout<< sendPacket.positon[i] << endl;
					}*/
					cout << recvPacket.positon[iter->first];
					cout << "===================================" << endl;

				/*	for each (int input in recvPacket.inputs)
					{
						if(input == 1){
							printf("!!");
						}
					}*/
					for (int i = 0; i < NUMBER_OF_INPUT; i++){
						if (recvPacket.inputs[i] == 1){
							cout << i << endl;
						}
					}
					cout << "===================================" << endl;

                    //printf("server received action event packet from client\n");

                    sendActionPackets();

                    break;

                default:

                    printf("error in packet types\n");

                    break;
            }
        }
    }
}


void ServerGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

	sendPacket.serialize(packet_data);

    network->sendToAll(packet_data,packet_size);
}