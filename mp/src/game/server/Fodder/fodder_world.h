#pragma once
#include "cbase.h"
#include "info_fodder_room.h"
#include "filesystem.h"

struct PlayerData
{
	CUtlVector< const char* > weapons;
	int health;
	bool saved;
};

class CFodderWorld
{
public:
	CFodderWorld( int maxRooms )
	{
		this->maxRooms = maxRooms;
		numRooms = numFloor = 1;
		data.saved = false;
	}

	bool GetRooms();
	CFodderRoom *GetRandomRoom( RoomType type );
	bool GetRandomLevel();

	//When generating a world a random spawn will be chosen and stored
	CFodderRoom* GetSpawn()
	{
		//Randomly choose a spawning room
		int randomRoom = 0;

		do
		{
			randomRoom = random->RandomInt( 0, rooms.Count()-1 );
		}
		while( !rooms[randomRoom]->CanSpawn() );
		//Keep looping until a spawn is found

		//Set the spawn
		return rooms[randomRoom];
	}

	void Save()
	{
		CBasePlayer *pPlayer = UTIL_GetLocalPlayer();

		for( int i = 0; i < pPlayer->WeaponCount(); i++ )
		{
			if( pPlayer->GetWeapon( i ) )
			{
				char *str = new char[128];
				Q_strcpy( str, pPlayer->GetWeapon( i )->GetClassname() );
				data.weapons.AddToTail( str );
			}
		}

		data.health = pPlayer->GetHealth();
		data.saved = true;
	}

	void Load()
	{
		if( !data.saved )
			return;

		CBasePlayer *pPlayer = UTIL_GetLocalPlayer();
		for( int i = 0; i < data.weapons.Count(); i++ )
		{
			pPlayer->GiveNamedItem( data.weapons[i] );
		}

		pPlayer->SetHealth( data.health );
		data.saved = false;
	}

	void SendFloorData()
	{
		CRecipientFilter filter;
		filter.AddAllPlayers();
 
		UserMessageBegin( filter, "FloorData" );
			WRITE_BYTE( numFloor );
			WRITE_BYTE( numRooms );
		MessageEnd();
	}

	//Singleton structure
	static CFodderWorld* GetCFodderWorld();
	static void ReleaseCFodderWorld();
	static void InitCFodderWorld( int i );	
	static CFodderWorld* instance;

private:
	CUtlVector< CFodderRoom* > rooms;
	int maxRooms;
	int numRooms;
	PlayerData data;

	int numFloor;
};
