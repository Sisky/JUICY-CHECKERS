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
#pragma pack(push, 1)
struct JoinServer
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_JOIN_SERVER;
};
#pragma pack(pop)

// This structure will hold all the current avaiable lobbys
#pragma pack(push, 1)
struct GetLobbys
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_GET_LOBBYS;
};
#pragma pack(pop)

// This structure will be sent to the server when the client wants to join
// a lobby, after this the client will get the lobby chat instead of the master chat.
#pragma pack(push, 1)
struct JoinLobby
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_JOIN_LOBBY;
};
#pragma pack(pop)

// This structure will hold lobby chat messages that will be sent to all
// players in the current lobby
#pragma pack(push, 1)
struct LobbyChat
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_LOBBY_CHAT;
};
#pragma pack(pop)

// This structure will hold master chat messages that will be sent to all players that
// are not in the lobby.
#pragma pack(push, 1)
struct MasterChat
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_MASTER_CHAT;
};
#pragma pack(pop)

// This structure will register a client as taking part in the campionship
#pragma pack(push, 1)
struct RegisterChampionship
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_CHAMPIONSHIP_REGISTER;
};
#pragma pack(pop)

// This structure contains a complete representation of the current game, clients
// can get this when they need to completly load the game say for spectating
#pragma pack(push, 1)
struct GameUpdate
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_GAME_UPDATE;
};
#pragma pack(pop)

// This structure is used to move a checker from one location to another 
// it is additionally used to update the game world on the clients end
// after a client moves a checker
#pragma pack(push, 1)
struct MovePiece
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_MOVE_PIECE;
};
#pragma pack(pop)

// This structure is used to update the game world in the case of a checker
// getting king status
#pragma pack(push, 1)
struct KingPiece
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_KING_PIECE;
};
#pragma pack(pop)

// This structure is used to update the game world when a player is taking a piece
#pragma pack(push, 1)
struct TakePiece
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_TAKE_PIECE;
};
#pragma pack(pop)

// This structure is used by the client to purchase an up upgrade
#pragma pack(push, 1)
struct SpendUpgrade
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_SPEND_UPGRADE;
};
#pragma pack(pop)

// This structure is used to get all the upgrades avaiable to the user
#pragma pack(push, 1)
struct GetUpgrades
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_GET_UPGRADES;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Error
{
	const DefaultMessageIDTypes m_eMessageID = ID_USER_ERROR;
};
#pragma pack(pop)

#endif //_DATASTRUCTURES_H__