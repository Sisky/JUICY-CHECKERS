/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: datastructures.h
 *   Description:  This file defines all the network data structures that are sent using raknet.
 *
 */

#ifndef _DATASTRUCTURES_H__
#define _DATASTRUCTURES_H__

#include "MessageIdentifiers.h"

	//ID_USER_JOIN_SERVER,
	//ID_USER_GET_LOBBYS,
	//ID_USER_JOIN_LOBBY,
	//ID_USER_LOBBY_CHAT,
	//ID_USER_MASTER_CHAT,
	//ID_USER_CHAMPIONSHIP_REGISTER,

	//// These packets are going to be game state updates
	//ID_USER_GAME_UPDATE,


// This structure is sent to the server when the clients wants to join
// after this the client will be notified of chat messages on the master
// chat
struct JoinServer
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_JOIN_SERVER;
};

// This structure will hold all the current avaiable lobbys
struct GetLobbys
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_GET_LOBBYS;
};

// This structure will be sent to the server when the client wants to join
// a lobby, after this the client will get the lobby chat instead of the master chat.
struct JoinLobby
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_JOIN_LOBBY;
};

// This structure will hold lobby chat messages that will be sent to all
// players in the current lobby
struct LobbyChat
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_LOBBY_CHAT;
};

// This structure will hold master chat messages that will be sent to all players that
// are not in the lobby.
struct MasterChat
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_MASTER_CHAT;
};

// This structure will register a client as taking part in the campionship
struct RegisterChampionship
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_CHAMPIONSHIP_REGISTER;
};

// This structure contains a complete representation of the current game, clients
// can get this when they need to completly load the game say for spectating
struct GameUpdate
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_GAME_UPDATE;
};

// This structure is used to move a checker from one location to another 
// it is additionally used to update the game world on the clients end
// after a client moves a checker
struct MovePiece
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_MOVE_PIECE;
};

// This structure is used to update the game world in the case of a checker
// getting king status
struct KingPiece
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_KING_PIECE;
};

// This structure is used to update the game world when a player is taking a piece
struct TakePiece
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_TAKE_PIECE;
};

// This structure is used by the client to purchase an up upgrade
struct SpendUpgrade
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_SPEND_UPGRADE;
};

// This structure is used to get all the upgrades avaiable to the user
struct GetUpgrades
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_GET_UPGRADES;
};

struct Error
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_ERROR;
};

#endif //_DATASTRUCTURES_H__