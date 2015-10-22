/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: main.cpp
 *   Description:  This is the dedicated server host for Juicy Checkers,  this server manages the hosting of
 *					tournaments and the player lobby.
 *
 */
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"  // MessageID

#include "server.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

enum GameMessages
{
	//ID_GAME_MESSAGE_1=ID_USER_PACKET_ENUM+1
};

int main(int argc, TCHAR *argv[])
{
	char str[512];

	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;
	RakNet::Packet *packet;

	printf("(C) or (S)erver?\n");
	gets(str);

	Server* s = new Server();
	

	while (s->isRunning())
	{
		s->Process(0.0f);
	}


	delete s;
	s = 0;

	return 0;
}