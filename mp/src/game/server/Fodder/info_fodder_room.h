#pragma once
#include "cbase.h"
#include "info_fodder_exit.h"
#include "info_fodder_spawn.h"

enum RoomType
{
	None,
	Exit,

	ShortCorridor,
	LongCorridor,
	LootCorridor,

	SmallRoom,
	MediumRoom,
	LargeRoom,
	LootRoom
};

class CFodderRoom : public CPointEntity
{
public:
	DECLARE_CLASS( CFodderRoom, CPointEntity );
	DECLARE_DATADESC();

	CFodderRoom();

	void Think();

	RoomType GetRoomType()
	{
		return type;
	}

	void SetRoomType( RoomType type )
	{
		this->type = type;
	}

	//Spawn
	bool CanSpawn()
	{
		return canSpawn;
	}

	void SetCanSpawn( bool canSpawn )
	{
		this->canSpawn = canSpawn;
	}

	//Exits
	void GetExits( CFodderExit *exit[4] )
	{
		if( getEntitys )
			exit = exits;
	}

	CFodderExit *GetExit( int index )
	{
		return exits[index];
	}
	
	int GetNumExits()
	{
		return numExits;
	}

	//Activated
	bool IsActivated()
	{
		return activated;
	}

	void SetActivated( bool active )
	{
		activated = active;
	}

	//Allow exiting the room
	bool ExitAllowed()
	{
		return allowExit;
	}

	void SetExitAllowed( bool allow )
	{
		allowExit = allow;
	}

	bool DisableOnUse()
	{
		return disableOnUse;
	}
	
	//Spawning
	void PlayerEntered();
	void PlayerExited();
	void DeathNotice( CBaseEntity *pVictim );

private:
	RoomType type;
	bool canSpawn;
	bool getEntitys;
	
	int numExits;
	CFodderExit *exits[4];
	string_t exitNames[4];
	bool activated;
	bool disableOnUse;
	bool allowExit;

	string_t spawnerName;
	CUtlVector< CFodderSpawn* > spawners;
	int maxSpawns;
	int enemysKilled;
};