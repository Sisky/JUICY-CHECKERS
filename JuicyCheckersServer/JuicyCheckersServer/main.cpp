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

void printWelcome()
{
	printf("|-----------------------------------------------------------|\n");
	printf("|-----------------------------------------------------------|\n");
	printf("|----------@@@ JUICY CHECKERS DEDICATED SERVER @@@----------|\n");
	printf("|-----------------------------------------------------------|\n");
	printf("|-----------------------------------------------------------|\n");
	printf("\n");
	printf("|:  Starting Server Up\n");
	printf("|:  Listening for new clients....\n");
	printf("\n");
}

int main(int argc, TCHAR *argv[])
{
	// Print the welcome
	printWelcome();	

	// Create the Server object
	Server* s = new Server();
	
	// While the server is running process input 
	while (s->isRunning())
	{
		s->Process(0.0f);
	}

	// Clean up
	delete s;
	s = 0;

	return 0;
}

