#include "StdAfx.h"
#include "ServerNetwork.h"

std::map<unsigned int, SOCKET> ServerNetwork::sessions;
std::map<int, bool> ServerNetwork::idArray;


ServerNetwork::ServerNetwork(void)
{

	numberOfPlayer = 0;
	rc = false;
	for(int i = 0 ; i < MAX_PLAYER_NUMBER; i ++){
		idArray.insert(pair<int, bool>(i, false));
	}

	// create WSADATA object
    WSADATA wsaData;

    // our sockets for the server
    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;

    // address info for the server to listen to
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }

    // set address information
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;    // TCP connection!!!
    hints.ai_flags = AI_PASSIVE;

	    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    u_long iMode = 1; // 1.nonblocking, 0. blocking
	
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

    if (iResult == SOCKET_ERROR) {
        printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // no longer need address information
    freeaddrinfo(result);

    // start listening for new clients attempting to connect
    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

}


ServerNetwork::~ServerNetwork(void)
{
}

// accept new connections
bool ServerNetwork::acceptNewClient(unsigned int & id)
{
    // if client waiting, accept the connection and save the socket
    ClientSocket = accept(ListenSocket,NULL,NULL);

    if (ClientSocket != INVALID_SOCKET) 
    {
        //disable nagle on the client's socket
        char value = 1;
        setsockopt( ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );

        // insert new client into session id table
        sessions.insert( pair<unsigned int, SOCKET>(id, ClientSocket) );
		numberOfPlayer++;
        return true;
    }

    return false;
}

// receive incoming data
int ServerNetwork::receiveData(unsigned int client_id, char * recvbuf)
{
    if( sessions.find(client_id) != sessions.end() )
    {
        SOCKET currentSocket = sessions[client_id];
        iResult = NetworkServices::receiveMessage(currentSocket, recvbuf, MAX_PACKET_SIZE);

        if (iResult == 0)
        {
            printf("Connection closed\n");
			// shutdown the send half of the connection since no more data will be sent
			iResult = shutdown(currentSocket, SD_SEND);
			if (iResult == SOCKET_ERROR) {
				printf("shutdown failed: %d\n", WSAGetLastError());
				closesocket(currentSocket);
				WSACleanup();
				return 1;
			}
            closesocket(currentSocket);
			idArray[client_id] = false;
			//push disconneted client(s) to a vector
			dcClient.push_back(client_id);
			rc = true;
        }

        return iResult;
    }

    return 0;
}

// send data to all clients
void ServerNetwork::sendToAll(char * packets, int totalSize)
{
    SOCKET currentSocket;
    std::map<unsigned int, SOCKET>::iterator iter;
    int iSendResult;

    for (iter = sessions.begin(); iter != sessions.end(); iter++)
    {
        currentSocket = iter->second;
        iSendResult = NetworkServices::sendMessage(currentSocket, packets, totalSize);

        if (iSendResult!= 10035 && iSendResult == SOCKET_ERROR) 
        {
            printf("send action packet failed with error: %d\n", WSAGetLastError());
            //closesocket(currentSocket);
        }
    }
}

void ServerNetwork::removeClient(){
	rc = false;
	for (unsigned int i = 0; i < dcClient.size(); i++){
		sessions.erase(dcClient[i]);
		numberOfPlayer--;
	}
	dcClient.clear();
}

int ServerNetwork::findClientId(){
	map<int, bool>::iterator it;
	for(it= idArray.begin(); it != idArray.end(); it ++){
		if(!it->second){
			return it->first;
		}
	}
	return -1;
}