#include "GameClass.h"

GameClass* GameClass::instance = NULL;
unsigned int GameClass::client_id;


GameClass::GameClass()	{
	//renderCounter = 0.0f;
	physicsCounter = 0.0f;
	soundCounter = 0.f;
	elapsed = 0.0f;
	instance = this;

	network = new ServerNetwork();

	client_id = 0;
}

GameClass::~GameClass(void)	{

	for (vector<GameEntity*>::iterator i = allEntities.begin(); i != allEntities.end(); ++i) {
		delete (*i);
	}

	//delete gameCamera;
	
}

void GameClass::UpdatePhysics(float msec) {
	

	physicsCounter += msec;
	elapsed += msec; //for our framerate
	while (physicsCounter >= 0.0f) {
		physicsCounter -= PHYSICS_TIMESTEP;
		PhysicsSystem::GetPhysicsSystem().Update(PHYSICS_TIMESTEP);
	}
	//how many updates did we manage in the last 1sec? ONE HUNDRED AND TWENTY I HOPE
	if (elapsed > 1000.0f) {
		PhysicsSystem::numUpdatesPerSec = PhysicsSystem::numUpdates;
		PhysicsSystem::numUpdates = 0;
		elapsed = 0.0f;
	}
}

//void GameClass::UpdateRendering(float msec) {
//	renderCounter -= msec;
//
//	if (renderCounter <= 0.0f) {	//Update our rendering logic
//		Renderer::GetRenderer().UpdateScene(1000.0f / (float)RENDER_HZ);
//		Renderer::GetRenderer().RenderScene();
//		renderCounter += (1000.0f / (float)RENDER_HZ);
//	}
//}


void GameClass::UpdateNetwork()
{
	// get new clients
	if (network->acceptNewClient(client_id))
	{
		printf("client %d has been connected to the server\n", client_id);
		network->idArray[client_id] = true;
		client_id = network->findClientId();
	}

	ReceiveFromClients();
	if (network->rc){
		client_id = network->findClientId();
		network->removeClient();
	}
}

void GameClass::ReceiveFromClients()
{

	// go through all clients
	std::map<unsigned int, SOCKET>::iterator iter;
	for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
	{
		int data_length = network->receiveData(iter->first, network_data);

		if (data_length <= 0)
		{
			//no data recieved
			continue;
		}

		int i = 0;
		/*while (i < (unsigned int)data_length)
		{*/
			recvPacket.deserialize(&(network_data[i]));
			i += sizeof(Packet);

			switch (recvPacket.packet_type) {

			case INIT_CONNECTION:

				sendPacket.packet_type = INIT_CONNECTION;
				sendPacket.clientNumber = iter->first;
				//init position of spaceship
				sendPacket.positon[iter->first] = Vector3(rand()%1000, 0, 0);
				printf("server received init packet from client\n");

				SendInitPacket(sendPacket.clientNumber);

				break;

			case ACTION_EVENT:

				sendPacket.packet_type = ACTION_EVENT;
				sendPacket.trackSeed = rand() % RAND_MAX;

				for (int i = 0; i < NUMBER_OF_INPUT; i++){
					if (recvPacket.inputs[i] == 1){
						switch (i){
						case KEY_I:
							cout << "Receive user input " << i << endl;
							sendPacket.positon[iter->first] += Vector3(0, 10, 0);
							break;
						default:
							break;
						}
					}
				}

				sendPacket.numberOfPlayers = network->numberOfPlayer;
				SendActionPackets();
				break;

			default:

				printf("error in packet types\n");

				break;
			}
		//}
	}
}


void GameClass::SendActionPackets()
{
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	sendPacket.serialize(packet_data);

	network->sendToAll(packet_data, packet_size);
}

void GameClass::SendInitPacket(int clientNumber){

	// send init packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	sendPacket.serialize(packet_data);

	SOCKET currentSocket;
	std::map<unsigned int, SOCKET>::iterator iter;
	int iSendResult;

	currentSocket  = ServerNetwork::sessions.find(clientNumber)->second;

	iSendResult = NetworkServices::sendMessage(currentSocket, packet_data, packet_size);

	if (iSendResult!=10035 && iSendResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		//closesocket(currentSocket);
	}

}